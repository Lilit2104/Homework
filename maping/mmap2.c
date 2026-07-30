#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>


typedef struct  
{
    int flag;
    int size;
    int numbers[100];
    int sum;
    int max;
}SharedData;



int main()
{
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

        int sum = 0;
        int maximum = mem->numbers[0];

        for(int i = 0; i < mem->size; i++)
        {
            sum += mem->numbers[i];

            if(mem->numbers[i] > maximum)
            {
                maximum = mem->numbers[i];
            } 
        }

        mem->sum = sum;
        mem->max = maximum;

        printf("[CHILD] I'm done ._. \n");
       
        munmap(mem, sizeof(SharedData));
        exit(1);
    }

    else
    {
        printf("Parent please enter the size and numbers: \n");
        scanf("%d",&mem->size);

        if (mem->size <= 0 || mem->size > 100 )
        {
            printf("Invalid size! Must be between 1 and 100");
            munmap(mem, sizeof(SharedData));
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < mem->size; i++)
        {
            scanf("%d",&mem->numbers[i]);
        }

        printf("[PARENT] Data is ready\n");

        mem->flag = 1;
        
        wait(NULL);
        
        printf("[PARENT] FINAL RESULT: SUM = %d  MAXIMUM =  %d\n", mem->sum, mem->max);

        munmap(mem, sizeof(SharedData));
    }
    return 0;
}    