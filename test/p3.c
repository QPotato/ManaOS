#include "syscall.h"

int main()
{
    int fd = Open("asd3");
    Write("lol3", 5, fd);
    
    return 0;
}
