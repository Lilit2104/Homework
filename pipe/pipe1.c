#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main()
{
    int fd[2];
    pid_t pid;
    char buf[100]; 

    if (pipe(fd) == -1) 
    {
        perror("pipe");
        exit(1);
    }
 
     pid = fork();
 
     if (pid < 0) 
     {
         perror("fork");
         exit(1);
     } 

     if (pid == 0) 
     {
        
        close(fd[0]);  
        char *message = "Բարև, ես child պրոցեսն եմ!";
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        printf("[Child] Հաղորդագրությունը ուղարկված է։\n");
        sleep(5);
        exit(0);
          
 
    } 
    else
    {
        close(fd[1]);   
        wait(NULL); 
        read(fd[0], buf, sizeof(buf));
        printf("[Parent] Ստացված հաղորդագրություն: %s\n", buf); 
        close(fd[0]);
    }

    return 0;
}