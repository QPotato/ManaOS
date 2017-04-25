#include "userprog.h"
#include "system.h"

#define min(a,b)  (((a) < (b)) ? (a) : (b))

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
    for(int i = 0; i < this->argc; i++)
    {
        free(this->argv[i]);
    }
    free(this->argv);
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

void UserProg::parseArgs(char* callString, size_t maxSize)
{
    unsigned argcl = 0;
    size_t sz = min(maxSize, strlen(callString));
    
    char **argvl;
    
    int init = 0;
    bool espacios = false;
    
    argvl = (char**) malloc(9000 * sizeof (char*));
    for(unsigned i = 0; i < sz; i++)
    {
        if(espacios)
        {
            if(callString[i] != ' ')
            {
                init = i;
                espacios = false;
            }
        }
        if(!espacios)
        {
            if(callString[i] == ' ')
            {
                argvl[argcl] = (char*)malloc((i - init) * sizeof(char));
                strncpy(argvl[argcl++], &(callString[init]), i - init);
                espacios = true;
            }
        }
    }
    
    //copio los argumentos al userprog
    this->argsize = 0;
    this->argc = argcl;
    this->argv = (char**)malloc(argcl * sizeof(char*));
    for(unsigned i = 0; i < argcl; i++)
    {
        int argSz = strlen(argv[i]);
        this->argv[i] = (char*)malloc(argSz * sizeof(char));
        strncpy(this->argv[i], argvl[i], argSz);
        this->argsize += argSz * sizeof(char);
    }
    for(unsigned i = 0; i < argcl; i++)
        free(argvl[i]);
    free(argvl);
    return;
}

int UserProg::getArgc()
{
    return this->argc;
}

char **UserProg::getArgv()
{
    return this->argv;
}

OpenFile* UserProg::getOpenFile(int fd)
{
    DEBUG('A', "op: %lu\n", abiertos[fd]);
    if(fd < maxFileDes)
        return abiertos[fd];
    else
        return NULL;
}