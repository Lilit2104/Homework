#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#define PORT 8080
#define IP_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 256


int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in  server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, IP_ADDRESS, &server_addr.sin_addr);

   if(connect(sock, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
{
    perror("connect");
    exit(1);
}

    printf("Connected to server\n");

    char buffer[BUFFER_SIZE];

    while(1)
    {
        printf("Enter text: ");
        
        fgets(buffer, BUFFER_SIZE, stdin);

        buffer[strcspn(buffer,"\n")] = 0;

        send(sock,buffer,strlen(buffer),0);

        if(strcmp(buffer, "exit") == 0)
        {
            break;
        }

        memset(buffer,0,BUFFER_SIZE);

        int n = recv(sock,buffer,BUFFER_SIZE,0);

        if(n <= 0)
            break;


        printf("Reversed: %s\n",buffer);
    }

    close(sock);
    return 0;
}