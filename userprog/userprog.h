#ifndef __UserProg_H__
#define __UserProg_H__
#include "addrspace.h"
#include "openfile.h"

#define MAX_NOMBRE 1024
#define MAX_ABIERTOS 256
#define MAX_PROCESOS 10000

// Estructura que almacena toda la informacion necesaria de un proceso de usuario.
class UserProg
{
    public:
        UserProg(AddrSpace *s);
        ~UserProg();
        int abrir(const char* nombre);
        OpenFile* getOpenFile(int fd);
        void cerrar(int fd);
        
        void parseArgs(char*, size_t);
        int getArgc();
        char** getArgv();
        
        AddrSpace *space;			// User code this thread is running.

    private:
        OpenFile* abiertos[MAX_ABIERTOS];
        int maxFileDes;
        int argc;
        char **argv;
        size_t argsize;
};
#endif
