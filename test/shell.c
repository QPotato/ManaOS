#include "syscall.h"

//
//Para poder hacer Write de un int...
//

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

int
main()
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
            char aId[100];
            Write("calling...\n", 20, output);
            SpaceId id = Exec(buffer);
            int len = myItoa(id, aId);
            Write(aId, len, output);
            Write("\n", 1, output);
        }
    }
}


