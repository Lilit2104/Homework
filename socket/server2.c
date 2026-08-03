#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8081
#define BUFFER_SIZE 256
#define SECRET_PASSWORD "PAsSWoRd123"

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in  serever_addr;

    memset(&serever_addr, 0, sizeof(serever_addr));

    serever_addr.sin_family = AF_INET;
    serever_addr.sin_port = htons(PORT);
    serever_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd,(struct sockaddr*)&serever_addr, sizeof(serever_addr));

    listen(server_fd, 5);

    printf("Waiting client...\n");


    int client_fd = accept(server_fd,NULL,NULL);

    printf("Client connected\n");

    char buffer[BUFFER_SIZE];

    while(1)
    {
        memset(buffer,0, BUFFER_SIZE);

        int n = recv(client_fd,buffer,BUFFER_SIZE,0);

        if(n <= 0)
            break;


        printf("\nClient sent: %s\n",buffer);

        if(strcmp(buffer,"exit") == 0)
        {
            printf("Client requested exit.\n");
            break;
        }


        if(strcmp(buffer, SECRET_PASSWORD) == 0)
        {
            send(client_fd, "ACCESS GRANTED", strlen("ACCESS GRANTED"), 0);
            printf("Server sent: ACCESS GRANTED\n");
        }
        else
        {
            send(client_fd, "ACCESS DENIED", strlen("ACCESS DENIED"),0);
            printf("Server sent: ACCESS DENIED\n");
        }
    }

    close(client_fd);
    close(server_fd);

    
    return 0;
}