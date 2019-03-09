#include <stdlib.h>
#include "coremap.h"

CoreMapEntry::CoreMapEntry() {
    addrSpace = NULL;
    vpn = 0;
    timestamp = 0;
}

CoreMapEntry::CoreMapEntry(AddrSpace* addrSpace, unsigned vpn, int timestamp) {
    addrSpace = addrSpace;
    vpn = vpn;
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
    entries[physPage] = CoreMapEntry(addrSpace, vpn, timestamp);
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