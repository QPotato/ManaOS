#include "tlbHandler.h"
#ifdef USE_TLB

void tlbHandler::pageFaultHandler(UserProg* prog, int virtualAddress) {
    unsigned vpn = (unsigned) virtualAddress / PageSize;
    TranslationEntry* entry = prog->translate(vpn);

    if(!entry->valid) {
        printf("TLB trata de cachear una página inválida (%d)!\n", vpn);
        prog->nisman();
        return;
    }

    // Tengo la entry y es válida!
    // La guardo en el cache.
    for(int i = 0; i < TLBSize; i++) {
        if(!machine->tlb[i].valid) {
            machine->tlb[i] = *entry;
            DEBUG('A', "Sabé, encontraste lugar en la TLB!\n");
            return;
        }
    }

    printf("Te quedaste sin TLB! Vamos ManaOS!\n");
    ASSERT(false); // Nos quedamos sin TLB!
}

void tlbHandler::readOnlyHandler(UserProg* prog, int virtualAddress) {
    printf("Segmentation fault, pavo! Vamos ManaOS!\n");
    prog->nisman();
}

void tlbHandler::busErrorHandler(UserProg* prog, int virtualAddress) {
    printf("UUUFFF!! Asignaste un marco fìsico de memoria que no existe! Vamos ManaOS!\n");
    ASSERT(false);
}

#endif