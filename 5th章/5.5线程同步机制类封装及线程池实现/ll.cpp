#include <iostream>
#include <string.h>
#include <sys/epoll.h>
using namespace std;

class Base
{
    static const int k = 9;

public:
    Base() {}
    void print()
    {
        cout << k << endl;
    }
};
int main(int argc, char const *argv[])
{
    epoll_ctl();
    Base b;
    b.print();
    return 0;
}