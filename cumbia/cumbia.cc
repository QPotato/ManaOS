void readStrFromUsr(int usrAddr, char *outStr)
{
	do
	{
		ReadMem(usrAddr, 1, outStr);
		usrAddr++;
		outStr++;
	}
	while(*outStr != '\0')
}

void writeStrToUsr(char *str, int usrAddr)
{
	do
	{
		WriteMem(usrAddr, 1, *Str);
		usrAddr++;
		Str++;
	}
	while(*Str != '\0')
}

void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount)
{
	for(int i = 0; i < byteCount; i++)
		ReadMem(usrAddr + i, 1, outStr + i);
}

void writeBuffToUsr(char *str, int usrAddr, int byteCount)
{
	for(int i = 0; i < byteCount; i++)
		WriteMem(usrAddr + i, 1, Str[i]);
}