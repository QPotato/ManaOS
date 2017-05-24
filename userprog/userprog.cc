#include "userprog.h"
#include "system.h"

#define min(a,b)  (((a) < (b)) ? (a) : (b))

void sProc(void* n);

UserProg::UserProg(AddrSpace* s)
{
    abiertos[0] = NULL;
    abiertos[1] = NULL;
    maxFileDes = 2;
    space = s;

    maxHijos = 0;
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
    
    argvl = (char**) malloc(90 * sizeof(char*));

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
                argvl[argcl] = (char*)malloc((i - init) * sizeof(char) + 1);
                strncpy(argvl[argcl], &(callString[init]), i - init);
                argvl[argcl++][i-init] = '\0';
                espacios = true;
            }
            else if(callString[i+1] == '\0')
            {
                argvl[argcl] = (char*)malloc((i - init) * sizeof(char) + 2);
                strncpy(argvl[argcl], &(callString[init]), i - init + 1);
                argvl[argcl++][i-init+1] = '\0';
                espacios = true;
            }
        }
    }
    
    //copio los argumentos al userprog
    
    this->argc = argcl;
    this->argv = (char**)malloc(argcl * sizeof(char*));

    for(unsigned i = 0; i < argcl; i++)
    {
        int argSz = strlen(argvl[i]);
        this->argv[i] = (char*)malloc(argSz * sizeof(char) + 1);
        strncpy(this->argv[i], argvl[i], argSz);
        this->argv[i][argSz] = '\0';
    }

    for(unsigned i = 0; i < argcl; i++)
        free(argvl[i]);
    free(argvl);

    return;
}

int UserProg::getArgc()
{
    DEBUG('B', "estoy en getArgc y voy a devolver %d\n", this->argc);
    return this->argc;
}

char **UserProg::getArgv()
{
    DEBUG('B', "estoy en getArgv");
    return this->argv;
}

SpaceId UserProg::nuevoHijo(char *filename, int j)
{
    Thread *t;
    char *fn;
    fn = (char*)malloc(strlen(filename) + 1);
    strncpy(fn, filename, strlen(filename));
    for(int i = 0; i < strlen(filename); i++)
    {
        if(fn[i] == ' ')
        {
            fn[i] = '\0';
            break;
        }
    }
    fn[strlen(filename)] = '\0';

    bool bjoin = j == 0 ? false : true;
    t = new Thread(fn, bjoin);

    DEBUG('A', "cree el thread. fn: %s. Vamos ManaOS!\n", fn);
    
    int ret = -1;
    if(bjoin)
        ret = hijos.put(t);

    t->Fork(sProc, (void*)filename);
    DEBUG('A', "Forkee. userProg: %ld. Vamos ManaOS!\n", t->userProg);

    return ret;
}

bool UserProg::join(SpaceId hijo)
{
    Thread* t = hijos.get(hijo);
    if(t == NULL)
        return false;
    t->Join();
    return true;
}

OpenFile* UserProg::getOpenFile(int fd)
{
    DEBUG('A', "op: %lu\n", abiertos[fd]);
    if(fd < maxFileDes)
        return abiertos[fd];
    else
        return NULL;
}

