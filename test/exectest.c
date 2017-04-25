#include "syscall.h"

int main()
{
    Write("Llamo a p1\n", 11, ConsoleOutput);
    Exec("../test/p1");
    Write("Llamo a p2\n", 11, ConsoleOutput);
    Exec("../test/p2");
    Write("Fin\n", 10, ConsoleOutput);
    return 0;
}
