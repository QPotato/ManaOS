#include "syscall.h"

int main()
{
    Create("asd1");
    int fd = Open("asd1");
    Write("lol1", 4, fd);
    
    return 0;
}
