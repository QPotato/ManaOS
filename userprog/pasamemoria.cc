#include "system.h"

void readStrFromUsr(int usrAddr, char *outStr)
{
    int i = 0, read;
    machine->ReadMem(usrAddr, 1, &read);
    while(read != '\0')
    {
        outStr[i] = read;
        i++;
        while( !machine->ReadMem(usrAddr + i, 1, &read) ) {
            DEBUG('K', "fallo en readMem! Reintento...\n");
        }
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
        while( !machine->ReadMem(usrAddr + i, 1, &read) ) {
            DEBUG('K', "fallo en readMem! Reintento...\n");
        }
    }
    DEBUG('V', "Terminé de readStrFromUsrSegura. Vamos ManaOS!\n");
    outStr[i] = '\0';
    return i;
}

void writeStrToUsr(char *str, int usrAddr)
{
    for(;*str != '\0'; usrAddr++, str++) {
		while( !machine->WriteMem(usrAddr, 1, *str) ) {
            DEBUG('K', "fallo en readMem! Reintento...\n");
        }
    }
	while( !machine->WriteMem(usrAddr, 1 , '\0') ) {
        DEBUG('K', "fallo en readMem! Reintento...\n");
    }
}

void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount)
{
    int tmp;
    for(int i = 0; i < byteCount; i++) {
        while( !machine->ReadMem(usrAddr + i, 1, &tmp) ) {
            DEBUG('K', "fallo en readMem! Reintento...\n");
        }
        outBuff[i] = tmp;
    }
}

void writeBuffToUsr(char *str, int usrAddr, int byteCount)
{
    for(int i = 0; i < byteCount; i++) {
        while( !machine->WriteMem(usrAddr + i, 1, str[i]) ) {
            DEBUG('K', "fallo en readMem! Reintento...\n");
        }
    }
}
