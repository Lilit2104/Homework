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
    int finsum;
    int partsum1;
    int partsum2;
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
        

        for(int i = 0; i < mem->size/2; i++)
        {
            sum += mem->numbers[i]; 
        }

        mem->partsum1 = sum;
       
        printf("[CHILD1] I'm done  half of sum = %d\n", mem->finsum);
        
        munmap(mem, sizeof(SharedData));
        exit(1);
    }

    pid_t pid1 = fork();

    if(pid1 < 0)
    {
        perror("fork");
        munmap(mem, sizeof(SharedData));
        exit(EXIT_FAILURE);
    }

    if(pid1 == 0)
    {
        while( mem->flag == 0)
        {
            sleep(1);
        }
        int sum = 0;
        

        for(int i = mem->size/2; i < mem->size; i++)
        {
            sum += mem->numbers[i]; 
        }

        mem->partsum2 = sum;
       
        printf("[CHILD2] I'm done  my part of sum =%d\n", sum);

        munmap(mem, sizeof(SharedData));
        exit(1);
    }

    
    
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
    wait(NULL);

    mem->finsum = mem->partsum1 + mem->partsum2;
    printf("[PARENT] FINAL RESULT: SUM = %d\n",  mem->finsum);

    munmap(mem, sizeof(SharedData));
    
    return 0;
}    