#include "tlbHandler.h"

void tlbHandler::pageFaultHandler(UserProg* prog, int virtualAddress) {
    unsigned vpn = (unsigned) virtAddr / PageSize;
    TranslationEntry* entry = prog->translate(vpn);

    if(!entry->valid) {
        printf("TLB trata de cachear una página inválida (%d)!\n", vpn);
        prog->nisman();
        return;
    }

    // Tengo la entry y es válida!
    // La guardo en el cache.
    for(int i = 0; i < TLBSize; i++) {
        if(!tlb[i].valid) {
            tlb[i] = *entry;
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
