#ifdef USE_TLB
#include <stdlib.h>
#include "coremap.h"

CoreMapEntry::CoreMapEntry() {
    addrSpace = NULL;
    vpn = 0;
    physPage = 0;
    timestamp = 0;
}

CoreMapEntry::CoreMapEntry(AddrSpace* addrSpace, unsigned vpn, unsigned physPage, int timestamp) {
    addrSpace = addrSpace;
    vpn = vpn;
    physPage = phyPage
    timestamp = timestamp;
}

CoreMap::CoreMap() {
    timestamp = 0;

    for(int i = 0; i < NumPhysPages; i++) {
        entries[i] = CoreMapEntry();
    }
}

CoreMap::~CoreMap() {}

void CoreMap::savePage(AddrSpace* addrSpace, int vpn, int physPage) {
    ASSERT(physPage < NumPhysPages); // Te pasaste de página física!!
    ASSERT(addrSpace != NULL); // el addrSpace es NULL!!

    DEBUG('S', "Guardando pagina virtual %d del addrSpace %s en pagina fisica %d\n", vpn, addrSpace->thread->getName(), physPage);

    timestamp++;
    entries[physPage] = CoreMapEntry(addrSpace, vpn, physPage, timestamp);
}

void CoreMap::freeSpaceEntries(AddrSpace* addrSpace){
    for(int i = 0; i < NumPhysPages; i++) {
        if(entries[i].addrSpace == addrSpace)
            entries[i] = CoreMapEntry()
    }
}

CoreMapEntry CoreMap::getNextToSwap() {
    return randomSwapPolicy();
}

CoreMapEntry CoreMap::randomSwapPolicy() {
    int replaceIndex = rand() % NumPhysPages;
    return entries[replaceIndex];
}

CoreMapEntry CoreMap::LRUSwapPolicy() {
    CoreMapEntry LRU = entries[0];
    for(int i = 0; i < NumPhysPages; i++) {
        if(entries[i].timestamp < LRU.timestamp) {
            LRU = entries[i];
        }
    }

    return LRU;
}
#endif