#ifndef CONFIG_H
#define CONFIG_H

#define MAX_CLIENTS 10
#define MAX_HEADER_SIZE 4096
#define MAX_BODY_SIZE   8192
#define MAX_RESPONSE_SIZE (1024 * 1024)
#define MAX_CACHE_ENTRIES 10
#define ORIGIN_CHANNELS 10
#define ORIGIN_FD_BASE 100
#define PROXY_LISTEN_PORT 8080
#define RECV_CHUNK_SIZE 4096
#define POLL_ARRAY_SIZE  (1 + MAX_CLIENTS + ORIGIN_CHANNELS +ORIGIN_CHANNELS)


void http_common_send_error(int client_fd, int error_code);
#endif