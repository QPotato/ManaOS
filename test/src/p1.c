#include "syscall.h"

int main()
{
    int i;
    Write("Entro enbucle infinito, soy p1\n", 25, ConsoleOutput);
    for(i = 0; ; i++)
        if(i % 1000 == 0)
            Write("Hola, soy p1\n", 25, ConsoleOutput);
    return 0;
}
