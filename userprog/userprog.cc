#include "userprog.h"
#include "system.h"

UserProg::UserProg(Thread* t, int pid)
{
    thread = t;
    abiertos[0] = NULL;
    abiertos[1] = NULL;
    Pid = pid;
    maxFileDes = 2;
}

UserProg::~UserProg()
{
    for(int i = 0; i < maxFileDes; i++)
        if(abiertos[i] != NULL)
        {
            delete abiertos[i];
            abiertos[i] = NULL;
        }
}

int UserProg::abrir(const char* nombre)
{
    if(maxFileDes >= MAX_ABIERTOS)
    {
        DEBUG('A', "El proceso tiene demasiados archivos abiertos\n");
        return -1;
    }
    OpenFile* op = fileSystem->Open(nombre);
    abiertos[maxFileDes] = op;
    return maxFileDes++;
}

void UserProg::cerrar(int fd)
{
    DEBUG('A', "Warning: Llamada a cerrar sobre un archivo ya cerrado.\n");
    if(abiertos[fd] != NULL)
    {
        delete abiertos[fd];
        abiertos[fd] = NULL;
    }
}

OpenFile* UserProg::getOpenFile(int fd)
{
    DEBUG('A', "op: %lu\n", abiertos[fd]);
    if(fd < maxFileDes)
        return abiertos[fd];
    else
        return NULL;
}

//--------------------------------------------------------//
// Clase encargada de administrar los procesos de usuario.
//--------------------------------------------------------//
UserProgMgr::UserProgMgr()
{
    nProcesos = 0;
}

UserProgMgr::~UserProgMgr()
{
    for(int i = 0; i < nProcesos; i++)
        if(procesos[i] != NULL)
            delete procesos[i];
}

void UserProgMgr::add()
{
    if(nProcesos >= MAX_PROCESOS)
    {
        DEBUG('A', "Se excedio el limite de procesos\n");
        return;
    }
    UserProg* up = new UserProg(currentThread, nProcesos);
    procesos[nProcesos] = up;
    nProcesos++;
}

UserProg* UserProgMgr::getCurrent()
{
    for(int i = 0; i < nProcesos; i++)
        if(procesos[i]->gThread() == currentThread)
            return procesos[i];
    return NULL;
}
