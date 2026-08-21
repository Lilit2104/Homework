#ifndef ORIGIN_H
#define ORIGIN_H

#include "config.h"
#include "server_client_signal.h"
#include <unistd.h>


enum origin_state
{
    ORIGIN_FREE,
    ORIGIN_ASSIGNED,
    ORIGIN_WAITING_FOR_ORIGIN,
    ORIGIN_RESPONSE_READY
};

struct origin
{
   int  write_fd;                                                           //pipe-ի ծայրը, որով proxy-ն գրում է origin-ին
   int read_fd;                                                             //pipe-ի ծայրը, որով proxy-ն կարդում է origin-ից
   enum origin_state state;                                                 //FREE / ASSIGNED / WAITING_FOR_ORIGIN / RESPONSE_READY
   int client_id;                                                           //ասոցացված client-ի index (-1, եթե ոչ մեկի)
   char request_buf[MAX_BODY_SIZE + MAX_HEADER_SIZE];                       //request-ի copy, որ գրվելու է origin-ին (partial write-ի համար)
   size_t  request_sent;                                                    //քանի բայթ արդեն գրվել է
   char response_buf[MAX_RESPONSE_SIZE];                                    //origin-ից եկող response-ի կուտակման buffer
   size_t response_len;                                                     //քանի բայթ արդեն ստացվել է
   size_t response_header_end_off;        
   size_t response_expected_len;
   size_t request_len;                                           
};

extern struct origin channels[ORIGIN_CHANNELS];
void origin_spawn(void);
int origin_assign_channel(int client_id, const char *request_data, size_t request_len);
void origin_handle_writable(int channel_index);
void origin_handle_readable(int channel_index);
void origin_release_channel(int channel_index);
void origin_shutdown(void);
int origin_response_status(int channel_index);


#endif