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
        printf("This is my PID: %d\n", getpid());
        sleep(33);
        exit(1);
    }

    else
    {
        sleep(2);

        kill(pid, SIGINT);

        int status;
        waitpid(pid, &status,0);

        if(WIFEXITED(status))
        {
            int res = WEXITSTATUS(status);
            printf("Child is exited: %d\n", res);
        }
        
        else if(WIFSIGNALED(status))
        {
            int result = WTERMSIG(status);
            printf("Child is signalled: %d\n", result);
        }

    }
    return 0;
}