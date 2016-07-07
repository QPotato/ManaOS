#include "syscall.h"

int main()
{
    int fd = Open("asd2");
    Write("lol2", 5, fd);
    
    return 0;
}
