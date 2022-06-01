
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    int ret = mkfifo("fifo1",0644);
    if (ret==-1)
    {
        perror("mkfifo");
        exit(-1);
    }

    /* code */
    return 0;
}
