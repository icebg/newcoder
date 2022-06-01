#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char const *argv[])
{
    pid_t pid;
    for (int i = 0; i < 5; i++)
    {
        pid = fork();
        if( pid == 0)
            break;//只允许父进程来循环fork。
    }
    if (pid > 0)
    {
        while (1)
        {
            printf("ParentProcess , pid == %d\n", getpid());
            int ret = wait(NULL);
            if (ret=-1)
            {
                break;
            }

            printf("child die, pid = %d \n",ret);
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        printf("ChildProcess , pid == %d\n", getpid());
    }

    return 0;
}
