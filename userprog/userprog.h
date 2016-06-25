#ifndef __UserProg_H__
#define __UserProg_H__
#include "thread.h"
#include "openfile.h"

#define MAX_NOMBRE 1024
#define MAX_ABIERTOS 256
#define MAX_PROCESOS 10000
// Estructura que almacena toda la informacion necesaria de un proceso de usuario.
class UserProg
{
    public:
        UserProg(Thread* t, int pid);
        ~UserProg();
        Thread getThread();
        int abrir(const char* nombre);
        OpenFile* getOpenFile(int fd);
        void cerrar(int fd);
    
        int getPid() { return Pid; }
        Thread* gThread() { return thread; }

    private:
        Thread* thread;
        OpenFile* abiertos[MAX_ABIERTOS];
        int maxFileDes;
        int Pid;    
};

//--------------------------------------------------------//
// Clase encargada de administrar los procesos de usuario.
//--------------------------------------------------------//
class UserProgMgr
{
    public:
        // Constructor y destructor
        UserProgMgr();
        ~UserProgMgr();
        
        // Crea un nuevo proceso de usuario asociado al thread actual.
        void add();
        
        // Devuelve un puntero al proceso de usuario que esta ejecutando actualmente.
        UserProg* getCurrent();

    private:
        UserProg* procesos[MAX_PROCESOS];
        int nProcesos;
};
#endif //__UserProg_H__
