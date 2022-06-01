#include <stdio.h>
int main(int argc, char const *argv[])
{
    int i = 0x1000000f;
    if (*(char *)&i == 0xf)
        printf("小端字节序\n"); //结果是 15.也就是说，低地址 存储的是 低字节。
    else if (*(char *)&i == 0x1)
        printf("大端字节序\n");
    else
        printf("未知\n");

    return 0;
}
