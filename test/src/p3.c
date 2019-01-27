#include "syscall.h"

int main()
{
    Write("Fui llamado con Exec\n", 22, ConsoleOutput);
    int i;
    for(i = 0; i < 100000; i++)
    {
        if(i % 10000 == 0)
            Write("Sigo vivo, vamos ManaOS!\n", 30, ConsoleOutput);
    }
    return 0;
}
