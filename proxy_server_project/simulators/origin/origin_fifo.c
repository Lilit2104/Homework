/*
 * Origin server — Named FIFOs (communication type 3)
 * --------------------------------------------------
 *
 * Same pair logic as pipes, but using FIFO files.
 * There is NO parent/child hierarchy: proxy and origin are independent
 * processes. The PROXY creates the FIFO files; the origin only opens them.
 *
 * For each channel i in [0 .. 9]:
 *   <prefix>_<i>_c2s   proxy -> origin  (origin reads requests)
 *   <prefix>_<i>_s2c   origin -> proxy  (origin writes responses)
 *
 * Default prefix: /tmp/http_proxy_fifo
 * Example files:
 *   /tmp/http_proxy_fifo_0_c2s
 *   /tmp/http_proxy_fifo_0_s2c
 *   ...
 *   /tmp/http_proxy_fifo_9_c2s
 *   /tmp/http_proxy_fifo_9_s2c
 *
 * Origin:
 *   - open all 20 FIFOs
 *   - poll() on the 10 c2s (read) ends
 *   - read request, simulate work, write response on matching s2c
 *
 * Build:  make origin_fifo
 * Run:    ./origin_fifo
 *         ./origin_fifo --prefix /tmp/http_proxy_fifo
 *         ./origin_fifo --selftest
 */

#include "protocol.h"

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_PREFIX "/tmp/http_proxy_fifo"

static volatile sig_atomic_t g_stop = 0;

static void on_sigint(int sig)
{
    (void)sig;
    g_stop = 1;
}

typedef struct {
    int read_fd;
    int write_fd;
    char *buf;
    size_t len;
    size_t cap;
} pair_t;

static void fifo_path(char *out, size_t n, const char *prefix, int i, int c2s)
{
    snprintf(out, n, "%s_%d_%s", prefix, i, c2s ? "c2s" : "s2c");
}

static int create_fifos(const char *prefix)
{
    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        char a[256], b[256];
        fifo_path(a, sizeof(a), prefix, i, 1);
        fifo_path(b, sizeof(b), prefix, i, 0);
        unlink(a);
        unlink(b);
        if (mkfifo(a, 0600) < 0 || mkfifo(b, 0600) < 0) {
            perror("mkfifo");
            return -1;
        }
    }
    return 0;
}

static int open_pair(const char *prefix, int i, pair_t *p)
{
    char c2s[256], s2c[256];
    fifo_path(c2s, sizeof(c2s), prefix, i, 1);
    fifo_path(s2c, sizeof(s2c), prefix, i, 0);

    /*
     * Open O_RDWR to avoid classic FIFO open-block deadlock when the
     * peer has not opened the other end yet. Works on Linux and macOS.
     */
    p->read_fd = open(c2s, O_RDWR);
    if (p->read_fd < 0) {
        fprintf(stderr, "open(%s): %s\n", c2s, strerror(errno));
        return -1;
    }
    p->write_fd = open(s2c, O_RDWR);
    if (p->write_fd < 0) {
        fprintf(stderr, "open(%s): %s\n", s2c, strerror(errno));
        close(p->read_fd);
        return -1;
    }
    p->buf = NULL;
    p->len = 0;
    p->cap = 0;
    return 0;
}

static int serve_pairs(pair_t *pairs, int n_pairs)
{
    printf("[origin_fifo] serving %d FIFO pairs\n", n_pairs);

    while (!g_stop) {
        struct pollfd pfds[ORIGIN_CHANNELS];
        for (int i = 0; i < n_pairs; i++) {
            pfds[i].fd = pairs[i].read_fd;
            pfds[i].events = POLLIN;
            pfds[i].revents = 0;
        }

        int pr = poll(pfds, (nfds_t)n_pairs, 500);
        if (pr < 0) {
            if (errno == EINTR)
                continue;
            perror("poll");
            return 1;
        }
        if (pr == 0)
            continue;

        for (int i = 0; i < n_pairs; i++) {
            if (!(pfds[i].revents & POLLIN))
                continue;
            int rc = origin_serve_one(pairs[i].read_fd, pairs[i].write_fd,
                                      &pairs[i].buf, &pairs[i].len, &pairs[i].cap);
            if (rc < 0)
                fprintf(stderr, "[origin_fifo] channel %d error\n", i);
        }
    }
    return 0;
}

static int run_worker(const char *prefix)
{
    pair_t pairs[ORIGIN_CHANNELS];
    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        if (open_pair(prefix, i, &pairs[i]) < 0)
            return 1;
        printf("[origin_fifo] channel %d opened\n", i);
    }
    return serve_pairs(pairs, ORIGIN_CHANNELS);
}

static int run_selftest(const char *prefix)
{
    if (create_fifos(prefix) < 0)
        return 1;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        signal(SIGINT, on_sigint);
        signal(SIGPIPE, SIG_IGN);
        int rc = run_worker(prefix);
        _exit(rc);
    }

    /* Parent = stand-in proxy (independent process role) */
    usleep(100000);
    pair_t proxy;
    if (open_pair(prefix, 0, &proxy) < 0)
        return 1;

    const char *req =
        "GET /index HTTP/1.1\n"
        "Content-Length: 0\n"
        "Connection: close\n"
        "\n";
    /* proxy writes to c2s, reads from s2c — opposite of origin open_pair roles?
     * open_pair opens c2s as read_fd and s2c as write_fd (origin view).
     * For proxy we need write c2s, read s2c. Re-open explicitly.
     */
    close(proxy.read_fd);
    close(proxy.write_fd);

    char c2s[256], s2c[256];
    fifo_path(c2s, sizeof(c2s), prefix, 0, 1);
    fifo_path(s2c, sizeof(s2c), prefix, 0, 0);
    int w = open(c2s, O_RDWR);
    int r = open(s2c, O_RDWR);
    if (w < 0 || r < 0) {
        perror("proxy open");
        return 1;
    }

    if (origin_write_all(w, req, strlen(req)) < 0) {
        perror("write");
        return 1;
    }

    char buf[1024];
    size_t got = 0;
    while (got < sizeof(buf) - 1) {
        ssize_t n = read(r, buf + got, sizeof(buf) - 1 - got);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            perror("read");
            return 1;
        }
        if (n == 0)
            break;
        got += (size_t)n;
        buf[got] = '\0';
        if (strstr(buf, "\n\n"))
            break;
    }
    fwrite(buf, 1, got, stdout);
    printf("\n[selftest] OK (%zu bytes)\n", got);

    close(w);
    close(r);
    kill(pid, SIGINT);
    waitpid(pid, NULL, 0);

    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        char a[256], b[256];
        fifo_path(a, sizeof(a), prefix, i, 1);
        fifo_path(b, sizeof(b), prefix, i, 0);
        unlink(a);
        unlink(b);
    }
    return 0;
}

static void usage(const char *argv0)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s [--prefix PATH]\n"
            "  %s --selftest [--prefix PATH]\n"
            "  %s --create [--prefix PATH]   create FIFOs then serve\n",
            argv0, argv0, argv0);
}

int main(int argc, char **argv)
{
    const char *prefix = DEFAULT_PREFIX;
    int do_selftest = 0;
    int do_create = 0;

    signal(SIGINT, on_sigint);
    signal(SIGTERM, on_sigint);
    signal(SIGPIPE, SIG_IGN);
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IOLBF, 0);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--prefix") && i + 1 < argc)
            prefix = argv[++i];
        else if (!strcmp(argv[i], "--selftest"))
            do_selftest = 1;
        else if (!strcmp(argv[i], "--create"))
            do_create = 1;
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage(argv[0]);
            return 0;
        } else {
            usage(argv[0]);
            return 1;
        }
    }

    if (do_selftest)
        return run_selftest(prefix);

    if (do_create && create_fifos(prefix) < 0)
        return 1;

    return run_worker(prefix);
}
