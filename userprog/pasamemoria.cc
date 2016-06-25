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

void readStrFromUsrSegura(int usrAddr, char *outStr, int size)
{
    int i = 0, read;
    machine->ReadMem(usrAddr, 1, &read);
    while(read != '\0' && i < size - 1)
    {
        outStr[i] = read;
        i++;
        machine->ReadMem(usrAddr + i, 1, &read);
    }
    outStr[i] = '\0';
}

void writeStrToUsr(char *str, int usrAddr)
{
	do
	{
		machine->WriteMem(usrAddr, 1, *str);
		usrAddr++;
		str++;
	}
	while(*str != '\0');
}

void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount)
{
    for(int i = 0; i < byteCount; i++)
        machine->ReadMem(usrAddr + i, 1, (int*)(outBuff + i));
}

void writeBuffToUsr(char *str, int usrAddr, int byteCount)
{
    for(int i = 0; i < byteCount; i++)
        machine->WriteMem(usrAddr + i, 1, str[i]);
}
