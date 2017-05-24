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
        Write("No le pasaste archivos a cat, Macri!!\n", 39, ConsoleOutput);
        return 0;
    }
    char fn[128];
    for(i = 1; i < argc; i++)
    {
        getArg(i, fn);
        OpenFileId id = Open(fn);
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

