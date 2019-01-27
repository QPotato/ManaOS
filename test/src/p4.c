#include "syscall.h"

int strlen(char* in)
{
    int len = 0;
    while(in[len] != '\0')
        len++;
    return len;
}


int main()
{
    int argc = getArgc(), i;
    char arg[100];
    Write("Hola, soy p4\n", 13, ConsoleOutput);
    for(i = 0; i < argc; i++)
    {
        getArg(i, arg);
        Write("Hola, soy p4. argumento =", 25, ConsoleOutput);
        Write(arg, strlen(arg), ConsoleOutput);
        Write("\n", 1, ConsoleOutput);
    }
    return 0;
}
