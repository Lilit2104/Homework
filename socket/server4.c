#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080
#define BUFFER_SIZE 256

void shift_lett( char *str)
{
    int len = strlen(str);

    for(int i =0; i < len; i++)
    {
        if(str[i] >= 'a' && str[i] <= 'z')
        {
            if(str[i] == 'z')
            {
                str[i] = 'a';
            }
            else
            {
                str[i] = str[i] + 1;
            }
        }
        else if(str[i] >= 'A' && str[i] <= 'Z')
        {
            if(str[i] == 'Z')
            {
                str[i] = 'A'; 
            }
            else
            {
                str[i] = str[i] + 1;
                
            }
        }
    }
}

int main()
{

    int server_fd = socket(AF_INET, SOCK_STREAM,0);

    if(server_fd <0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in server_addr;

     memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));

    listen(server_fd, 5);

    printf("Waiting client...\n");

    int client_fd = accept(server_fd, NULL,NULL);

    printf("Client is connected\n");


    char buffer[BUFFER_SIZE];

    while(1)
    {
        memset(buffer,0,BUFFER_SIZE);
        int n = recv(client_fd,buffer,BUFFER_SIZE,0);

        if(n <= 0)
            break;


         printf("\nClient sent: %s\n",buffer);

        if(strcmp(buffer,"exit") == 0)
            break;

         shift_lett(buffer);  

        send(client_fd,buffer, strlen(buffer), 0);

        printf("Server sent: %s\n",buffer);
    }

    close(server_fd);
    close(client_fd);
    return 0;
}