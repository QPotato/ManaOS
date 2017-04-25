#include "syscall.h"

int main()
{
    int argc = getArgc(), i;
    char arg[100];
    Write("Hola, soy p4\n", 40, ConsoleOutput);
    for(i = 0; i < argc; i++)
    {
        getArg(i, arg);
        Write("Hola, soy p4. argumento =", 40, ConsoleOutput);
        Write(arg, 40, ConsoleOutput);
        Write("\n", 40, ConsoleOutput);
    }
    return 0;
}
