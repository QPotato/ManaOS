#ifndef __UserProg_H__
#define __UserProg_H__
#include "addrspace.h"
#include "openfile.h"
#include "thread.h" // para la lista de hijos
#include "syscall.h" // SpaceId, MAX_NOMBRE
#include "listaCumbiera.h"

#define MAX_ABIERTOS 256
#define MAX_PROCESOS 10000

class Thread;

// Estructura que almacena toda la informacion necesaria de un proceso de usuario.
class UserProg
{
    public:
        UserProg(Thread* t, AddrSpace* s);
        ~UserProg();

        // Para las syscalls de archivos
        int abrir(const char* nombre);
        OpenFile* getOpenFile(int fd);
        void cerrar(int fd);

        // Para las syscalls de argumentos
        void parseArgs(char*, size_t);
        int getArgc();
        char** getArgv();

        // Para las syscalls de exec y join.
        SpaceId nuevoHijo(char* filename, int j);
        bool join(SpaceId hijo);

        // Proxies de los metodos de space.
        void saveState();
        void restoreState();

        #ifdef USE_TLB
        TranslationEntry* translate(int vpn);
        #endif

        // Le dice al programa de usuario que se mate.
        // Cierra los archivos, libera su memotia y mata el thread.
        void nisman();


    private:
        OpenFile* abiertos[MAX_ABIERTOS];
        int maxFileDes;

        int argc;
        char **argv;

        listaCumbiera<Thread> hijos;
        int maxHijos;

        Thread* thread;
        AddrSpace* space;
};
#endif
