#include "system.h"

void readStrFromUsr(int usrAddr, char *outStr)
{
    int i = 0, read;
    machine->ReadMem(usrAddr, 1, &read);
    while(read != '\0')
    {
        outStr[i] = read;
        i++;
        machine->ReadMem(usrAddr + i, 1, &read);
    }
    outStr[i] = '\0';
}

unsigned readStrFromUsrSegura(int usrAddr, char *outStr, int size)
{
    unsigned i = 0;
    int read;
    machine->ReadMem(usrAddr, 1, &read);
    while(read != '\0' && i < (unsigned)size - 1)
    {
        outStr[i] = read;
        i++;
        machine->ReadMem(usrAddr + i, 1, &read);
    }
    outStr[i] = '\0';
    return i;
}

void writeStrToUsr(char *str, int usrAddr)
{
    for(;*str != '\0'; usrAddr++, str++)
		machine->WriteMem(usrAddr, 1, *str);
	machine->WriteMem(usrAddr, 1 , '\0');
}

void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount)
{
    int tmp;
    for(int i = 0; i < byteCount; i++) {
        machine->ReadMem(usrAddr + i, 1, &tmp);
        outBuff[i] = tmp;
    }
}

void writeBuffToUsr(char *str, int usrAddr, int byteCount)
{
    for(int i = 0; i < byteCount; i++)
        machine->WriteMem(usrAddr + i, 1, str[i]);
}
