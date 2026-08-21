#include "protocol.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

int origin_write_all(int fd, const void *buf, size_t len)
{
    const char *p = buf;
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = write(fd, p + sent, len - sent);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        if (n == 0)
            return -1;
        sent += (size_t)n;
    }
    return 0;
}

static char *find_header_end(const char *buf, size_t len)
{
    /* Protocol uses LF only: headers end at "\n\n" */
    for (size_t i = 0; i + 1 < len; i++) {
        if (buf[i] == '\n' && buf[i + 1] == '\n')
            return (char *)(buf + i);
    }
    return NULL;
}

static long parse_content_length(const char *headers, size_t header_len)
{
    const char *p = headers;
    const char *end = headers + header_len;
    while (p < end) {
        const char *line_end = memchr(p, '\n', (size_t)(end - p));
        if (!line_end)
            break;
        size_t line_len = (size_t)(line_end - p);
        if (line_len >= 15 && strncasecmp(p, "Content-Length:", 15) == 0) {
            const char *v = p + 15;
            while (v < p + line_len && (*v == ' ' || *v == '\t'))
                v++;
            return strtol(v, NULL, 10);
        }
        p = line_end + 1;
    }
    return 0;
}

static int parse_request_line(const char *req, char *method, char *path, char *version)
{
    const char *eol = strchr(req, '\n');
    if (!eol)
        return -1;
    size_t line_len = (size_t)(eol - req);
    char line[512];
    if (line_len >= sizeof(line))
        return -1;
    memcpy(line, req, line_len);
    line[line_len] = '\0';
    return sscanf(line, "%63s %255s %63s", method, path, version) == 3 ? 0 : -1;
}

static int append_bytes(char *out, size_t out_cap, size_t *used,
                        const void *data, size_t n)
{
    if (*used + n > out_cap)
        return -1;
    memcpy(out + *used, data, n);
    *used += n;
    return 0;
}

static int format_response(char *out, size_t out_cap,
                           int status, const char *reason,
                           const char *extra_headers,
                           const char *body, size_t body_len,
                           int is_head)
{
    char header[1024];
    int n = snprintf(header, sizeof(header),
                     "HTTP/1.1 %d %s\n"
                     "Content-Length: %zu\n"
                     "Connection: keep-alive\n"
                     "%s"
                     "\n",
                     status, reason, body_len,
                     extra_headers ? extra_headers : "");
    if (n < 0 || (size_t)n >= sizeof(header))
        return -1;

    size_t used = 0;
    if (append_bytes(out, out_cap, &used, header, (size_t)n) < 0)
        return -1;
    if (!is_head && body_len > 0 && body) {
        if (append_bytes(out, out_cap, &used, body, body_len) < 0)
            return -1;
    }
    return (int)used;
}

int origin_send_response(int fd, int status, const char *reason,
                         const char *extra_headers,
                         const char *body, size_t body_len,
                         int is_head)
{
    char tmp[ORIGIN_MAX_HEADER_SIZE + ORIGIN_MAX_BODY_SIZE + 1024];
    int n = format_response(tmp, sizeof(tmp), status, reason,
                            extra_headers, body, body_len, is_head);
    if (n < 0)
        return -1;
    return origin_write_all(fd, tmp, (size_t)n);
}

int origin_handle_request_buf(const char *req, size_t req_len,
                              char *out, size_t out_cap)
{
    char method[64], path[256], version[64];
    (void)req_len;

    if (parse_request_line(req, method, path, version) < 0)
        return format_response(out, out_cap, 400, "Bad Request", NULL, NULL, 0, 0);

    int is_head = (strcmp(method, "HEAD") == 0);
    int is_get  = (strcmp(method, "GET") == 0);
    int is_post = (strcmp(method, "POST") == 0);
    int is_opt  = (strcmp(method, "OPTIONS") == 0);

    fprintf(stderr, "[origin] %s %s %s\n", method, path, version);
    if (is_opt)
        return format_response(out, out_cap, 200, "OK",
                               "Allow: GET, HEAD, POST, OPTIONS\n",
                               NULL, 0, 0);

    if (!is_get && !is_head && !is_post) {
        const char *body = "Method Not Allowed";
        return format_response(out, out_cap, 405, "Method Not Allowed",
                               "Allow: GET, HEAD, POST, OPTIONS\n",
                               body, strlen(body), 0);
    }

    if (is_post) {
        char msg[256];
        int n = snprintf(msg, sizeof(msg), "POST accepted for %s", path);
        if (n < 0)
            n = 0;
        return format_response(out, out_cap, 200, "OK", NULL, msg, (size_t)n, 0);
    }

    if (strcmp(path, "/") == 0 || strcmp(path, "/index") == 0) {
        const char *body = "Hello, client\n";
        return format_response(out, out_cap, 200, "OK", NULL, body, strlen(body), is_head);
    }
    if (strcmp(path, "/data") == 0) {
        const char *body = "sample-data-payload";
        return format_response(out, out_cap, 200, "OK", NULL, body, strlen(body), is_head);
    }
    if (strcmp(path, "/user") == 0) {
        const char *body = "{\"id\":25,\"name\":\"student\"}";
        return format_response(out, out_cap, 200, "OK",
                               "Content-Type: application/json\n",
                               body, strlen(body), is_head);
    }
    if (strcmp(path, "/big") == 0) {
        /* Keep /big modest for SHM (4 pages). Use 2 KiB here. */
        size_t sz = 2048;
        char *body = malloc(sz);
        if (!body)
            return format_response(out, out_cap, 500, "Internal Server Error",
                                   NULL, NULL, 0, 0);
        for (size_t i = 0; i < sz; i++)
            body[i] = (char)('A' + (i % 26));
        int n = format_response(out, out_cap, 200, "OK", NULL, body, sz, is_head);
        free(body);
        return n;
    }
    if (strcmp(path, "/empty") == 0)
        return format_response(out, out_cap, 200, "OK", NULL, NULL, 0, is_head);

    {
        const char *body = "Not Found";
        return format_response(out, out_cap, 404, "Not Found", NULL, body, strlen(body), is_head);
    }
}

void origin_handle_request(int out_fd, const char *req, size_t req_len)
{
    char tmp[ORIGIN_MAX_HEADER_SIZE + ORIGIN_MAX_BODY_SIZE + 1024];
    int n = origin_handle_request_buf(req, req_len, tmp, sizeof(tmp));
    if (n > 0)
        origin_write_all(out_fd, tmp, (size_t)n);
}

/*
 * Try to extract one complete request from buffer without reading.
 * Returns 1 if a complete request was handled and removed from buf,
 * 0 if need more data, -1 on error.
 */
static int try_consume_request(int out_fd, char **buf, size_t *len, size_t *cap)
{
    (void)cap;
    char *hend = find_header_end(*buf, *len);
    if (!hend)
        return 0;

    size_t header_bytes = (size_t)(hend - *buf);
    size_t sep_len = 2; /* "\n\n" */
    long cl = parse_content_length(*buf, header_bytes + sep_len);
    if (cl < 0 || cl > ORIGIN_MAX_BODY_SIZE) {
        origin_send_response(out_fd, 413, "Content Too Large", NULL, NULL, 0, 0);
        return -1;
    }

    size_t total = header_bytes + sep_len + (size_t)cl;
    if (*len < total)
        return 0;

    char saved = (*buf)[total];
    (*buf)[total] = '\0';
    origin_handle_request(out_fd, *buf, total);
    (*buf)[total] = saved;

    size_t remain = *len - total;
    memmove(*buf, *buf + total, remain);
    *len = remain;
    (*buf)[*len] = '\0';
    return 1;
}

int origin_serve_one(int in_fd, int out_fd, char **buf, size_t *len, size_t *cap)
{
    if (!*buf) {
        *cap = ORIGIN_MAX_HEADER_SIZE + ORIGIN_MAX_BODY_SIZE + 64;
        *buf = malloc(*cap);
        if (!*buf)
            return -1;
        *len = 0;
    }

    char tmp[ORIGIN_RECV_CHUNK];
    ssize_t n = read(in_fd, tmp, sizeof(tmp));
    if (n == 0)
        return 0;
    if (n < 0) {
        if (errno == EINTR)
            return 1;
        return -1;
    }

    /* >= because the NUL below is written at index *len */
    if (*len + (size_t)n >= *cap) {
        origin_send_response(out_fd, 413, "Content Too Large", NULL, NULL, 0, 0);
        return -1;
    }
    memcpy(*buf + *len, tmp, (size_t)n);
    *len += (size_t)n;
    (*buf)[*len] = '\0';

    for (;;) {
        int rc = try_consume_request(out_fd, buf, len, cap);
        if (rc < 0)
            return -1;
        if (rc == 0)
            return 1;
        if (*len == 0)
            return 1;
    }
}

void origin_serve_stream(int in_fd, int out_fd, volatile int *stop)
{
    char *buf = NULL;
    size_t len = 0, cap = 0;

    while (!stop || !*stop) {
        int rc = origin_serve_one(in_fd, out_fd, &buf, &len, &cap);
        if (rc <= 0)
            break;
    }
    free(buf);
}

int origin_read_complete_request(int in_fd, char **buf, size_t *len, size_t *cap)
{
    if (!*buf) {
        *cap = ORIGIN_MAX_HEADER_SIZE + ORIGIN_MAX_BODY_SIZE + 64;
        *buf = malloc(*cap);
        if (!*buf)
            return -1;
        *len = 0;
    }

    for (;;) {
        char *hend = find_header_end(*buf, *len);
        if (hend) {
            size_t header_bytes = (size_t)(hend - *buf);
            size_t sep_len = 2; /* "\n\n" */
            long cl = parse_content_length(*buf, header_bytes + sep_len);
            if (cl < 0 || cl > ORIGIN_MAX_BODY_SIZE)
                return -1;
            size_t total = header_bytes + sep_len + (size_t)cl;
            if (*len >= total)
                return (int)total;
        }

        char tmp[ORIGIN_RECV_CHUNK];
        ssize_t n = read(in_fd, tmp, sizeof(tmp));
        if (n == 0)
            return (*len > 0) ? -1 : 0;
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        /* >= because the NUL below is written at index *len */
        if (*len + (size_t)n >= *cap)
            return -1;
        memcpy(*buf + *len, tmp, (size_t)n);
        *len += (size_t)n;
        (*buf)[*len] = '\0';
    }
}
