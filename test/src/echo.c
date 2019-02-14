#include "syscall.h"

int strlen(char *in)
{
    int len = 0;
    while(in[len] != '\0')
        len++;
    return len;
}

int main()
{
    int argc = getArgc(), i;
    
    for(i = 1; i < argc; i++)
    {
        char fn[128];
        getArg(i, fn);
        Write(fn, strlen(fn), ConsoleOutput);
        Write(" ", 1, ConsoleOutput);
    }
    Write("\n", 1, ConsoleOutput);
    return 0;
}
