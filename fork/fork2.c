#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
    int pid = fork();
    
    if(pid < 0)
    {
        printf("faild..");
        exit(1);
    }

   else if(pid == 0)
    {
        sleep(7);
        
    }
    
    else
    {
        printf("waiting child...\n");
        wait(NULL);
        printf("Finally my child wake up\n");
    }
    return 0;
}