/*
 * Test Client Simulator
 * ---------------------
 * Sends simplified HTTP-like requests to a student proxy for debugging.
 *
 * Usage:
 *   ./test_client <proxy_host> <proxy_port> <command> [args...]
 *   ./test_client <proxy_host> <proxy_port> scenario <name>
 *
 * Commands:
 *   get PATH
 *   head PATH
 *   post PATH BODY
 *   options PATH
 *   raw FILE                 Send exact bytes from a file
 *   keepalive                Two GET requests on one connection
 *   close                    GET with Connection: close
 *   forbidden                POST body containing blocked keyword
 *   oversized-body           POST with Content-Length > 8192
 *   oversized-header         Headers larger than 4096 bytes
 *   eleven                   Open 11 connections (expect 503 on last)
 *   concurrent N             N parallel GETs (N <= 10 recommended)
 *   big                      GET /big (large origin response)
 *   scenario all             Run built-in smoke scenarios sequentially
 *
 * Line endings: LF only (\n). Header section ends with \n\n.
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define RECV_BUF (128 * 1024)
#define RECV_TIMEOUT_SEC 5

typedef struct {
    char host[256];
    int  port;
    int  id;
    int  ok;
} concurrent_arg_t;

static int connect_tcp(const char *host, int port)
{
    char portstr[16];
    snprintf(portstr, sizeof(portstr), "%d", port);

    struct addrinfo hints, *res = NULL, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int ga = getaddrinfo(host, portstr, &hints, &res);
    if (ga != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ga));
        return -1;
    }

    int fd = -1;
    for (rp = res; rp; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd < 0)
            continue;
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(fd);
        fd = -1;
    }
    freeaddrinfo(res);

    /*
     * Give up instead of hanging forever when the proxy never answers.
     * A missing response must show as a [FAIL] line, not a frozen terminal.
     */
    if (fd >= 0) {
        struct timeval tv;
        tv.tv_sec = RECV_TIMEOUT_SEC;
        tv.tv_usec = 0;
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    }
    return fd;
}

static int send_all(int fd, const char *buf, size_t len)
{
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = send(fd, buf + sent, len - sent, 0);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            perror("send");
            return -1;
        }
        sent += (size_t)n;
    }
    return 0;
}

static int recv_response(int fd, char *out, size_t out_cap, size_t *out_len,
                         int expect_body)
{
    size_t total = 0;
    int headers_done = 0;
    long content_length = -1;
    size_t header_end = 0;

    while (total + 1 < out_cap) {
        ssize_t n = recv(fd, out + total, out_cap - total - 1, 0);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                fprintf(stderr,
                        "[timeout] no response after %d s (%zu bytes so far)\n",
                        RECV_TIMEOUT_SEC, total);
                break;
            }
            perror("recv");
            return -1;
        }
        if (n == 0)
            break;
        total += (size_t)n;
        out[total] = '\0';

        if (!headers_done) {
            char *p = strstr(out, "\n\n");
            if (p) {
                headers_done = 1;
                header_end = (size_t)(p - out) + 2;
                content_length = 0;
                char *line = out;
                while (line < p) {
                    char *nl = strchr(line, '\n');
                    if (!nl || nl > p)
                        break;
                    if (strncasecmp(line, "Content-Length:", 15) == 0) {
                        content_length = strtol(line + 15, NULL, 10);
                    }
                    line = nl + 1;
                }
                /* HEAD responses advertise Content-Length but send no body */
                if (!expect_body)
                    break;
            }
        }

        if (headers_done && expect_body) {
            size_t body_have = (total > header_end) ? (total - header_end) : 0;
            if (content_length >= 0 && body_have >= (size_t)content_length)
                break;
        }
    }

    if (out_len)
        *out_len = total;
    out[total] = '\0';
    return (int)total;
}

static void print_response(const char *label, const char *buf, size_t len)
{
    printf("\n===== %s (%zu bytes) =====\n", label, len);
    fwrite(buf, 1, len, stdout);
    if (len == 0 || buf[len - 1] != '\n')
        printf("\n");
    printf("===== end =====\n");
}

static int status_code(const char *resp)
{
    int code = 0;
    if (sscanf(resp, "HTTP/%*s %d", &code) == 1)
        return code;
    return -1;
}

static int expect_status(const char *label, const char *resp, int want)
{
    int got = status_code(resp);
    if (got == want) {
        printf("[PASS] %s -> %d\n", label, got);
        return 0;
    }
    printf("[FAIL] %s -> got %d, expected %d\n", label, got, want);
    return 1;
}

static char *build_request(const char *method, const char *path,
                           const char *extra_headers,
                           const char *body, size_t body_len,
                           int connection_close)
{
    size_t cap = 4096 + body_len + 256;
    char *req = malloc(cap);
    if (!req)
        return NULL;

    int n = snprintf(req, cap,
                     "%s %s HTTP/1.1\n"
                     "%s"
                     "Content-Length: %zu\n"
                     "Connection: %s\n"
                     "\n",
                     method, path,
                     extra_headers ? extra_headers : "",
                     body_len,
                     connection_close ? "close" : "keep-alive");
    if (n < 0 || (size_t)n >= cap) {
        free(req);
        return NULL;
    }
    if (body_len > 0 && body)
        memcpy(req + n, body, body_len);
    /* store total length in unused way: append NUL after full request */
    req[n + body_len] = '\0';
    /* We need length separately — encode as: caller uses strlen only if no
       embedded NUL in body. For binary-safe use, return via out param.
       Here bodies are text. */
    return req;
}

static size_t request_len(const char *method, const char *path,
                          const char *extra_headers,
                          size_t body_len, int connection_close)
{
    char tmp[1024];
    int n = snprintf(tmp, sizeof(tmp),
                     "%s %s HTTP/1.1\n"
                     "%s"
                     "Content-Length: %zu\n"
                     "Connection: %s\n"
                     "\n",
                     method, path,
                     extra_headers ? extra_headers : "",
                     body_len,
                     connection_close ? "close" : "keep-alive");
    return (size_t)n + body_len;
}

static int do_exchange(const char *host, int port,
                       const char *req, size_t req_len,
                       char *resp, size_t resp_cap, size_t *resp_len,
                       int reuse_fd, int *inout_fd, int expect_body)
{
    int fd;
    if (reuse_fd && inout_fd && *inout_fd >= 0) {
        fd = *inout_fd;
    } else {
        fd = connect_tcp(host, port);
        if (fd < 0) {
            fprintf(stderr, "connect to %s:%d failed\n", host, port);
            return -1;
        }
        if (inout_fd)
            *inout_fd = fd;
    }

    if (send_all(fd, req, req_len) < 0) {
        close(fd);
        if (inout_fd)
            *inout_fd = -1;
        return -1;
    }

    size_t got = 0;
    if (recv_response(fd, resp, resp_cap, &got, expect_body) < 0) {
        close(fd);
        if (inout_fd)
            *inout_fd = -1;
        return -1;
    }
    if (resp_len)
        *resp_len = got;

    if (!reuse_fd) {
        close(fd);
        if (inout_fd)
            *inout_fd = -1;
    }
    return 0;
}

static int cmd_simple(const char *host, int port,
                      const char *method, const char *path,
                      const char *body, int close_conn)
{
    size_t blen = body ? strlen(body) : 0;
    size_t rlen = request_len(method, path, NULL, blen, close_conn);
    char *req = build_request(method, path, NULL, body, blen, close_conn);
    if (!req)
        return 1;

    char resp[RECV_BUF];
    size_t resp_len = 0;
    int expect_body = strcmp(method, "HEAD") != 0;
    int rc = do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len,
                         0, NULL, expect_body);
    free(req);
    if (rc < 0)
        return 1;
    print_response(method, resp, resp_len);
    return 0;
}

static int cmd_keepalive(const char *host, int port)
{
    int fd = -1;
    char resp[RECV_BUF];
    size_t resp_len;
    int fails = 0;

    const char *body = NULL;
    size_t rlen = request_len("GET", "/index", NULL, 0, 0);
    char *req = build_request("GET", "/index", NULL, body, 0, 0);
    if (!req)
        return 1;

    if (do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 1, &fd, 1) < 0) {
        free(req);
        return 1;
    }
    print_response("keepalive #1", resp, resp_len);
    fails += expect_status("keepalive #1", resp, 200);

    if (do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 1, &fd, 1) < 0) {
        free(req);
        if (fd >= 0)
            close(fd);
        return 1;
    }
    print_response("keepalive #2", resp, resp_len);
    fails += expect_status("keepalive #2", resp, 200);

    free(req);
    if (fd >= 0)
        close(fd);
    return fails ? 1 : 0;
}

static int cmd_forbidden(const char *host, int port)
{
    const char *body = "hello forbidden world";
    size_t blen = strlen(body);
    size_t rlen = request_len("POST", "/message", NULL, blen, 1);
    char *req = build_request("POST", "/message", NULL, body, blen, 1);
    if (!req)
        return 1;

    char resp[RECV_BUF];
    size_t resp_len = 0;
    int rc = do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 1);
    free(req);
    if (rc < 0)
        return 1;
    print_response("forbidden POST", resp, resp_len);
    /* Proxy should reject with 403 before reaching origin */
    return expect_status("forbidden POST", resp, 403);
}

static int cmd_oversized_body(const char *host, int port)
{
    size_t blen = 8193;
    char *body = malloc(blen);
    if (!body)
        return 1;
    memset(body, 'X', blen);

    size_t rlen = request_len("POST", "/message", NULL, blen, 1);
    char *req = build_request("POST", "/message", NULL, body, blen, 1);
    free(body);
    if (!req)
        return 1;

    char resp[RECV_BUF];
    size_t resp_len = 0;
    int rc = do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 1);
    free(req);
    if (rc < 0)
        return 1;
    print_response("oversized body", resp, resp_len);
    return expect_status("oversized body", resp, 413);
}

static int cmd_oversized_header(const char *host, int port)
{
    /* Build a request whose header section exceeds 4096 bytes */
    size_t pad = 4200;
    char *junk = malloc(pad + 1);
    if (!junk)
        return 1;
    memset(junk, 'A', pad);
    junk[pad] = '\0';

    size_t cap = pad + 512;
    char *req = malloc(cap);
    if (!req) {
        free(junk);
        return 1;
    }
    int n = snprintf(req, cap,
                     "GET /index HTTP/1.1\n"
                     "X-Padding: %s\n"
                     "Connection: close\n"
                     "\n",
                     junk);
    free(junk);
    if (n < 0 || (size_t)n >= cap) {
        free(req);
        return 1;
    }

    char resp[RECV_BUF];
    size_t resp_len = 0;
    int rc = do_exchange(host, port, req, (size_t)n, resp, sizeof(resp), &resp_len, 0, NULL, 1);
    free(req);
    if (rc < 0)
        return 1;
    print_response("oversized header", resp, resp_len);
    /* 400 or 413 are both acceptable depending on student design */
    int code = status_code(resp);
    if (code == 400 || code == 413) {
        printf("[PASS] oversized header -> %d\n", code);
        return 0;
    }
    printf("[FAIL] oversized header -> got %d, expected 400 or 413\n", code);
    return 1;
}

static int cmd_eleven(const char *host, int port)
{
    int fds[11];
    int fails = 0;
    for (int i = 0; i < 11; i++)
        fds[i] = -1;

    printf("Opening 11 connections to %s:%d ...\n", host, port);
    for (int i = 0; i < 11; i++) {
        fds[i] = connect_tcp(host, port);
        if (fds[i] < 0) {
            printf("[FAIL] connect #%d failed\n", i + 1);
            fails++;
            break;
        }
        printf("  connected #%d fd=%d\n", i + 1, fds[i]);
    }

    /*
     * Assignment: when all 10 slots are full, proxy accepts the 11th
     * connection, sends 503 immediately, then closes. No request required.
     */
    if (fds[10] >= 0) {
        char resp[RECV_BUF];
        size_t resp_len = 0;
        if (recv_response(fds[10], resp, sizeof(resp), &resp_len, 1) >= 0 && resp_len > 0) {
            print_response("11th client", resp, resp_len);
            fails += expect_status("11th client", resp, 503);
        } else {
            printf("[FAIL] 11th client: expected immediate 503 response\n");
            fails++;
        }
    }

    for (int i = 0; i < 11; i++) {
        if (fds[i] >= 0)
            close(fds[i]);
    }
    return fails ? 1 : 0;
}

static void *concurrent_worker(void *arg)
{
    concurrent_arg_t *a = arg;
    size_t rlen = request_len("GET", "/data", NULL, 0, 1);
    char *req = build_request("GET", "/data", NULL, NULL, 0, 1);
    char resp[RECV_BUF];
    size_t resp_len = 0;
    a->ok = 0;
    if (!req)
        return NULL;
    if (do_exchange(a->host, a->port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 1) == 0) {
        int code = status_code(resp);
        printf("[concurrent #%d] status=%d (%zu bytes)\n", a->id, code, resp_len);
        a->ok = (code == 200);
    } else {
        printf("[concurrent #%d] exchange failed\n", a->id);
    }
    free(req);
    return NULL;
}

static int cmd_concurrent(const char *host, int port, int n)
{
    if (n < 1)
        n = 1;
    if (n > 10)
        n = 10;

    pthread_t threads[10];
    concurrent_arg_t args[10];
    for (int i = 0; i < n; i++) {
        strncpy(args[i].host, host, sizeof(args[i].host) - 1);
        args[i].host[sizeof(args[i].host) - 1] = '\0';
        args[i].port = port;
        args[i].id = i + 1;
        args[i].ok = 0;
        if (pthread_create(&threads[i], NULL, concurrent_worker, &args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    int fails = 0;
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
        if (!args[i].ok)
            fails++;
    }
    if (fails == 0)
        printf("[PASS] concurrent %d clients\n", n);
    else
        printf("[FAIL] concurrent: %d/%d failed\n", fails, n);
    return fails ? 1 : 0;
}

static int cmd_raw(const char *host, int port, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror(path);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz < 0 || sz > 1024 * 1024) {
        fclose(f);
        fprintf(stderr, "raw file too large or invalid\n");
        return 1;
    }
    char *buf = malloc((size_t)sz);
    if (!buf) {
        fclose(f);
        return 1;
    }
    if (fread(buf, 1, (size_t)sz, f) != (size_t)sz) {
        fclose(f);
        free(buf);
        return 1;
    }
    fclose(f);

    char resp[RECV_BUF];
    size_t resp_len = 0;
    int rc = do_exchange(host, port, buf, (size_t)sz, resp, sizeof(resp), &resp_len, 0, NULL, 1);
    free(buf);
    if (rc < 0)
        return 1;
    print_response("raw", resp, resp_len);
    return 0;
}

static int run_scenario(const char *host, int port, const char *name)
{
    int fails = 0;
    char resp[RECV_BUF];
    size_t resp_len;
    char *req;
    size_t rlen;

    if (!strcmp(name, "all")) {
        fails += run_scenario(host, port, "get");
        fails += run_scenario(host, port, "head");
        fails += run_scenario(host, port, "post");
        fails += run_scenario(host, port, "options");
        fails += run_scenario(host, port, "keepalive");
        fails += run_scenario(host, port, "forbidden");
        fails += run_scenario(host, port, "oversized-body");
        fails += run_scenario(host, port, "cache");
        printf("\n=== scenario all: %s ===\n", fails ? "SOME FAILURES" : "ALL PASSED");
        return fails ? 1 : 0;
    }

    if (!strcmp(name, "get")) {
        rlen = request_len("GET", "/index", NULL, 0, 1);
        req = build_request("GET", "/index", NULL, NULL, 0, 1);
        if (!req || do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 1) < 0) {
            free(req);
            return 1;
        }
        free(req);
        print_response("GET /index", resp, resp_len);
        return expect_status("GET /index", resp, 200);
    }
    if (!strcmp(name, "head")) {
        rlen = request_len("HEAD", "/index", NULL, 0, 1);
        req = build_request("HEAD", "/index", NULL, NULL, 0, 1);
        if (!req || do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 0) < 0) {
            free(req);
            return 1;
        }
        free(req);
        print_response("HEAD /index", resp, resp_len);
        /* HEAD must have Content-Length but empty body */
        return expect_status("HEAD /index", resp, 200);
    }
    if (!strcmp(name, "post")) {
        const char *body = "hello allowed";
        rlen = request_len("POST", "/message", NULL, strlen(body), 1);
        req = build_request("POST", "/message", NULL, body, strlen(body), 1);
        if (!req || do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 1) < 0) {
            free(req);
            return 1;
        }
        free(req);
        print_response("POST /message", resp, resp_len);
        return expect_status("POST /message", resp, 200);
    }
    if (!strcmp(name, "options")) {
        rlen = request_len("OPTIONS", "*", NULL, 0, 1);
        req = build_request("OPTIONS", "*", NULL, NULL, 0, 1);
        if (!req || do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 0, NULL, 1) < 0) {
            free(req);
            return 1;
        }
        free(req);
        print_response("OPTIONS", resp, resp_len);
        return expect_status("OPTIONS", resp, 200);
    }
    if (!strcmp(name, "keepalive"))
        return cmd_keepalive(host, port);
    if (!strcmp(name, "forbidden"))
        return cmd_forbidden(host, port);
    if (!strcmp(name, "oversized-body"))
        return cmd_oversized_body(host, port);
    if (!strcmp(name, "cache")) {
        /* Two identical GETs — second should still be 200 (cache hit or miss both OK) */
        rlen = request_len("GET", "/data", NULL, 0, 0);
        req = build_request("GET", "/data", NULL, NULL, 0, 0);
        if (!req)
            return 1;
        int fd = -1;
        if (do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 1, &fd, 1) < 0) {
            free(req);
            return 1;
        }
        fails += expect_status("cache miss/first", resp, 200);
        if (do_exchange(host, port, req, rlen, resp, sizeof(resp), &resp_len, 1, &fd, 1) < 0) {
            free(req);
            if (fd >= 0)
                close(fd);
            return 1;
        }
        fails += expect_status("cache hit/second", resp, 200);
        free(req);
        if (fd >= 0)
            close(fd);
        printf("[INFO] Visually confirm second response came from cache if you log hits.\n");
        return fails ? 1 : 0;
    }

    fprintf(stderr, "Unknown scenario: %s\n", name);
    return 1;
}

static void usage(const char *argv0)
{
    fprintf(stderr,
            "Usage:\n"
            "  %s <host> <port> get <path>\n"
            "  %s <host> <port> head <path>\n"
            "  %s <host> <port> post <path> <body>\n"
            "  %s <host> <port> options <path>\n"
            "  %s <host> <port> close\n"
            "  %s <host> <port> keepalive\n"
            "  %s <host> <port> forbidden\n"
            "  %s <host> <port> oversized-body\n"
            "  %s <host> <port> oversized-header\n"
            "  %s <host> <port> eleven\n"
            "  %s <host> <port> concurrent [N]\n"
            "  %s <host> <port> big\n"
            "  %s <host> <port> raw <file>\n"
            "  %s <host> <port> scenario <name>\n"
            "\n"
            "Scenarios: get head post options keepalive forbidden\n"
            "           oversized-body cache all\n",
            argv0, argv0, argv0, argv0, argv0, argv0, argv0,
            argv0, argv0, argv0, argv0, argv0, argv0, argv0);
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        usage(argv[0]);
        return 1;
    }

    const char *host = argv[1];
    int port = atoi(argv[2]);
    const char *cmd = argv[3];

    if (!strcmp(cmd, "get") && argc >= 5)
        return cmd_simple(host, port, "GET", argv[4], NULL, 1);
    if (!strcmp(cmd, "head") && argc >= 5)
        return cmd_simple(host, port, "HEAD", argv[4], NULL, 1);
    if (!strcmp(cmd, "post") && argc >= 6)
        return cmd_simple(host, port, "POST", argv[4], argv[5], 1);
    if (!strcmp(cmd, "options") && argc >= 5)
        return cmd_simple(host, port, "OPTIONS", argv[4], NULL, 1);
    if (!strcmp(cmd, "close"))
        return cmd_simple(host, port, "GET", "/index", NULL, 1);
    if (!strcmp(cmd, "big"))
        return cmd_simple(host, port, "GET", "/big", NULL, 1);
    if (!strcmp(cmd, "keepalive"))
        return cmd_keepalive(host, port);
    if (!strcmp(cmd, "forbidden"))
        return cmd_forbidden(host, port);
    if (!strcmp(cmd, "oversized-body"))
        return cmd_oversized_body(host, port);
    if (!strcmp(cmd, "oversized-header"))
        return cmd_oversized_header(host, port);
    if (!strcmp(cmd, "eleven"))
        return cmd_eleven(host, port);
    if (!strcmp(cmd, "concurrent")) {
        int n = (argc >= 5) ? atoi(argv[4]) : 5;
        return cmd_concurrent(host, port, n);
    }
    if (!strcmp(cmd, "raw") && argc >= 5)
        return cmd_raw(host, port, argv[4]);
    if (!strcmp(cmd, "scenario") && argc >= 5)
        return run_scenario(host, port, argv[4]);

    usage(argv[0]);
    return 1;
}
