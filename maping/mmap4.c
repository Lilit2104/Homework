#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>


#define MAX_COMMANDS 10

typedef struct  
{
    int flag;
    char command[MAX_COMMANDS][50];
    char results[MAX_COMMANDS][50];
    int command_count;
}SharedData;


void cleanup(void)
{
    printf("[CLEANUP] Process is exiting...\n");
}


int main()
{
    atexit(cleanup); 

    SharedData *mem = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    if(mem == MAP_FAILED)
    {
        printf("Maping failed");
        exit(EXIT_FAILURE);
    }

    mem->flag = 0;
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("fork");
        munmap(mem, sizeof(SharedData));
        exit(EXIT_FAILURE);
    }


    if(pid == 0)
    {
        while( mem->flag == 0)
        {
            sleep(1);
        }
       
        char op[15];
        int a,b;
        int res;
        
        for(int i = 0; i < mem->command_count; i++)
        {
            sscanf(mem->command[i], "%s %d %d", op, &a, &b);
        
            if (strcmp(op, "ADD") == 0)
            {
                res = a + b;
            }

            else if (strcmp(op, "MUL") == 0)
            {
                res = a * b;
            }

            else if (strcmp(op, "SUB") == 0)
            {
                res = a - b;
            }
            else
            {
                sprintf(mem->results[i], "Invalid command: %s", op);
                continue;
            }

            sprintf(mem->results[i], "%s %d %d = %d", op, a, b, res);
        }
        
        munmap(mem, sizeof(SharedData));
        exit(0);
    }

    else
    {
        strcpy(mem->command[0], "ADD 10 20");
        strcpy(mem->command[1], "MUL 5 6");
        strcpy(mem->command[2], "SUB 23 5");

        printf("[PARENT] Data is ready\n");

        mem->command_count = 3;
        mem->flag = 1;

        wait(NULL);
        
        printf("\n[PARENT] Final Results:\n");
        
        for (int i = 0; i < mem->command_count; i++)
        {
            printf("%s\n", mem->results[i]);
        }

        munmap(mem, sizeof(SharedData));
    }
    return 0;
}    