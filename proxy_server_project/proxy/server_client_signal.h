#ifndef SIGNALS_H
#define SIGNALS_H
#include <signal.h>

void signals_setup(void);
extern volatile sig_atomic_t stop_requested;

#endif



#ifndef SERVER_H
#define SERVER_H

extern int listen_fd;

int server_create_listening_socket(void);
int server_accept_client(int server_fd);

#endif




#ifndef CLIENT_H
#define CLIENT_H

#include "config.h"
#include <unistd.h>

enum client_state {
    READING_REQUEST,
    VALIDATING_REQUEST,
    WAITING_FOR_ORIGIN,
    READING_ORIGIN_RESPONSE,
    READING_CACHE,
    SENDING_RESPONSE,
    CLOSING_CONNECTION
};

struct client
{
    int fd;
    char request_buf[MAX_HEADER_SIZE + MAX_BODY_SIZE];
    size_t request_len;
    char method[8];                        // optionը ամենաերկարն է 7 + 1 \0
    char path[MAX_HEADER_SIZE];            // ապահովության համար սա վերցնենք
    char version[16];                       // հաշվարկային 16ը չի գերազանցի
    size_t expected_body_size;
    size_t header_end_off;                  //header֊ի ավարտ սրանից հետո սկսվում է body֊ն
    int content_length_present;
    enum client_state state;
    int origin_channel;
    int connection_close;
    char *response_buf;                    
    size_t response_len;                    //պատասխանի երկարությունն է
    size_t response_sent;                   // ուղարկված բայթերի քանակ
};

extern struct client clients[MAX_CLIENTS];
void client_init_all(void);
int client_add(int fd);
void client_remove(int index);
#endif