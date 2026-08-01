#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main() 
{
    int fd_child[2];
    int fd_parent[2];
    int buf[100];
    int buff1[100];

    if (pipe(fd_child) == -1 || pipe(fd_parent) == -1) 
    {
        perror("pipe");
        exit(1);
     }

    pid_t pid = fork(); 

    if (pid < 0) 
    {
        perror("fork");
        exit(1);
    } 


    if(pid == 0)
    {
        close(fd_parent[1]);
        close(fd_child[0]);

        read(fd_parent[0], buf, sizeof(buf));
        printf("[CHILD] Ստացված հաղորդագրություն: %s\n", buf); 
        close(fd_parent[0]);
        sleep(3);
        char *mess = "Thanks";
        write(fd_child[1], mess, strlen(mess) + 1);
        close(fd_child[1]);
        printf("[CHILD] Հաղորդագրությունը ուղարկված է։\n");

        wait(NULL);

    }
    else
    { 
        close(fd_parent[0]);  
        close(fd_child[1]);

        char *message = "Hello";
        write(fd_parent[1], message, strlen(message) + 1);
        close(fd_parent[1]);
       
        printf("[Parent] Հաղորդագրությունը ուղարկված է։\n");
        sleep(3);

        read(fd_child[0], buff1, sizeof(buf));
        printf("[PARENT] Ստացված հաղորդագրություն: %s\n", buff1);
        close(fd_child[0]);


        wait(NULL);

    }
}    
