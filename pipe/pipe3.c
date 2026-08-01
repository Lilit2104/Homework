#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

struct ArrayStats
{
    int sum;
    int average;
    int max;
    int min;
};


int main()
{
    int fd_child[2];
    int fd_parent[2];
    int buf[100];


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

        int arr[5];
        read(fd_parent[0], arr, sizeof(arr));
        printf("[CHILD] Հաղորդագրությունը ստացել եմ:\n");
        close(fd_parent[0]);

        struct ArrayStats stats;       
        int  max = arr[0];
        int min = arr[1];
        int average = 0;
        int sum = 0;

        for (int i = 0; i < 5; i++) 
        {
           
            if(arr[i] > max)
            {
                max = arr[i];
            }

            if(arr[i] < min)
            {
                min = arr[i];
            }

            sum += arr[i];
        }

        average = sum/5;

        stats.max = max;
        stats.min = min;
        stats.average = average;
        stats.sum = sum;

        write(fd_child[1], &stats, sizeof(stats));
        close(fd_child[1]);

        printf("[CHILD] Հաղորդագրությունը ուղարկված է։\n");
        sleep(3);
    }
    else
    {
        close(fd_parent[0]);  
        close(fd_child[1]);

        int arr[5] = {10, 20, 30, 40, 50};
        write(fd_parent[1], arr, sizeof(arr));
        close(fd_parent[1]);
       
        printf("[Parent] Հաղորդագրությունը ուղարկված է։\n");
        sleep(3);

        struct ArrayStats stats;

        read(fd_child[0], &stats, sizeof(stats));

        printf("[PARENT] Ահա վերջնական արդյունքը\n");
        printf("Sum = %d\n", stats.sum);
        printf("Average = %d\n", stats.average);
        printf("Max = %d\n", stats.max);
        printf("Min = %d\n", stats.min);

        close(fd_child[0]);
        
    }

    return 0;
}