#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

struct TextStats
{
    int letters;
    int digits;
    int spaces;
    int lines;
};


int main()
{
    int fd_parent[2];
    int fd_child[2];
    char buff[100];

    if(pipe(fd_parent) == -1 || pipe(fd_child) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if(pid == 0)
    {
        close(fd_parent[1]);
        close(fd_child[0]);
        read(fd_parent[0], buff, sizeof(buff));
        printf("[CHILD] Հաղորդագրությունը ստացել եմ:\n");
        sleep(3);
        close(fd_parent[0]);

        struct TextStats text = {0};

        if (buff[0] != '\0')
        {
             text.lines = 1;
        }

        for(int i = 0; buff[i] != '\0'; i++)
        {
            if ((buff[i] >= 'A' && buff[i] <= 'Z') ||(buff[i] >= 'a' && buff[i] <= 'z'))
            {
                text.letters++;
            }

            if((buff[i] >= '0' && (buff[i]) <= '9'))
            {
                text.digits++;
            }
            
            if(buff[i] == ' ')
            {
                text.spaces++;
            }

            if(buff[i] == '\n')
            {
                text.lines++;
            }
        }

        write(fd_child[1], &text, sizeof(text));
        close(fd_child[1]);

        
        printf("[CHILD] Հաղորդագրությունը ուղարկված է։\n");
        sleep(3);

    }
    else
    {
        close(fd_parent[0]);
        close(fd_child[1]);

        char message[100];
        fgets(message, sizeof(message), stdin);


        write(fd_parent[1], message, strlen(message) + 1);
        close(fd_parent[1]);

        sleep(3);
        printf("[PARENT] Հաղորդագրությունը ուղարկված է։\n");

        wait(NULL);

        struct TextStats text;
        
        read(fd_child[0], &text, sizeof(text));
        sleep(2);

        printf("[PARENT] Ահա վերջնական արդյունքը\n");
        printf("Letters equal to: %d\n", text.letters);
        printf("Digits equal to: %d\n", text.digits);
        printf("Spaces equal to: %d\n", text.spaces);
        printf("Lines equal to: %d\n", text.lines);

        close(fd_child[0]);
    }
    return 0; 

}