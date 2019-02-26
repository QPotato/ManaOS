#include "syscall.h"

#define BLOCK_BYTES 256

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
    if(argc < 2)
    {
        Write("No le pasaste archivos a cat, gato!!\n", 37, ConsoleOutput);
        return 0;
    }
    char fn[128];
    for(i = 1; i < argc; i++)
    {
        getArg(i, fn);
        OpenFileId id = Open(fn);

        if(id < 0)
        {
            Write("Archivo inexistente! Vamos ManaOS!\n", 35, ConsoleOutput);
            return -1;
        }

        char block[BLOCK_BYTES];
        int read = BLOCK_BYTES;
        while(read == BLOCK_BYTES)
        {
            read = Read(block, BLOCK_BYTES, id);
            Write(block, read, ConsoleOutput);
        }
        Close(fn);
    }
    return 0;
}

