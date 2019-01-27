#include "syscall.h"

int main()
{
    int i;
    Write("Entro enbucle infinito, soy p2\n", 25, ConsoleOutput);
    for(i = 0; ; i++)
        if(i % 1000 == 0)
            Write("Hola, soy p2\n", 25, ConsoleOutput);
    return 0;
}
