#include "userprog.h"
#include "system.h"

UserProg::UserProg(AddrSpace* s)
{
    abiertos[0] = NULL;
    abiertos[1] = NULL;
    maxFileDes = 2;
    space = s;
}

UserProg::~UserProg()
{
    for(int i = 0; i < maxFileDes; i++)
        this->cerrar(i);
    if(space)
    {
        delete space;
        space = NULL;
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
