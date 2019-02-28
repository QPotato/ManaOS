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
            DEBUG('V', "Sabé, encontraste lugar en la TLB!\n");
            return;
        }
    }

    int replace_index = 7;
    DEBUG('V', "Te quedaste sin TLB! Pero no importa, sacamos al %d! Vamos ManaOS!\n", replace_index);
    machine->tlb[replace_index] = *entry;
    return;
}

void tlbHandler::readOnlyHandler(UserProg* prog, int virtualAddress) {
    printf("Segmentation fault, pavo! Vamos ManaOS!\n");
    prog->nisman();
}

void tlbHandler::busErrorHandler(UserProg* prog, int virtualAddress) {
    printf("UUUFFF!! Asignaste un marco fìsico de memoria que no existe! Vamos ManaOS!\n");
    ASSERT(false);
}

void tlbHandler::tlbCleaner(UserProg* newUserProg) {
    // Actualizo, si es necesario, el lastUserProg
    if(newUserProg == NULL) {
        return;
    }

    if(tlbHandler::lastUserProg != newUserProg) {
        DEBUG('V', "Limpio la TLB. Vamos ManaOS!\n");
        for(int i = 0; i < TLBSize; i++) {
            machine->tlb[i].valid = false;
        }
        tlbHandler::lastUserProg = newUserProg;
        return;
    }

    return;
}

#endif
