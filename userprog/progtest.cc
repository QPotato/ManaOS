// progtest.cc 
//	Test routines for demonstrating that ManaOS can load
//	a user program and execute it.  
//
//	Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"
 
//----------------------------------------------------------------------
// StartProcess
// 	Run a user program.  Open the executable, load it into
//	memory, and jump to it.
//----------------------------------------------------------------------
void StartProcess(char *filename)
{
    char *fn;
    fn = (char*)malloc(strlen(filename) + 1);
    strncpy(fn, filename, strlen(filename));
    for(unsigned i = 0; i < strlen(filename); i++)
    {
        if(fn[i] == ' ')
        {
            fn[i] = '\0';
            break;
        }
    }
    fn[strlen(filename)] = '\0';
    
    DEBUG('A', "quiero iniciar proceso con filename -%s-\n", fn);
    OpenFile *executable = fileSystem->Open(fn);
    AddrSpace *space;

    DEBUG('A', "executable: %ld", executable);

    if (executable == NULL)
    {
        printf("Unable to open file %s\n", fn);
        return;
    }
    DEBUG('A', "Creando ejecutable a partir de #%s#. Vamos ManaOS\n", filename);
    space = new AddrSpace(executable);    
    currentThread->userProg = new UserProg(space);
    
    currentThread->userProg->parseArgs(filename, MAX_NOMBRE);
    free(fn);
    free(filename); // lo tenemos que liberar aca por temas de concurrencia. Esta funion se ejecuta en un fork. ExceptionHandler no tiene forma de saber cuando lo terminamos de usar.
    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register

    machine->Run();			// jump to the user progam
    ASSERT(false);			// machine->Run never returns;
					// the address space exits
					// by doing the syscall "exit"
}

// Version ejecutable por Thread::Fork
void sProc(void* n){ StartProcess((char*)n); }

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
// 	Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void ReadAvail(void* arg) { readAvail->V(); }
static void WriteDone(void* arg) { writeDone->V(); }

//----------------------------------------------------------------------
// ConsoleTest
// 	Test the console by echoing characters typed at the input onto
//	the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void 
ConsoleTest (const char *in, const char *out)
{
    char ch;

    console = new Console(in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    
    for (;;) {
	readAvail->P();		// wait for character to arrive
	ch = console->GetChar();
	console->PutChar(ch);	// echo it!
	writeDone->P() ;        // wait for write to finish
	if (ch == 'q') return;  // if q, quit
    }
}
