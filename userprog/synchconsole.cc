#include "synchconsole.h"

static void ra(void* arg)
{
    SynchConsole* synchCon = (SynchConsole*) arg;
    synchCon->handleReadAvail();
}

static void wd(void* arg)
{
    SynchConsole* synchCon = (SynchConsole*) arg;
    synchCon->handleWriteDone();
}

SynchConsole::SynchConsole()
{
    writeSemaphore = new Semaphore("synch console write", 0);
    writeLock = new Lock("synch console write lock");
    readSemaphore = new Semaphore("synch console read", 0);
    readLock = new Lock("synch console read lock");
    consola = new Console(NULL, NULL, ra, wd, this);
}

SynchConsole::~SynchConsole()
{
    delete consola;
    delete writeLock;
    delete readLock;
    delete writeSemaphore;
    delete readSemaphore;
}

int SynchConsole::read(char* outStr, int readSize)
{
    char c;
    int i;
    for(i = 0; i < readSize; i++)
    {
        readLock->Acquire();			// only one disk I/O at a time
        readSemaphore->P();			// wait for interrupt
        c = consola->GetChar();
        if(c == EOF)
            break;
        outStr[i] = c;
        readLock->Release();
    }
    return i;
}

void SynchConsole::write(const char* inStr, int writeSize)
{
    for(int i = 0; i < writeSize; i++)
    {
        writeLock->Acquire();			// only one disk I/O at a time
        consola->PutChar(inStr[i]);
        writeSemaphore->P();			// wait for interrupt
        writeLock->Release();
    }
}

void SynchConsole::handleReadAvail()
{ 
    readSemaphore->V();
}

void SynchConsole::handleWriteDone()
{ 
    writeSemaphore->V();
}
