#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


struct Temperature
{
    float celsius;
};

struct ConvertedTemperature
{
    float fahrenheit;
};

int main()
{

    int fd_parent[2];
    int fd_child[2];
    
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

        struct Temperature  cels;
        struct ConvertedTemperature fahren;

        read(fd_parent[0], &cels, sizeof(cels));
        printf("[CHILD] Հաղորդագրությունը ստացել եմ:\n");
        sleep(3);
        close(fd_parent[0]);

        fahren.fahrenheit = cels.celsius * 1.8 + 32;

        write(fd_child[1], &fahren, sizeof(cels));
        close(fd_child[1]);
        
        printf("[CHILD] Հաղորդագրությունը ուղարկված է։\n");
        sleep(3);
    }

    else
    {
        close(fd_parent[0]);
        close(fd_child[1]);

        struct Temperature  cels;
        struct ConvertedTemperature fahren;

        printf("[PARENT] Մուտքագրեք ջերմաստիճանը Ցելսիուսով : ");
        scanf("%f", &cels.celsius);

        write(fd_parent[1], &cels, sizeof(cels));
        close(fd_parent[1]);

       

        read(fd_child[0], &fahren, sizeof(cels));
        sleep(3);
        printf("[PARENT] Ահա վերջնական արդյունքը\n");
        printf("[PARENT] Ջերմաստիճանը Ֆարենհայտով՝ %.2f°F\n", fahren.fahrenheit);
        wait(NULL);

    }
    return 0;
}