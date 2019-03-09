#include "swap.h"

Swap::Swap(char *asid, int addrSpaceSz) {
    sprintf(fileName, "SWAP.%s", asid);

    // Suponemos que siempre vamos a poder crear un archivo de swap!
    ASSERT(fileSystem->Create(fileName, addrSpaceSz));

    swapFile = fileSystem->Open(fileName);
}

Swap::~Swap() {
    delete swapFile;
    fileSystem->Remove(fileName);
}

void Swap::swapIn(unsigned vpn, unsigned physPage) {
    // Escribimos una pagina del swap a la memoria física
    DEBUG('S', "Traigo la pagina %d desde swap a marco fisico %d\n", vpn, physPage);
    char swapData[PageSize];

    int rdSize = swapFile->ReadAt(swapData, PageSize, vpn * PageSize);
    for (int i = 0; i < rdSize; i++)
    {
        int dirFisica = physPage * PageSize + i;
        machine->mainMemory[dirFisica] = swapData[i];
    }

    return;
}

void Swap::swapOut(unsigned vpn, unsigned physPage) {
    // Escribimos una página de memoria física al swap
    DEBUG('S', "Escribo el marco fisico %d (pagina virtual %d) al swap\n", physPage, vpn);
    char swapData[PageSize];
    for (unsigned int i = 0; i < PageSize; i++)
    {
        int dirFisica = physPage * PageSize + i;
        swapData[i] = machine->mainMemory[dirFisica];
    }

    swapFile->WriteAt(swapData, PageSize, vpn * PageSize);
    
    return;
}
