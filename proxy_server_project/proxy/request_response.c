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
#include <errno.h>






int request_validator_validate(struct client *c)
{
    if(strlen(c->method) == 0 )
    {
        return 400;
    }
    if(strcmp(c->method, "POST") == 0 && c->content_length_present == 0)
    {
        return 400;
    }
    if(c->expected_body_size > MAX_BODY_SIZE)
    {
        return 413;
    }
    if(c->header_end_off > MAX_HEADER_SIZE)
    {
        return 413;
    }
    return 0;
}






int request_reader_handle(struct client *c)
{
    int n = recv(c->fd,c->request_buf + c->request_len,((MAX_HEADER_SIZE + MAX_BODY_SIZE) - c->request_len),0);
    if(n < 0)
    {
        perror("recv");
        return -1;    // recv չեղավ
    }
    else if(n == 0)
    {
        return 0;     //  client  disconnect
    }
    else
    {
        c->request_len += n;
        char *header = memmem(c->request_buf, c->request_len, "\n\n", strlen("\n\n"));
        if(header == NULL)
        {
            c->header_end_off = 0;
        }
        else
        {
            c->header_end_off = header - c->request_buf;

            char *cont = memmem(c->request_buf,c->header_end_off, "Content-Length:", strlen("Content-Length:"));
            if(cont == NULL)
            {
                c->expected_body_size = 0; 
                c->content_length_present = 0;
                return 2;
            }
            else
            {   
                c->content_length_present = 1;
                cont += strlen("Content-Length:");
                while (*cont == ' ')
                {
                    cont++;
                }
                sscanf(cont, "%zu", &c->expected_body_size);

                size_t  body_start =  c->header_end_off + 2;
                size_t  body_received = c->request_len - body_start; 
                if(body_received < c->expected_body_size)
                {
                    return 1;  //թերի է հարցումը
                }
                else
                {
                    return 2; //requestը պատրաստ է parse անելու համար
                }
                
            }
        }
        return 1;
    }
}






int request_parser_parse(struct client *c)
{
    char *line_end = memmem(c->request_buf, c->header_end_off, "\n", strlen("\n"));
    if(line_end == NULL)
    {
        return 1;
    }

    char *meth = memmem(c->request_buf, (line_end - c->request_buf), " ", strlen(" "));
    if(meth == NULL || (size_t)(meth - c->request_buf) >= sizeof(c->method))
    {
        return 1;
    }
    strncpy(c->method, c->request_buf, (meth - c->request_buf));
    c->method[meth - c->request_buf] = '\0';

    char *path = memmem(meth + 1, (line_end - meth), " ", strlen(" "));
    if(path == NULL || (size_t)(path - (meth + 1)) >= sizeof(c->path))
    {
        return 1;
    }
    strncpy(c->path, meth + 1, (path - (meth + 1)));
    c->path[path - (meth + 1)] = '\0';

    char *vers = memmem(path + 1, (line_end - path), "\n", strlen("\n"));
    if(vers == NULL || (size_t)(vers - (path + 1)) >= sizeof(c->version))
    {
        return 1;
    }
    strncpy(c->version, path + 1, (vers - (path + 1)));
    c->version[vers - (path + 1)] = '\0';

    char *connect = memmem(c->request_buf, c->header_end_off, "Connection: close", strlen("Connection: close"));
    if(connect == NULL)
    {
        c->connection_close = 0;
    }
    else
    {
        c->connection_close = 1;
    }
    return 0;
}






int request_filter_check(struct client *c)
{
    const char *blocked_keywords[] = {
    "forbidden",
    "blocked",
    "restricted"
    };
    if(strcmp(c->method, "POST") != 0)
    {
        return 0;
    }
    else
    {
        for(int i = 0; i < (sizeof(blocked_keywords)/sizeof(blocked_keywords[0])); i++)
        {
            if(memmem((c->request_buf + c->header_end_off + 2),c->expected_body_size, blocked_keywords[i], strlen(blocked_keywords[i])) != NULL)
            {
                return 403;
            }
        }
    }
    return 0;
}







int response_writer_send(struct client *c)
{
    int n = send(c->fd, c->response_buf + c->response_sent, c->response_len - c->response_sent, 0);   
    if(n < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 1;  
        }
        perror("send");
        return -1;
    }
    else
    {
        c->response_sent += n;
        if(c->response_sent == c->response_len)
        {
            return 2;        //հաջողության դեպք
        }
        else
        {
            return 1;       //կիսատ է
        }
        

    }
}