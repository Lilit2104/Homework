/*
 * Origin server — TCP loopback (communication type 1)
 * ---------------------------------------------------
 *
 * Replaces shared-memory IPC with TCP on the loopback address.
 *
 *   Proxy  --connect()-->  127.0.0.1:PORT  <--listen--  Origin
 *
 * Proxy responsibilities:
 *   - Start this origin (or assume it is already running)
 *   - open up to 10 TCP connections to 127.0.0.1:PORT
 *   - each connection is one bidirectional channel (request + response)
 *   - put those sockets into the same select()/poll() loop as clients
 *
 * Origin behavior:
 *   - listen on 127.0.0.1 (default port 9000)
 *   - accept at most 10 simultaneous channels
 *   - poll() listening socket + connected sockets
 *   - read complete HTTP-like request, simulate work, write response
 *
 * Build:  make origin_tcp
 * Run:    ./origin_tcp
 *         ./origin_tcp 9000
 *         ./origin_tcp --selftest
 */

#include "protocol.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define DEFAULT_PORT 9000

static volatile sig_atomic_t g_stop = 0;

static void on_sigint(int sig)
{
    (void)sig;
    g_stop = 1;
}

typedef struct {
    int active;
    int fd;
    char *buf;
    size_t len;
    size_t cap;
} channel_t;

static channel_t g_ch[ORIGIN_CHANNELS];

static int listen_loopback(int port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(1);
    }

    int yes = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* 127.0.0.1 */
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    if (listen(fd, ORIGIN_CHANNELS) < 0) {
        perror("listen");
        exit(1);
    }

    printf("[origin_tcp] listening on 127.0.0.1:%d (max %d channels)\n",
           port, ORIGIN_CHANNELS);
    return fd;
}

static void channel_close(int i)
{
    if (!g_ch[i].active)
        return;
    close(g_ch[i].fd);
    free(g_ch[i].buf);
    memset(&g_ch[i], 0, sizeof(g_ch[i]));
    printf("[origin_tcp] channel %d closed\n", i);
}

static int channel_add(int fd)
{
    for (int i = 0; i < ORIGIN_CHANNELS; i++) {
        if (!g_ch[i].active) {
            g_ch[i].active = 1;
            g_ch[i].fd = fd;
            g_ch[i].buf = NULL;
            g_ch[i].len = 0;
            g_ch[i].cap = 0;
            printf("[origin_tcp] channel %d accepted fd=%d\n", i, fd);
            return i;
        }
    }
    return -1;
}

static int serve(int port)
{
    int lfd = listen_loopback(port);
    printf("[origin_tcp] ready (Ctrl+C to stop)\n");

    while (!g_stop) {
        struct pollfd pfds[ORIGIN_CHANNELS + 1];
        int cmap[ORIGIN_CHANNELS + 1];
        int nfds = 0;

        pfds[0].fd = lfd;
        pfds[0].events = POLLIN;
        pfds[0].revents = 0;
        cmap[0] = -1;
        nfds = 1;

        for (int i = 0; i < ORIGIN_CHANNELS; i++) {
            if (!g_ch[i].active)
                continue;
            cmap[nfds] = i;
            pfds[nfds].fd = g_ch[i].fd;
            pfds[nfds].events = POLLIN;
            pfds[nfds].revents = 0;
            nfds++;
        }

        int pr = poll(pfds, (nfds_t)nfds, 500);
        if (pr < 0) {
            if (errno == EINTR)
                continue;
            perror("poll");
            break;
        }
        if (pr == 0)
            continue;

        if (pfds[0].revents & POLLIN) {
            int cfd = accept(lfd, NULL, NULL);
            if (cfd >= 0) {
                if (channel_add(cfd) < 0) {
                    const char *msg =
                        "HTTP/1.1 503 Service Unavailable\n"
                        "Content-Length: 0\n"
                        "Connection: close\n"
                        "\n";
                    origin_write_all(cfd, msg, strlen(msg));
                    close(cfd);
                    fprintf(stderr, "[origin_tcp] rejected: all %d channels busy\n",
                            ORIGIN_CHANNELS);
                }
            }
        }

        for (int i = 1; i < nfds; i++) {
            int ci = cmap[i];
            if (ci < 0)
                continue;
            short rev = pfds[i].revents;
            if (rev & (POLLERR | POLLHUP | POLLNVAL)) {
                channel_close(ci);
                continue;
            }
            if (rev & POLLIN) {
                int rc = origin_serve_one(g_ch[ci].fd, g_ch[ci].fd,
                                          &g_ch[ci].buf, &g_ch[ci].len,
                                          &g_ch[ci].cap);
                if (rc <= 0)
                    channel_close(ci);
            }
        }
    }

    printf("\n[origin_tcp] shutting down\n");
    for (int i = 0; i < ORIGIN_CHANNELS; i++)
        channel_close(i);
    close(lfd);
    return 0;
}

static int run_selftest(int port)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        signal(SIGINT, on_sigint);
        signal(SIGPIPE, SIG_IGN);
        serve(port);
        _exit(0);
    }

    /* Parent = stand-in proxy: connect to loopback and send one request */
    usleep(150000);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons((uint16_t)port);

    int connected = 0;
    for (int t = 0; t < 50; t++) {
        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
            connected = 1;
            break;
        }
        usleep(50000);
    }
    if (!connected) {
        perror("connect");
        kill(pid, SIGINT);
        return 1;
    }

    const char *req =
        "GET /index HTTP/1.1\n"
        "Content-Length: 0\n"
        "Connection: close\n"
        "\n";
    if (origin_write_all(fd, req, strlen(req)) < 0) {
        perror("write");
        return 1;
    }

    char buf[1024];
    size_t got = 0;
    while (got < sizeof(buf) - 1) {
        ssize_t n = read(fd, buf + got, sizeof(buf) - 1 - got);
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

    close(fd);
    kill(pid, SIGINT);
    waitpid(pid, NULL, 0);
    return 0;
}

static void usage(const char *argv0)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s [port]           Listen on 127.0.0.1:port (default %d)\n"
            "  %s --selftest [port]\n",
            argv0, DEFAULT_PORT, argv0);
}

int main(int argc, char **argv)
{
    int port = DEFAULT_PORT;
    int do_selftest = 0;

    signal(SIGINT, on_sigint);
    signal(SIGTERM, on_sigint);
    signal(SIGPIPE, SIG_IGN);
    setvbuf(stdout, NULL, _IOLBF, 0);
    setvbuf(stderr, NULL, _IOLBF, 0);

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--selftest"))
            do_selftest = 1;
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage(argv[0]);
            return 0;
        } else {
            port = atoi(argv[i]);
            if (port <= 0 || port > 65535) {
                fprintf(stderr, "bad port\n");
                return 1;
            }
        }
    }

    if (do_selftest) {
        /* Prefer a high port for selftest to avoid clashing with a running origin. */
        if (argc == 2)
            port = 19000;
        return run_selftest(port);
    }
    return serve(port);
}
