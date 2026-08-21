#define _GNU_SOURCE
#include "request_response.h"
#include "server_client_signal.h"
#include "config.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "origin.h"

void http_common_send_error(int client_fd, int error_code)
{
    char *response;
    if (error_code == 400)
    {
        response =
            "HTTP/1.1 400 Bad Request\n"
            "Content-Length: 0\n"
            "Connection: close\n"
            "\n";
    }
    else if(error_code == 403)
    {
        response =
            "HTTP/1.1 403 Forbidden\n"
            "Content-Length: 0\n"
            "Connection: close\n"
            "\n";
    }
    else if(error_code == 413)
    {
        response = 
            "HTTP/1.1 413 Content Too Large\n"
            "Content-Length: 0\n"
            "Connection: close\n"
            "\n";
    }
    else if(error_code == 502)
    {
        response =
            "HTTP/1.1 502 Bad Gateway\n"
            "Content-Length: 41\n"
            "Connection: keep-alive\n"
            "\n"
            "Origin server is temporarily unavailable.";
    }
    else if(error_code == 503)
    {
        response =
            "HTTP/1.1 503 Service Unavailable\n"
            "Content-Length: 49\n"
            "Connection: close\n"
            "\n"
            "Proxy is temporarily unable to accept new clients.";
    }
    else
    {
        return;
    }
    if(send( client_fd, response, strlen(response), 0) < 0)
    {
        perror("send");
        return;
    }
}