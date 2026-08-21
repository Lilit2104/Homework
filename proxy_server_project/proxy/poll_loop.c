#define _GNU_SOURCE
#include "server_client_signal.h"
#include "config.h"
#include "request_response.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include <errno.h>
#include "origin.h"
#include "cache.h"



void run_event_loop(void)
{
    while(!stop_requested)
    {
        struct  pollfd fds[POLL_ARRAY_SIZE];
        fds[0].fd = listen_fd;
        fds[0].events = POLLIN;
        
        for(int i = 0; i  < MAX_CLIENTS; i++ )
        {
            fds[1 + i].fd = clients[i].fd;
            fds[1 + i].events = 0;
            if(clients[i].fd == -1)
            {
            
            }
            else
            {
                if(clients[i].state == READING_REQUEST || clients[i].state == VALIDATING_REQUEST)
                {
                    fds[1 +i].events = POLLIN;
                }
                else if(clients[i].state == SENDING_RESPONSE)
                {
                    fds[1 +i].events = POLLOUT;
                }
            }
        }

       
        for(int j = 0; j < ORIGIN_CHANNELS; j++ )
        {
            fds[1 + MAX_CLIENTS +j].fd = channels[j].write_fd;
            fds[1 + MAX_CLIENTS + j].events = 0;
            if(channels[j].state == ORIGIN_ASSIGNED)
            {
                fds[1 + MAX_CLIENTS + j].events = POLLOUT;
            }
                
                
        }
            
         for(int j = 0; j < ORIGIN_CHANNELS; j++)
        {
            fds[1 + MAX_CLIENTS + ORIGIN_CHANNELS +j].fd = channels[j].read_fd;          
            fds[1 + MAX_CLIENTS + ORIGIN_CHANNELS + j].events = 0; 
            if(channels[j].state == ORIGIN_WAITING_FOR_ORIGIN)
            {
                fds[1 + MAX_CLIENTS + ORIGIN_CHANNELS +j].events = POLLIN;
            }
    
        }
        int n = poll(fds, POLL_ARRAY_SIZE, -1);
        if(n < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("poll");
                continue;
            }
        }

        if(fds[0].revents & POLLIN)
        {
            server_accept_client(listen_fd);
        }
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
            if(clients[i].fd != -1)
            {
                if(fds[i + 1].revents & POLLIN)
                {
                    int res = request_reader_handle(&clients[i]);
                    if(res == -1 || res == 0)
                    {
                        client_remove(i);
                    }
                    else if(res == 1)
                    {
                        continue;
                    }
                    else if(res == 2)
                    {
                        int res_parse = request_parser_parse(&clients[i]);
                        if(res_parse != 0)
                        {
                            http_common_send_error(clients[i].fd, 400 );
                            client_remove(i);
                        }
                        else if(res_parse == 0)
                        {
                            int res_valid = request_validator_validate(&clients[i]);
                            if(res_valid != 0)
                            {
                                http_common_send_error(clients[i].fd, res_valid );
                                client_remove(i);
                            }
                            else if(res_valid == 0)
                            {
                                int res_filter = request_filter_check(&clients[i]);
                                if(res_filter != 0 )
                                {
                                    http_common_send_error(clients[i].fd, res_filter);
                                    client_remove(i);
                                }
                                else
                                {
                                    char req_line[MAX_HEADER_SIZE * 3];
                                    snprintf(req_line, sizeof(req_line), "%s %s %s", clients[i].method ,clients[i].path , clients[i].version);
                                    char *filename =  cache_lookup(req_line);
                                    if(filename != NULL)
                                    {         
                                        size_t  loaded_len;                              
                                        clients[i].response_buf = malloc(MAX_RESPONSE_SIZE);
                                       
                                        if(clients[i].response_buf == NULL)
                                        {
                                            http_common_send_error(clients[i].fd, 502);
                                            client_remove(i);
                                            continue;
                                        }
                                        else
                                        {
                                           int n = cache_load(filename, clients[i].response_buf, &loaded_len);
                                           if(n < 0)
                                           {

                                                free(clients[i].response_buf);
                                                clients[i].response_buf = NULL;
                                                http_common_send_error(clients[i].fd, 502);
                                                client_remove(i);
                                                continue;
                                           }
                                           else
                                           {
                                                clients[i].state = SENDING_RESPONSE;
                                                clients[i].response_len = loaded_len;
                                           }
                                        }
                                    }
                                    else
                                    {
                                       int res_origin =  origin_assign_channel(i ,clients[i].request_buf, clients[i].request_len);
                                       if(res_origin >= 0)
                                       {
                                        clients[i].state = WAITING_FOR_ORIGIN;
                                        clients[i].origin_channel = res_origin;
                                       }
                                       else
                                       {
                                            http_common_send_error(clients[i].fd, 502);
                                       }
                                    }
                                }
                            }
                        }
                    }

                }
                else if(fds[i + 1].revents & POLLOUT)
                {
                   int result = response_writer_send(&clients[i]);
                   if(result == -1)
                   {
                        client_remove(i);
                   }
                   else if(result == 1)
                   {
                        continue;
                   }
                   else if(result == 2)
                   {
                        if(clients[i].connection_close == 1)
                        {
                            client_remove(i);
                        }
                        else
                        {
                            clients[i].state = READING_REQUEST;
                            clients[i].request_len = 0;
                            clients[i].connection_close = 0;
                            clients[i].origin_channel = -1;
                            clients[i].response_len = 0;
                            free(clients[i].response_buf);
                            clients[i].response_buf = NULL;
                            clients[i].response_sent = 0;             
                            clients[i].content_length_present = 0;
                        }
                   }
                }
            }
        }

        for(int i = 0; i < ORIGIN_CHANNELS; i++)
        {
                if(fds[1 + MAX_CLIENTS + i ].revents & POLLOUT)
                {
                    origin_handle_writable(i);
                }
            
        }
        for(int i = 0; i < ORIGIN_CHANNELS; i++)
        {
            if(fds[1 + MAX_CLIENTS + ORIGIN_CHANNELS + i].revents & POLLIN)
            {
                origin_handle_readable(i);
            }
        }
       
        for(int i = 0; i < ORIGIN_CHANNELS; i++)
        {
            if(channels[i].state == ORIGIN_RESPONSE_READY)
            {
                if((strcmp(clients[channels[i].client_id].method, "GET") == 0 || strcmp(clients[channels[i].client_id].method, "HEAD") == 0) && origin_response_status(i) == 200)
                {
                    char resp_line[MAX_HEADER_SIZE * 3];
                    snprintf(resp_line, sizeof(resp_line), "%s %s %s", clients[channels[i].client_id].method ,clients[channels[i].client_id].path , clients[channels[i].client_id].version);
                    cache_insert(resp_line, channels[i].response_buf,channels[i].response_len);
                }
                    origin_release_channel(i);
                
            }
                   
        }
    }

    close(listen_fd);
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        if(clients[i].fd != -1)
        {
            client_remove(i);
        }
    }
    origin_shutdown();

}



