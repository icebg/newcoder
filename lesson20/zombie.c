#include <unistd.h>
#include <stdio.h>

int main()
{
    // 创建一个子进程，在子进程中执行exec函数族中的函数
    pid_t pid = fork();

    if (pid > 0)
    {
        // 父进程
        while (1)
        {
            printf("i am parent process, pid : %d\n", getpid());
            sleep(1);
        }
    }
    else if (pid == 0)
    {
        printf("i am child process, pid : %d\n", getpid());
    }

    for (int i = 0; i < 3; i++)
    {
        printf("i = %d, pid = %d\n", i, getpid());
    }

    return 0;
}