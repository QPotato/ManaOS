// exception.cc
//	Entry point into the ManaOS kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the ManaOS kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the ManaOS kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "machine.h"

#include "syscall.h"
#include "pasamemoria.h"

#ifdef USE_TLB
#include "tlbHandler.h"
#endif
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the ManaOS kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

void execHandler();
void getArgcHandler();
void getArgHandler();
void joinHandler();

void createHandler();
void openHandler();
void readHandler();
void writeHandler();
void closeHandler();

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    UserProg* currentProgram = currentThread->userProg;
    if (which == SyscallException) {
        switch(type)
        {
            case SC_Halt:
                DEBUG('A', "Shutdown, initiated by user program.\n");
                interrupt->Halt();
           	    break;

       	    case SC_Exit:
                currentProgram->nisman();
           	    break;

       	    case SC_Exec:
       	        execHandler();
           	    break;

       	    case SC_GetArgc:
       	        getArgcHandler();
       	        break;

       	    case SC_GetArg:
       	        getArgHandler();
       	        break;

       	    case SC_Join:
       	        joinHandler();
           	    break;

       	    case SC_Create:
       	        createHandler();
           	    break;

       	    case SC_Open:
       	        openHandler();
           	    break;

       	    case SC_Read:
       	        readHandler();
           	    break;

       	    case SC_Write:
       	        writeHandler();
           	    break;

       	    case SC_Close:
       	        closeHandler();
           	    break;

       	    case SC_Fork:
	              DEBUG('A', "Syscall no implementada: Fork.\n");
                currentProgram->nisman();
           	    break;

       	    case SC_Yield:
	              DEBUG('A', "Syscall no implementada: Yield.\n");
                currentProgram->nisman();
           	    break;

        }
    }
    else if(which == AddressErrorException)
    {
  	    printf("Segmentation Fault, PAVO!\nVamos ManaOS!\n");
        currentProgram->nisman();
    }

    #ifdef USE_TLB
        else if(which == PageFaultException) {
            int virtualAddress = machine->ReadRegister(BadVAddrReg);
            tlbHandler::pageFaultHandler(currentProgram, virtualAddress);
        }
        else if(which == ReadOnlyException) {
            int virtualAddress = machine->ReadRegister(BadVAddrReg);
            tlbHandler::readOnlyHandler(currentProgram, virtualAddress);
        }
        else if(which == BusErrorException) {
            int virtualAddress = machine->ReadRegister(BadVAddrReg);
            tlbHandler::busErrorHandler(currentProgram, virtualAddress);
        }
    #else
        else if(which == PageFaultException) {
            printf("Segmentation fault, pavo! Vamos ManaOS\n");
            currentProgram->nisman();
        }
    #endif

    else
    {
  	    printf("Unexpected user mode exception %d %d\n", which, type);
  	    ASSERT(false);
    }
}
