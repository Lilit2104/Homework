#include  <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
    int pid = fork();

    if(pid < 0)
    {
        printf("fork failed");
        exit(1);
    }

    else if(pid == 0)
    {
        sleep(5);
        exit(33);
    }

    else
    {
        int status;
        printf("parent wait..\n");

        waitpid(pid, &status,0);
        
        if(WIFEXITED(status))
        {
            int res = WEXITSTATUS(status);
            printf("tadaa: %d\n", res);
        }

    }
    return 0;
}