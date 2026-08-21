/*
 * Example attach sketches for the three origin communication types.
 * Documentation only — not built by the Makefile.
 */

#if 0

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHANNELS 10

/* ========== 1) TCP loopback ========== */

int connect_origin_tcp(unsigned short port)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* 127.0.0.1 */
    addr.sin_port = htons(port);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }
    return fd; /* one bidirectional channel */
}

/* Proxy may open up to 10 such connections and pool them. */

/* ========== 2) Pipes ========== */

int spawn_origin_pipes(pid_t *out_pid,
                       int to_origin_write[CHANNELS],
                       int from_origin_read[CHANNELS])
{
    int to_origin[CHANNELS][2];
    int from_origin[CHANNELS][2];
    for (int i = 0; i < CHANNELS; i++) {
        pipe(to_origin[i]);
        pipe(from_origin[i]);
    }

    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i < CHANNELS; i++) {
            close(to_origin[i][1]);
            close(from_origin[i][0]);
            dup2(to_origin[i][0], 100 + 2 * i);
            dup2(from_origin[i][1], 100 + 2 * i + 1);
            close(to_origin[i][0]);
            close(from_origin[i][1]);
        }
        execl("./origin_pipes", "origin_pipes", "--base-fd", "100", (char *)NULL);
        _exit(127);
    }

    for (int i = 0; i < CHANNELS; i++) {
        close(to_origin[i][0]);
        close(from_origin[i][1]);
        to_origin_write[i] = to_origin[i][1];
        from_origin_read[i] = from_origin[i][0];
    }
    *out_pid = pid;
    return 0;
}

/* ========== 3) FIFOs (no hierarchy) ========== */

int create_origin_fifos(const char *prefix)
{
    for (int i = 0; i < CHANNELS; i++) {
        char c2s[256], s2c[256];
        snprintf(c2s, sizeof(c2s), "%s_%d_c2s", prefix, i);
        snprintf(s2c, sizeof(s2c), "%s_%d_s2c", prefix, i);
        unlink(c2s);
        unlink(s2c);
        mkfifo(c2s, 0600);
        mkfifo(s2c, 0600);
    }
    return 0;
}

/* Then start ./origin_fifo in another process; proxy opens c2s write, s2c read. */

#endif
