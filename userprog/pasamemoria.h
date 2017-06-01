void readStrFromUsr(int usrAddr, char *outStr);
unsigned readStrFromUsrSegura(int usrAddr, char *outStr, int size);
void readBuffFromUsr(int usrAddr, char *outBuff, int byteCount);
void writeStrToUsr(char *str, int usrAddr);
void writeBuffToUsr(char *str, int usrAddr, int byteCount);
