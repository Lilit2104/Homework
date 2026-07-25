#include  <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void cleanup1(void)
{
    printf("cleanup1: woriking\n");
}

void cleanup2(void)
{
    printf("cleanup2: woriking\n");
}

int main()
{
    atexit(cleanup1);
    atexit(cleanup2);

    printf("WORKKKKKKKKKKKK!!!!\n");
    
    return 0;
}