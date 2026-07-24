#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

//1
/*
void handler(int signum)
{
    printf("Hello from hendler %d",signum);
}
int main()
{
    signal(SIGINT,handler);
    while(1)
    {
        printf("while \n");
        sleep(1);
    }
    return 0;
}

*/
//2
/*
void handler(int signum)
{
    printf("SHUTING DOWN %d\n",signum);
    exit(1);
}
int main()
{
    signal(SIGINT,handler);
    while(1)
    {
        printf("while \n");
        sleep(1);
    }
    return 0;
}
*/

//3


void handler(int signum) {
    printf("SIGINT(%d)\n", signum);
}

int main() {
    sigset_t new_mask, old_mask;   
    signal(SIGINT, handler);

    sigemptyset(&new_mask);
   
    sigaddset(&new_mask, SIGINT);

    printf("Block SIGINT 5 second \n");
   
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

   
    sleep(5);

    printf("unblock \n");
    
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    sleep(5);

    printf("\n");
    return 0;
}