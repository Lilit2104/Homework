#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define size 1024

typedef struct
{
    int flag;
    char mess[size];
} SharedMemory;

int main()
{
    SharedMemory *mem = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);

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
        munmap(mem, sizeof(SharedMemory));
        exit(EXIT_FAILURE);
    }


    if(pid == 0)
    {
        while( mem->flag == 0)
        {
            sleep(1);
        }

        printf(" [CHILD] RECEIVED: %s\n", mem->mess);
        
        munmap(mem, sizeof(SharedMemory));
        exit(1);
    }

    else
    {
        strcpy(mem->mess, "Hello from parent");
        printf("[PARENT] Parent writes : %s\n", mem->mess);

        mem->flag = 1;

        wait(NULL);

        munmap(mem, sizeof(SharedMemory));

    }
    return 0;
}