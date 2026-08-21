#define _GNU_SOURCE
#include "server_client_signal.h"
#include "config.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "origin.h"

struct origin channels[ORIGIN_CHANNELS];
pid_t origin_pid;

void origin_spawn(void)
{
    int to_origin[ORIGIN_CHANNELS][2];
    int from_origin[ORIGIN_CHANNELS][2];

    for(int i = 0; i  < ORIGIN_CHANNELS; i++)
    {
        if(pipe(to_origin[i]) == -1 )
        {
            perror("pipe");
            exit(1);
        }
        
        if(pipe(from_origin[i]) ==  -1)
        {
            perror("pipe");
            exit(1);
        }
    }

    pid_t pid = fork();

    if(pid == -1)
    {
        perror("fork");
        exit(1);
    }
   else if(pid == 0)                       //child
    {
        for(int i = 0; i < ORIGIN_CHANNELS; i++)
        {
            dup2(to_origin[i][0],ORIGIN_FD_BASE + 2*i);
            dup2(from_origin[i][1],ORIGIN_FD_BASE + 2*i +1);

            close(to_origin[i][1]);
            close(from_origin[i][0]);

            close(to_origin[i][0]);
            close(from_origin[i][1]);
            
        }
        char base_fd_str[16];
        snprintf(base_fd_str, sizeof(base_fd_str), "%d", ORIGIN_FD_BASE);
        execl("./origin_pipes","origin_pipes","--base-fd", base_fd_str, NULL);
        perror("execl");
        exit(1);
    }
    else                                     //parent
    {
        for(int i = 0; i < ORIGIN_CHANNELS; i++)
        {
            close(to_origin[i][0]);
            close(from_origin[i][1]);
            channels[i].write_fd = to_origin[i][1];
            channels[i].read_fd = from_origin[i][0];
            channels[i].state  = ORIGIN_FREE;
            channels[i].client_id = -1;
        }
        origin_pid = pid;
    }   
}

int origin_assign_channel(int client_id, const char *request_data, size_t request_len)
{
    int index = 0;
    for(int i =0; i < ORIGIN_CHANNELS; i++)
    {
        if(channels[i].client_id == -1)
        {
            memcpy(channels[i].request_buf,request_data, request_len);
            channels[i].request_len = request_len;
            channels[i].state = ORIGIN_ASSIGNED;
            channels[i].client_id = client_id;
            channels[i].request_sent = 0;
            index = i;
            return index;
        }
    }   
    return -1;
}

void origin_handle_writable(int channel_index)
{
    int  n = write(channels[channel_index].write_fd, channels[channel_index].request_buf + channels[channel_index].request_sent,channels[channel_index].request_len - channels[channel_index].request_sent);
    
    if(n < 0)
    {
        perror("write");
        int cid = channels[channel_index].client_id;
        int wanted_close = clients[cid].connection_close;

        http_common_send_error(clients[cid].fd, 502);
        channels[channel_index].state = ORIGIN_FREE;
        channels[channel_index].client_id = -1;

        if(wanted_close)
        {
            client_remove(cid);
        }
        else
        {
            clients[cid].state = READING_REQUEST;
            clients[cid].request_len = 0;
            clients[cid].origin_channel = -1;
            clients[cid].response_len = 0;
            free(clients[cid].response_buf);
            clients[cid].response_buf = NULL;
            clients[cid].response_sent = 0;
            clients[cid].content_length_present = 0;
        }
    }
    else
    {
        channels[channel_index].request_sent += n;
        if(channels[channel_index].request_sent == channels[channel_index].request_len)
        {
            channels[channel_index].state = ORIGIN_WAITING_FOR_ORIGIN;
        }
    }
        
}

void origin_handle_readable(int channel_index)
{
    int n = read(channels[channel_index].read_fd,channels[channel_index].response_buf + channels[channel_index].response_len,( MAX_RESPONSE_SIZE - channels[channel_index].response_len));
    if(n < 0)
    {
        perror("read");
        int cid = channels[channel_index].client_id;
        int wanted_close = clients[cid].connection_close;

        http_common_send_error(clients[cid].fd, 502);
        channels[channel_index].state = ORIGIN_FREE;
        channels[channel_index].client_id = -1;

        if(wanted_close)
        {
            client_remove(cid); 
        }
        else
        {
            clients[cid].state = READING_REQUEST;
            clients[cid].request_len = 0;
            clients[cid].origin_channel = -1;
            clients[cid].response_len = 0;
            free(clients[cid].response_buf);
            clients[cid].response_buf = NULL;
            clients[cid].response_sent = 0;
            clients[cid].content_length_present = 0;
        }
        return;
    }
    else if(n == 0)
    {
        int cid = channels[channel_index].client_id;
        int wanted_close = clients[cid].connection_close;

        http_common_send_error(clients[cid].fd, 502);
        channels[channel_index].state = ORIGIN_FREE;
        channels[channel_index].client_id = -1;

        if(wanted_close)
        {
            client_remove(cid);
        }
        else
        {
            clients[cid].state = READING_REQUEST;
            clients[cid].request_len = 0;
            clients[cid].origin_channel = -1;
            clients[cid].response_len = 0;
            free(clients[cid].response_buf);
            clients[cid].response_buf = NULL;
            clients[cid].response_sent = 0;
            clients[cid].content_length_present = 0;
        }
        return;
    }
    else
    {
        channels[channel_index].response_len += n;
         char *header_response = memmem(channels[channel_index].response_buf, channels[channel_index].response_len, "\n\n", strlen("\n\n"));
        if(header_response == NULL)
        {
            return;
        }
        else
        {
            channels[channel_index].response_header_end_off = header_response - channels[channel_index].response_buf;
            char *cont_response = memmem(channels[channel_index].response_buf,channels[channel_index].response_header_end_off, "Content-Length:", strlen("Content-Length:"));
            if(cont_response == NULL)
            {
                channels[channel_index].response_expected_len = 0;  
                channels[channel_index].state = ORIGIN_RESPONSE_READY;              
            }
            else
            {
                cont_response += strlen("Content-Length:");
                while (*cont_response == ' ')
                {
                    cont_response++;
                }
                sscanf(cont_response, "%zu", &channels[channel_index].response_expected_len);

                size_t  response_body_start =  channels[channel_index].response_header_end_off + 2;
                size_t  response_body_received = channels[channel_index].response_len - response_body_start; 
                if(response_body_received < channels[channel_index].response_expected_len)
                {
                    return;  //թերի է հարցումը
                }
                else
                {
                    channels[channel_index].state = ORIGIN_RESPONSE_READY;
                }
                
            }

        }
    } 
}

void origin_release_channel(int channel_index)
{
    if(channels[channel_index].client_id != -1)
    {
        int cid = channels[channel_index].client_id;

        clients[cid].response_buf = malloc(channels[channel_index].response_len);
        if(clients[cid].response_buf == NULL)
        {
            channels[channel_index].state = ORIGIN_FREE;
            channels[channel_index].client_id = -1;
            channels[channel_index].request_sent = 0;
            channels[channel_index].response_len = 0;
            return;
        }

        memcpy(clients[cid].response_buf, channels[channel_index].response_buf, channels[channel_index].response_len);
        clients[cid].response_len = channels[channel_index].response_len;
        clients[cid].response_sent = 0;
        clients[cid].state = SENDING_RESPONSE;

        channels[channel_index].state = ORIGIN_FREE;
        channels[channel_index].client_id = -1;
        channels[channel_index].request_sent = 0;
        channels[channel_index].response_len = 0;
    }
}

void origin_shutdown(void)
{
    for(int i =0; i < ORIGIN_CHANNELS; i++)
    {
        close(channels[i].read_fd);
        close(channels[i].write_fd);
    }
    kill(origin_pid, SIGTERM);
    int status;
    waitpid(origin_pid, &status, 0);
}


int origin_response_status(int channel_index)
{
    int status_code = -1;
   
        char *p = memmem(channels[channel_index].response_buf, channels[channel_index].response_len, "HTTP/1.1", strlen("HTTP/1.1"));
        if(p != NULL)
        {
            p += strlen("HTTP/1.1");
            sscanf(p, "%d", &status_code);
        }
    
    return status_code;
}