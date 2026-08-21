#include <signal.h>
#include <stdio.h>
#include "config.h"
#include "origin.h"
#include "server_client_signal.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>





int listen_fd;

int server_create_listening_socket(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PROXY_LISTEN_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if(listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen");
        close(server_fd);
        return -1;
    }
    listen_fd = server_fd;
    return listen_fd;
}

int server_accept_client(int server_fd)
{
    int client_fd = accept(server_fd, NULL,NULL);
    if(client_fd < 0)
    {
        perror("accept");
        return -1;
    }
    if(client_add(client_fd) == -1)
    {
        char response503[] = 
            "HTTP/1.1 503 Service Unavailable\n"
            "Content-Length: 49\n"
            "Connection: close\n\n"
            "Proxy is temporarily unable to accept new clients.\n";
            
        if(send(client_fd, response503, strlen(response503),0) < 0)
        {
            perror("send");
            close(client_fd);
            return -1;
        }
        close(client_fd);
    }
    
    return 0;
}








struct client clients[MAX_CLIENTS];

void client_init_all(void)
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        clients[i].fd = -1;
    }
}


int client_add(int fd)
{
    int index = 0;
    
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(clients[i].fd == -1)
        {
            clients[i].state = READING_REQUEST;
            clients[i].request_len = 0;
            clients[i].connection_close = 0;
            clients[i].origin_channel = -1;
            clients[i].response_len = 0;
            clients[i].response_buf = NULL;
            clients[i].response_sent = 0; 
            clients[i].fd = fd;
            clients[i].content_length_present = 0;
            index = i;
            return  index;
        }
    }
    return  -1;
}


void client_remove(int index)
{
    if(clients[index].fd != -1)
    {
        if(clients[index].origin_channel != -1)
        {
            channels[clients[index].origin_channel].client_id = -1;
        }
        if(clients[index].response_buf != NULL)
        {
            free(clients[index].response_buf);
        }
        close(clients[index].fd);
        clients[index].fd = -1;
        clients[index].state = READING_REQUEST;
        clients[index].request_len = 0;
        clients[index].connection_close = 0;
        clients[index].origin_channel = -1;
        clients[index].response_len = 0;
        clients[index].response_sent = 0; 
        clients[index].content_length_present = 0;
    }
}






volatile sig_atomic_t stop_requested = 0;

static void handle_sigint(int signum)
{
    stop_requested = 1;
}
void signals_setup(void)
{
    signal(SIGINT,handle_sigint);
    signal(SIGPIPE, SIG_IGN);
}