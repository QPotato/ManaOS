
#ifndef SWAP_H
#define SWAP_H
#include "filesys.h"
#include "machine.h"
extern FileSystem *fileSystem;
extern Machine* machine;

class Swap {
    public:
        Swap(char *asid, int addrSpaceSz);
        ~Swap();

        void swapIn(unsigned vpn, unsigned physPage);
        void swapOut(unsigned vpn, unsigned physPage);
    private:
        OpenFile* swapFile;
        char fileName[32];
};

#endif