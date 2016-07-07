#include "syscall.h"

int main()
{
    Create("asd2");
    int fd = Open("asd2");
    Write("lol2", 4, fd);
    
    return 0;
}
