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
#include "syscall.h"
#include "pasamemoria.h"
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
void incrementar_PC();
bool checkFilename(char*);
void StartProcess(const char *filename);
void sProc(void* n){ StartProcess((char*)n); }

void ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    
    //TODO: limpiar toda esta basura
    //variables de create
    int r;
    char nombre[MAX_NOMBRE];
    
    //variables de read y write
    int usrBuffer;
    int opSize;
    int fileDes;
    char* buffer;
    OpenFile* op;
    
    //variables de open
    char filename[MAX_NOMBRE];
    UserProg* up = currentThread->userProg;
    
    if (which == SyscallException) {
        switch(type)
        {
            case SC_Halt:
	            DEBUG('A', "Shutdown, initiated by user program.\n");
           	    interrupt->Halt();
           	    break;
       	    
       	    case SC_Exit:
	            DEBUG('A', "Syscall no implementada: Exit.\n");
           	    interrupt->Halt();
           	    break;
       	    
       	    case SC_Exec:
       	        r = machine->ReadRegister(4);
       	        readStrFromUsrSegura(r, nombre, MAX_NOMBRE);
       	        currentThread->Fork(sProc, (void*) nombre);
           	    break;
       	    
       	    case SC_Join:
	            DEBUG('A', "Syscall no implementada: Join.\n");
           	    interrupt->Halt();
           	    break;
       	    
       	    case SC_Create:
       	        r = machine->ReadRegister(4);
       	        
       	        readStrFromUsrSegura(r, nombre, MAX_NOMBRE);
       	        if(fileSystem->Create(nombre, 0))
       	        {
    	            DEBUG('A', "Create: la mejor. Vamos ManaOS!\n");
    	            machine->WriteRegister(2, 0);
	            }
	            else
	            {
    	            DEBUG('A', "Create: la peor.\n");
    	            machine->WriteRegister(2, -1);
    	        }
    	        
    	        incrementar_PC();
           	    break;
           	    
       	    case SC_Open:
       	        readStrFromUsrSegura(machine->ReadRegister(4), filename, MAX_NOMBRE);
                if(!checkFilename(filename))
       	        {
           	        machine->WriteRegister(2, -1);
           	        DEBUG('A', "El programa de usuario, quiso abrir un archivo con nomrbe no valido, llamado %s\n", filename);
       	            
       	        }
       	        else
       	        {
           	        fileDes = up->abrir(filename);
           	        machine->WriteRegister(2, fileDes);
           	        DEBUG('A', "El programa de usuario, abrio un archivo con FD: %i, llamado %s\nVamos ManaOS!\n", fileDes, filename);
       	        }
       	        incrementar_PC();
           	    break;
       	    
       	    case SC_Read:
   	            // r4 - puntero al buffer (char*)
   	            // r5 - size (int)
   	            // r6 - id de archivo (OpenfileDes)
   	            usrBuffer = machine->ReadRegister(4);
   	            opSize = machine->ReadRegister(5);
   	            fileDes = machine->ReadRegister(6);
   	            buffer = (char*) malloc(sizeof(char) * opSize);
   	            if(buffer == NULL)
   	            {
                    DEBUG('A', "Operacion de I/O incorrecta.\n");
       	            machine->WriteRegister(2, -1);
       	            break;
   	            }
   	            
                if(fileDes == ConsoleInput)
                {
                    synchConsole->read(buffer, opSize);
                    writeBuffToUsr(buffer, usrBuffer, opSize);
       	            machine->WriteRegister(2, 0);
                }
                else if(fileDes == ConsoleOutput)
                {
                    printf("No podes leer de la salida a consola, PAVO! Vamos ManaOS!\n");
            	    interrupt->Halt(); //TODO: cambiar a que mate el proceso llamante
                }
                else
                {
                    op = up->getOpenFile(fileDes);
                    op->Read(buffer, opSize);
                    writeBuffToUsr(buffer, usrBuffer, opSize);
       	            machine->WriteRegister(2, 0);
                    
                }
                free(buffer);
    	        
    	        incrementar_PC();
           	    break;
           	    
       	    case SC_Write:
       	        // void Write(char *buffer, int size, OpenfileDes id);
   	            // r4 - puntero al buffer (char*)
   	            // r5 - size (int)
   	            // r6 - id de archivo (OpenfileDes)
   	            usrBuffer = machine->ReadRegister(4);
   	            opSize = machine->ReadRegister(5);
   	            fileDes = machine->ReadRegister(6);
   	            buffer = (char*) malloc(sizeof(char) * opSize);
   	            if(buffer == NULL)
   	            {
                    DEBUG('A', "Operacion de I/O incorrecta.\n");
       	            machine->WriteRegister(2, -1);
       	            break;
   	            }
   	            
                else if(fileDes == ConsoleOutput)
                {
                    readBuffFromUsr(usrBuffer, buffer, opSize);
                    synchConsole->write(buffer, opSize);
       	            machine->WriteRegister(2, 0);
                }
                else if(fileDes == ConsoleInput)
                {
                    printf("No podes escribir en la entrada de consola, PAVO! Vamos ManaOS!\n");
            	    interrupt->Halt(); //TODO: cambiar a que mate el proceso llamante
                }
                else
                {
                    
                    readBuffFromUsr(usrBuffer, buffer, opSize);
                    op = up->getOpenFile(fileDes);
                    op->Write(buffer, opSize);
       	            machine->WriteRegister(2, 0);
                }
                
    	        free(buffer);
    	        incrementar_PC();
           	    break;
       	    
       	    case SC_Close:
   	            fileDes = machine->ReadRegister(6);
                up->cerrar(fileDes);
           	    break;
       	    
       	    case SC_Fork:
	            DEBUG('A', "Syscall no implementada: Fork.\n");
           	    interrupt->Halt();
           	    break;
       	    
       	    case SC_Yield:
	            DEBUG('A', "Syscall no implementada: Yield.\n");
           	    interrupt->Halt();
           	    break;
       	        
        }
    }
    else if(which == AddressErrorException)
    {
	    printf("Segmentation Fault, PAVO!\nVamos ManaOS!\n");
	    interrupt->Halt(); //TODO: cambiar a que mate el proceso llamante
    }
    else
    {
	    printf("Unexpected user mode exception %d %d\n", which, type);
	    ASSERT(false);
	    
    }
}

void incrementar_PC()
{
    machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
    machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg) + 4);
}

bool checkFilename(char* name)
{
    for(int i = 0; name[i] != '\0'; i++)
    {
        char c = name[i];
        if( (c < 'a' || c > 'z')
         && (c < 'A' || c > 'Z')
         && (c < '0' || c > '9')
         && (c != '-')
         && (c != '_')
         && (c != '(')
         && (c != ')')
         && (c != '.')
         && (c != ',')
         && (c != ' ') )
            return false;
    }
    return true;
}
