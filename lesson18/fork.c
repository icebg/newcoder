#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
    pid_t pid = fork();

    if( pid == - 1)
    {
        printf("error\n");
    }
    else if( pid > 0 )
    {
        printf("I'm parent. pid : %d , ppid : %d\n",getpid(),getppid());
    }
    else if( pid == 0)
    {
        printf("I'm child. pid : %d , ppid : %d\n",getpid(),getppid());
    }
    for (int i = 0; i < 300; i++)
    {
        printf("i is  %d , pid: %d\n", i, getpid());
    }
    return 0;
}