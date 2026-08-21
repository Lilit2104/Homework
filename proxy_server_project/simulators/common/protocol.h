/*
 * Shared simplified HTTP-like protocol helpers for origin simulators.
 */

#ifndef ORIGIN_PROTOCOL_H
#define ORIGIN_PROTOCOL_H

#include <stddef.h>

#define ORIGIN_MAX_HEADER_SIZE 4096
#define ORIGIN_MAX_BODY_SIZE   8192
#define ORIGIN_RECV_CHUNK      1024

#define ORIGIN_CHANNELS        10

int origin_write_all(int fd, const void *buf, size_t len);

int origin_send_response(int fd, int status, const char *reason,
                         const char *extra_headers,
                         const char *body, size_t body_len,
                         int is_head);

/* Build response into memory. Returns bytes written, or -1 on error. */
int origin_handle_request_buf(const char *req, size_t req_len,
                              char *out, size_t out_cap);

void origin_handle_request(int out_fd, const char *req, size_t req_len);

int origin_serve_one(int in_fd, int out_fd, char **buf, size_t *len, size_t *cap);
void origin_serve_stream(int in_fd, int out_fd, volatile int *stop);

/* Read one complete framed request from a byte stream (pipe/FIFO). */
int origin_read_complete_request(int in_fd, char **buf, size_t *len, size_t *cap);

#endif
