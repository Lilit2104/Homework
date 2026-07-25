#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
   int pid = fork();

   if(pid == 0)
     {
       sleep(2);  
        exit(42);
      }

      else 
      {
         int status;
         wait(&status);

        if(WIFEXITED(status))
        {
             printf("%d\n", WEXITSTATUS(status));
        }
     }
     
     
    return 0;
}
