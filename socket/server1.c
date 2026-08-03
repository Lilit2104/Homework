#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 256


int evaluate(char *expr, double *result)
{
    double a, b;
    char op;

    int n = sscanf(expr, "%lf %c %lf", &a, &op, &b);

    if(n != 3)
        return -1;


    if(op == '+')
        *result = a + b;

    else if(op == '-')
        *result = a - b;

    else if(op == '*')
        *result = a * b;

    else if(op == '/')
    {
        if(b == 0)
            return -1;

        *result = a / b;
    }

    else if(op == '%')
    {
        long ai = (long)a;
        long bi = (long)b;

        if(bi == 0)
            return -1;

        *result = (double)(ai % bi);
    }

    else
        return -1;


    return 0;
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

    memset(&server_addr,0,sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;


    bind(server_fd,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr));


    listen(server_fd,5);


    printf("Waiting client...\n");


    int client_fd = accept(server_fd,NULL,NULL);

    printf("Client connected\n");


    char buffer[BUFFER_SIZE];

    while(1)
    {
        memset(buffer,0,BUFFER_SIZE);

        int n = recv(client_fd,buffer,BUFFER_SIZE,0);

        if(n <= 0)
            break;


        printf("\nClient: %s\n",buffer);


        if(strcmp(buffer,"exit") == 0)
            break;


        double result;
        char response[BUFFER_SIZE];

        if(evaluate(buffer,&result) == 0)
        {
            if(result == (long long)result)
                snprintf(response,BUFFER_SIZE,"Result = %lld",(long long)result);

            else
                snprintf(response,BUFFER_SIZE,"Result = %.4f",result);
        }

        else
            snprintf(response,BUFFER_SIZE,"ERROR");


        send(client_fd,response,strlen(response),0);

        printf("Server: %s\n",response);
    }


    close(client_fd);
    close(server_fd);

    return 0;
}