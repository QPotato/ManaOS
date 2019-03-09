// addrspace.cc
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option
//	2. run coff2noff to convert the object file to ManaOS format
//		(ManaOS object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the ManaOS file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "machine.h"

#include "system.h"
#include "addrspace.h"
#include "pasamemoria.h"
#ifdef USE_TLB
#include "tlbHandler.h"
#endif

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader(NoffHeader *noffH)
{
    noffH->noffMagic = WordToHost(noffH->noffMagic);
    noffH->code.size = WordToHost(noffH->code.size);
    noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
    noffH->initData.size = WordToHost(noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost(noffH->uninitData.size);
    noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace(OpenFile *executable)
{
    progExecutable = executable;
    unsigned int size;

    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
        (WordToHost(noffH.noffMagic) == NOFFMAGIC))
        SwapHeader(&noffH);
    ASSERT(noffH.noffMagic == NOFFMAGIC);

    // how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize; // we need to increase the size to leave room for the stack
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    ASSERT(numPages <= NumPhysPages); // check we're not trying
                                      // to run anything too big --
                                      // at least until we have
                                      // virtual memory

    DEBUG('a', "Initializing address space, num pages %d, size %d\n",
          numPages, size);

    // first, set up the translation
    pageTable = new TranslationEntry[numPages];
    for (int i = 0; i < numPages; i++)
    {
        pageTable[i].virtualPage = i;
        pageTable[i].physicalPage = -1;
        pageTable[i].valid = false;
        pageTable[i].use = false;
        pageTable[i].dirty = false;
        pageTable[i].readOnly = false; // if the code segment was entirely on
                                       // a separate page, we could set its
                                       // pages to be read-only
    }

}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    for (unsigned i = 0; i < numPages; i++)
    {
        if(pageTable[i].physicalPage >= 0) {
            memoryManager->liberarPagina(pageTable[i].physicalPage);
        }
    }
    delete pageTable;
    delete progExecutable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
        machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//----------------------------------------------------------------------

void AddrSpace::SaveState()
{
#ifndef USE_TLB
    numPages = machine->pageTableSize;
#else
    // ???
#endif
    //pageTable = machine->pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState()
{
#ifndef USE_TLB
    DEBUG('A', "Seteando la pageTable de %ld. Vamos ManOS.\n", pageTable);
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
#else
    tlbHandler::tlbCleaner(currentThread->userProg);
#endif
}

#ifdef USE_TLB
TranslationEntry *AddrSpace::translate(int vpn)
{
    ASSERT((unsigned)vpn < numPages); // Pediste una pagina fuera de la tabla de paginacion!
    TranslationEntry* entry = &pageTable[vpn];
    
    if(entry->valid) {
        DEBUG('D', "[TRANSLATE]: la entry para la página %d ya era válida!\n", vpn);
        return entry;
    }

    ASSERT(entry->physicalPage < 0);

    entry->valid = true;
    entry->physicalPage = memoryManager->alocarPagina(this, vpn);

    int virtualPageStart = vpn * PageSize;

    int codeStart = noffH.code.virtualAddr;
    int codeEnd = codeStart + noffH.code.size;
    if(virtualPageStart >= codeStart && virtualPageStart < codeEnd) {
        // Esto es código!!
        DEBUG('D', "[TRANSLATE]: la entry para la página %d parece ser código, cargarlo...\n", vpn);
        entry->readOnly = true;
        
        char datosExe[PageSize];
        unsigned int rdSize;

        int inSegmentOffset = virtualPageStart - codeStart;

        // Escribimos una pagina leida de los datos del ejecutable.
        rdSize = progExecutable->ReadAt(datosExe, PageSize, noffH.code.inFileAddr + inSegmentOffset);
        for (unsigned int i = 0; i < rdSize; i++)
        {
            // Escribimos el byte i de la pagina (s / PageSize) del ejecutable.
            int dirVirtual = noffH.code.virtualAddr + inSegmentOffset + i;
            int pagFisica = entry->physicalPage;
            int dirFisica = pagFisica * PageSize + dirVirtual % PageSize;
            machine->mainMemory[dirFisica] = datosExe[i];
        }
        
        return entry;
    }

    int initDataStart = noffH.initData.virtualAddr;
    int initDataEnd = codeStart + noffH.initData.size;
    if(virtualPageStart >= initDataStart && virtualPageStart < initDataEnd) {
        // Esto es initData!!
        DEBUG('D', "[TRANSLATE]: la entry para la página %d parece ser del segmento de datos, cargarlo...\n", vpn);
        entry->readOnly = true;
        
        char datosExe[PageSize];
        unsigned int rdSize;

        int inSegmentOffset = virtualPageStart - codeStart;

        // Escribimos una pagina leida de los datos del ejecutable.
        rdSize = progExecutable->ReadAt(datosExe, PageSize, noffH.initData.inFileAddr + inSegmentOffset);
        for (unsigned int i = 0; i < rdSize; i++)
        {
            // Escribimos el byte i de la pagina (s / PageSize) del ejecutable.
            int dirVirtual = noffH.initData.virtualAddr + inSegmentOffset + i;
            int pagFisica = entry->physicalPage;
            int dirFisica = pagFisica * PageSize + dirVirtual % PageSize;
            machine->mainMemory[dirFisica] = datosExe[i];
        }

        return entry;
    }

    int uninitDataStart = noffH.uninitData.virtualAddr;
    int uninitDataEnd = codeStart + noffH.uninitData.size;
    //ASSERT(virtualPageStart >= uninitDataStart && virtualPageStart < uninitDataEnd);

    DEBUG('D', "[TRANSLATE]: la entry para la página %d parece ser stack.\n", vpn);

    return entry;
}
#endif
