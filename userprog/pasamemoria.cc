#include "system.h"

void readStrFromUsr(int usrAddr, char *outStr)
{
	do
	{
		machine->ReadMem(usrAddr, 1, (int*)outStr);
		usrAddr++;
		outStr++;
	}
	while(*outStr != '\0');
}

void readStrFromUsrSegura(int usrAddr, char *outStr, int size)
{
    int i = 0;
	do
	{
		machine->ReadMem(usrAddr, 1, (int*)outStr);
		usrAddr++;
		outStr++;
		i++;
	}
	while(*outStr != '\0' && i < size);
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
