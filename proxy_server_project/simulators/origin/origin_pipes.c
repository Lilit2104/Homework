/*
 * Origin server — Pipes (communication type 2)
 * --------------------------------------------
 *
 * Parent PROXY responsibilities:
 *   - Create 20 pipe ends = 10 pairs
 *     pair[i].to_origin   : proxy WRITES request,  origin READS
 *     pair[i].from_origin : origin WRITES response, proxy READS
 *   - fork() child origin
 *   - In the child, keep origin-side ends and dup2 them to fixed fds
 *     (or pass fd list), then exec this program
 *   - In the parent, close origin-side ends; poll/select on proxy-side ends
 *
 * Origin behavior:
 *   - poll() all request (read) pipes
 *   - when pair i is readable: read one complete request
 *   - simulate parse/generate
 *   - write the full response to pair i response pipe
 *
 * Default fd map (after proxy dup2 in child):
 *   pair i: read_fd = BASE + 2*i
 *           write_fd = BASE + 2*i + 1
 *   BASE default = 100
 *
 * Build:  make origin_pipes
 * Run:    ./origin_pipes
 *         ./origin_pipes --base-fd 100
 *         ./origin_pipes --pairs 3:4,5:6,7:8,...
 *         ./origin_pipes --selftest
 */

#include "protocol.h"

#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static volatile sig_atomic_t g_stop = 0;

static void on_sigint(int sig)
{
    (void)sig;
    g_stop = 1;
}

typedef struct {
    int read_fd;  /* request from proxy */
    int write_fd; /* response to proxy */
    char *buf;
    size_t len;
    size_t cap;
} pair_t;

static int serve_pairs(pair_t *pairs, int n_pairs)
{
    printf("[origin_pipes] serving %d pairs\n", n_pairs);

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
            short rev = pfds[i].revents;
            if (rev & (POLLERR | POLLNVAL)) {
                fprintf(stderr, "[origin_pipes] pair %d error\n", i);
                continue;
            }
            if (rev & POLLHUP) {
                /* writer closed; drain if data pending */
            }
            if (!(rev & POLLIN))
                continue;

            int rc = origin_serve_one(pairs[i].read_fd, pairs[i].write_fd,
                                      &pairs[i].buf, &pairs[i].len, &pairs[i].cap);
            if (rc == 0) {
                fprintf(stderr, "[origin_pipes] pair %d EOF\n", i);
            } else if (rc < 0) {
                fprintf(stderr, "[origin_pipes] pair %d I/O error\n", i);
            }
        }
    }
    return 0;
}

static int parse_pairs_arg(const char *arg, pair_t *pairs, int *n_out)
{
    char *copy = strdup(arg);
    if (!copy)
        return -1;
    int n = 0;
    char *save = NULL;
    for (char *tok = strtok_r(copy, ",", &save); tok; tok = strtok_r(NULL, ",", &save)) {
        int r = -1, w = -1;
        if (sscanf(tok, "%d:%d", &r, &w) != 2 || r < 0 || w < 0) {
            free(copy);
            return -1;
        }
        if (n >= ORIGIN_CHANNELS) {
            free(copy);
            return -1;
        }
        pairs[n].read_fd = r;
        pairs[n].write_fd = w;
        pairs[n].buf = NULL;
        pairs[n].len = 0;
        pairs[n].cap = 0;
        n++;
    }
    free(copy);
    *n_out = n;
    return n > 0 ? 0 : -1;
}

static void fill_base_fd(pair_t *pairs, int base)
{
    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        pairs[i].read_fd = base + 2 * i;
        pairs[i].write_fd = base + 2 * i + 1;
        pairs[i].buf = NULL;
        pairs[i].len = 0;
        pairs[i].cap = 0;
    }
}

static int run_selftest(void)
{
    int to_origin[ORIGIN_CHANNELS][2];
    int from_origin[ORIGIN_CHANNELS][2];
    pair_t child_pairs[ORIGIN_CHANNELS];

    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        if (pipe(to_origin[i]) < 0 || pipe(from_origin[i]) < 0) {
            perror("pipe");
            return 1;
        }
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        signal(SIGINT, on_sigint);
        for (int i = 0; i < ORIGIN_CHANNELS; i++) {
            close(to_origin[i][1]);
            close(from_origin[i][0]);
            child_pairs[i].read_fd = to_origin[i][0];
            child_pairs[i].write_fd = from_origin[i][1];
            child_pairs[i].buf = NULL;
            child_pairs[i].len = 0;
            child_pairs[i].cap = 0;
        }
        serve_pairs(child_pairs, ORIGIN_CHANNELS);
        _exit(0);
    }

    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        close(to_origin[i][0]);
        close(from_origin[i][1]);
    }

    const char *req =
        "GET /data HTTP/1.1\n"
        "Content-Length: 0\n"
        "Connection: close\n"
        "\n";
    if (origin_write_all(to_origin[0][1], req, strlen(req)) < 0) {
        perror("write");
        return 1;
    }

    char buf[1024];
    size_t got = 0;
    while (got < sizeof(buf) - 1) {
        ssize_t n = read(from_origin[0][0], buf + got, sizeof(buf) - 1 - got);
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
        if (strstr(buf, "\n\n")) {
            /* have headers; for this small response, body is short */
            char *p = strstr(buf, "Content-Length:");
            long cl = 0;
            if (p)
                cl = strtol(p + 15, NULL, 10);
            char *he = strstr(buf, "\n\n");
            size_t header_end = (size_t)(he - buf) + 2;
            if (got >= header_end + (size_t)cl)
                break;
        }
    }
    fwrite(buf, 1, got, stdout);
    printf("\n[selftest] OK (%zu bytes)\n", got);

    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        close(to_origin[i][1]);
        close(from_origin[i][0]);
    }
    kill(pid, SIGINT);
    waitpid(pid, NULL, 0);
    return 0;
}

static void usage(const char *argv0)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s\n"
            "  %s --base-fd N\n"
            "  %s --pairs r:w,r:w,...\n"
            "  %s --selftest\n",
            argv0, argv0, argv0, argv0);
}

int main(int argc, char **argv)
{
    signal(SIGINT, on_sigint);
    signal(SIGTERM, on_sigint);
    signal(SIGPIPE, SIG_IGN);
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IOLBF, 0);

    pair_t pairs[ORIGIN_CHANNELS];
    int n_pairs = ORIGIN_CHANNELS;
    int base = 100;
    int have_pairs = 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--selftest"))
            return run_selftest();
        if (!strcmp(argv[i], "--base-fd") && i + 1 < argc) {
            base = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "--pairs") && i + 1 < argc) {
            if (parse_pairs_arg(argv[++i], pairs, &n_pairs) < 0) {
                fprintf(stderr, "bad --pairs\n");
                return 1;
            }
            have_pairs = 1;
        } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage(argv[0]);
            return 0;
        } else {
            usage(argv[0]);
            return 1;
        }
    }

    if (!have_pairs)
        fill_base_fd(pairs, base);

    return serve_pairs(pairs, n_pairs);
}
