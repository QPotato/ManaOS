#include "syscall.h"

//
//Para poder hacer Write de un int...
//

int mystrlen(char* in)
{
    int len = 0;
    while(in[len] != '\0')
        len++;
    return len;
}

int min(int a, int b)
{
    return (a < b ? a : b);
}

void mystrncat(char *dest, char *src, unsigned max)
{
    Write("me llamaron\n", 12, ConsoleOutput);
    int i, limit, len = mystrlen(dest);
    limit = min(mystrlen(dest) + mystrlen(src), max);
    for(i = len; i < limit; i++)
    {
        dest[i] = src[i - len];
    }
    if(i < max)
        dest[i] = '\0';
    else
        dest[max-1] = '\0';

    return;
}

int pow(int a, int b)
{
    int i;
    int ret = 1;
    for(i = 0; i < b; i++)
    {
        ret *= a;
    }
    return ret;
}

//El n-esimo digito de a
char getNthDigit(int a, int n)
{
    return ((int)((a % pow(10, n + 1)) / pow(10, n))) + '0';
}

//devuelvo el largo de la cadena porque puedo
int myItoa(int a, char *s)
{
    char inv[100];
    int i, j;

    //0 es un caso especial por algun motivo
    if(a == 0)
    {
        s[0] = '0';
        s[1] = '\0';
        return 1;
    }
    
    //obtengo el arreglo de digitos invertido
    for(i = 0; a >= pow(10, i); i++)
        inv[i] = getNthDigit(a, i);
    inv[i] = '\0';

    //doy vuelta el arreglo
    for(j = 0; j < i; j++)
        s[j] = inv[i - j - 1];
    s[j] = '\0';

    return j;
}

//
// MAIN     TODO: Llevar todo lo anterior a un .h
//

void handleCmd(char*);

int main()
{
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char prompt[2], buffer[60];
    int i;

    prompt[0] = '-';
    prompt[1] = '-';

    while(1)
    {
        Write(prompt, 2, output);

        i = 0;
    
        do
        {
            Read(&buffer[i], 1, input); 
        }
        while( buffer[i++] != '\n' );

        buffer[--i] = '\0';

        if(i > 0)
        {
            handleCmd(buffer);
        }
    }
}

void handleCmd(char *buf)
{
    int background = 0;
    if(buf[0] == '&')
    {
        background = 1;
        buf += 1;
    }

    SpaceId id = execProgram(buf, background);
    if(!background && id != -1)
    {
        Join(id);
    }
}

SpaceId execProgram(char *buf, int back)
{
    SpaceId ret;
    if(buf[0] == '.')
    {
        //ejecuto programa de usuario (./algo)
        ret = Exec(buf, !back);
    }
    else
    {
        //ejecuto programa utilitario (cat, cp, etc...) TODO: implementar
        char path[MAX_NOMBRE];
        path[0] = '\0';
        Write("strcat empieza\n", 16, ConsoleOutput);
        mystrncat(path, "../test/", MAX_NOMBRE);
        mystrncat(path, buf, MAX_NOMBRE);
        Write("strcat ended\n", 14, ConsoleOutput);
        ret = Exec(path, !back);
    }
    return ret;
}
