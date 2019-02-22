#include "system.h"
#include "syscall.h"
#include "pasamemoria.h"

void incrementar_PC();
bool checkFilename(char*);
void StartProcess(const char *filename);
void sProc(void* n);

void execHandler() {
    int j, r;
    char* filename;
    SpaceId s;
    UserProg* up = currentThread->userProg;
    
    r = machine->ReadRegister(4);
    j = machine->ReadRegister(5);
    
    // Lo libera StartProcess en progtest.cc por concurrencia.
    filename = (char*) malloc(sizeof(char) * MAX_NOMBRE);
    readStrFromUsrSegura(r, filename, MAX_NOMBRE);

    s = up->nuevoHijo(filename, j);

    DEBUG('A', "Exec con: #%s#. Vamos ManaOS!\n", filename);
    machine->WriteRegister(2, s);

    incrementar_PC();
}

void getArgcHandler() {
    int argc;
    UserProg* up = currentThread->userProg;
    
    argc = up->getArgc();
    machine->WriteRegister(2, argc);
    incrementar_PC();
}

void getArgHandler() {
    int argc, dArg;
    char** argv;
    int argn;
    UserProg* up = currentThread->userProg;
    
    argn = machine->ReadRegister(4);
    dArg = machine->ReadRegister(5);
    argc = up->getArgc();
    argv = up->getArgv();
    if(argn < argc)
    {
        argv = up->getArgv();
        writeStrToUsr(argv[argn], dArg);
    }
    else
    {
        writeStrToUsr((char *)"", dArg);
    }
    incrementar_PC();
}

void joinHandler() {
    SpaceId s;
    UserProg* up = currentThread->userProg;
    
    s = machine->ReadRegister(4);
    if(up->join(s))
        machine->WriteRegister(2, 1);
    else
        machine->WriteRegister(2, 0);
    incrementar_PC();
}

void createHandler() {
    int r;
    char filename[MAX_NOMBRE];
    r = machine->ReadRegister(4);

    readStrFromUsrSegura(r, filename, MAX_NOMBRE);
    if(fileSystem->Create(filename, 0))
    {
        DEBUG('A', "Create: la mejor. Vamos ManaOS!\n");
        machine->WriteRegister(2, 0);
    }
    else
    {
        DEBUG('A', "Create: la peor. Vamos ManaOS!\n");
        machine->WriteRegister(2, -1);
    }

    incrementar_PC();
}

void openHandler() {
    char filename[MAX_NOMBRE];
    int fileDes;
    UserProg* up = currentThread->userProg;
    
    readStrFromUsrSegura(machine->ReadRegister(4), filename, MAX_NOMBRE);
    if(!checkFilename(filename))
    {
        DEBUG('A', "El programa de usuario, quiso abrir un archivo con filename no valido, llamado #%s#. Vamos ManaOS!\n", filename);
        machine->WriteRegister(2, -1);
        
    }
    else
    {
        fileDes = up->abrir(filename);
        machine->WriteRegister(2, fileDes);
        DEBUG('A', "El programa de usuario abrio un archivo con FD: %i, llamado #%s#. Vamos ManaOS!\n", fileDes, filename);
    }
    incrementar_PC();
}

void readHandler() {
    int usrBuffer, opSize, fileDes, bytes;
    char* buffer;
    UserProg* up = currentThread->userProg;
    OpenFile* op;
    
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
    }
    else if(fileDes == ConsoleInput)
    {
        bytes = synchConsole->read(buffer, opSize);
        printf("AAAAAAAAAAAAAAAAAAHHH\n");
        writeBuffToUsr(buffer, usrBuffer, bytes);
        machine->WriteRegister(2, bytes);
    }
    else if(fileDes == ConsoleOutput)
    {
        DEBUG('A',"No podes leer de la salida a consola, PAVO! Vamos ManaOS!\n");
        machine->WriteRegister(2, -1);
    }
    else
    {
        op = up->getOpenFile(fileDes);
        bytes = op->Read(buffer, opSize);
        writeBuffToUsr(buffer, usrBuffer, bytes);
        machine->WriteRegister(2, bytes);
        
    }
    free(buffer);
    incrementar_PC();
}

void writeHandler() {
    int usrBuffer, opSize, fileDes;
    char* buffer;
    OpenFile* op;
    UserProg* up = currentThread->userProg;
    
    // r4 - puntero al buffer (char*)
    // r5 - size (int)
    // r6 - id de archivo (OpenfileDes)
    usrBuffer = machine->ReadRegister(4);
    opSize = machine->ReadRegister(5);
    fileDes = machine->ReadRegister(6);
    buffer = (char*) malloc(sizeof(char) * opSize);

    DEBUG('V', "fileDes = %d\n", fileDes);

    if(buffer == NULL || fileDes < 0)
    {
        DEBUG('A', "Operacion de I/O incorrecta.\n");
        machine->WriteRegister(2, -1);
    }
    else if(fileDes == ConsoleOutput)
    {
        readBuffFromUsr(usrBuffer, buffer, opSize);
        DEBUG('A', "Usuario escribe #%*s# FD: %d\n", opSize, buffer, fileDes);
        synchConsole->write(buffer, opSize);
        machine->WriteRegister(2, 0);
    }
    else if(fileDes == ConsoleInput)
    {
        DEBUG('A', "No podes escribir en la entrada de consola, PAVO! Vamos ManaOS!\n");
        machine->WriteRegister(2, -1);
    }
    else
    {
        readBuffFromUsr(usrBuffer, buffer, opSize);
        op = up->getOpenFile(fileDes);
        DEBUG('K', "Usuario escribe #%*s# FD: %d\n", opSize, buffer, fileDes);
        op->Write(buffer, opSize);
        machine->WriteRegister(2, 0);
    }
    
    free(buffer);
    incrementar_PC();
}

void closeHandler() {
    int fileDes;
    UserProg* up = currentThread->userProg;
    
    fileDes = machine->ReadRegister(6);
    up->cerrar(fileDes);
    incrementar_PC();
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
