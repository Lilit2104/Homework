#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080
#define BUFFER_SIZE 256

void reverse_words(char *str)
{
    char *words[BUFFER_SIZE];  
    int count = 0;

    char *token = strtok(str, " ");

    while(token != NULL)
    {
        words[count] = token;
        count++;
        token = strtok(NULL, " ");
    }


    char result[BUFFER_SIZE];
    result[0] = '\0';           
    for(int i = count - 1; i >= 0; i--)
    {
        strcat(result, words[i]);

        if(i != 0)
            strcat(result, " ");
    }


    strcpy(str, result);
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
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

        reverse_words(buffer);

        send(client_fd,buffer, strlen(buffer), 0);

        printf("Server sent: %s\n",buffer);
    }

    close(server_fd);
    close(client_fd);
    return 0;
}