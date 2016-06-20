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
    consola = new Console("consola_read", "consola_write", ra, wd, this);
}

SynchConsole::~SynchConsole()
{
    delete consola;
    delete writeLock;
    delete readLock;
    delete writeSemaphore;
    delete readSemaphore;
}

void SynchConsole::read(char* outStr, int readSize)
{
    for(int i = 0; i < readSize; i++)
    {
        readLock->Acquire();			// only one disk I/O at a time
        readSemaphore->P();			// wait for interrupt
        outStr[i] = consola->GetChar();
        readLock->Release();
    }
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
