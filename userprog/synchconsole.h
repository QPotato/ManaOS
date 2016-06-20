#ifndef __SYNCHCONSOLE_H__
#define __SYNCHCONSOLE_H__

#include "console.h"
#include "synch.h"

class SynchConsole
{
    public:
        SynchConsole();
        ~SynchConsole();
        
        void read(char* outStr, int readSize);
        void write(const char* inStr, int writeSize);
        
        void handleReadAvail();
        void handleWriteDone();
    private:
        Console* consola;
        Semaphore* readSemaphore;
        Semaphore* writeSemaphore;
        Lock* readLock;
        Lock* writeLock;
};

#endif //__SYNCHCONSOLE_H__
