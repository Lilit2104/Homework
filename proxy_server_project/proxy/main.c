#include "server_client_signal.h"
#include "config.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "origin.h"
#include "poll_loop.h"

int main()
{
    signals_setup();
    client_init_all();
    server_create_listening_socket();
    if(listen_fd == -1)
    {
        return 1;
    }
    printf("Server started. Listening on port ...\n");
    origin_spawn();
    run_event_loop();

    return 0;
}