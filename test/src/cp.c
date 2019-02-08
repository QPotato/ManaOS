#include "syscall.h"
#define MAX_FILENAME 128
#define BLOCK_BYTES 256

int main()
{
    int argc = getArgc();
    if(argc < 3)
        return 1;
    char origin[MAX_FILENAME], destination[MAX_FILENAME];
    getArg(1, origin);
    getArg(2, destination);
    
    Create(destination);
    OpenFileId o = Open(origin), d = Open(destination);
    char block[BLOCK_BYTES];
    int read = BLOCK_BYTES;
    while(read == BLOCK_BYTES)
    {
        read = Read(block, BLOCK_BYTES, o);
        Write(block, read, d);
    }
    Close(o); Close(d);
    return 0;
}
