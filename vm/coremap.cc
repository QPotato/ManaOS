#ifdef USE_TLB
#include <stdlib.h>
#include "coremap.h"

CoreMapEntry::CoreMapEntry() {
    this->addrSpace = NULL;
    this->vpn = 0;
    this->physPage = 0;
    this->timestamp = 0;
}

CoreMapEntry::CoreMapEntry(AddrSpace* _addrSpace, unsigned _vpn, unsigned _physPage, int _timestamp) {
    this->addrSpace = _addrSpace;
    this->vpn = _vpn;
    this->physPage = _physPage;
    this->timestamp = _timestamp;
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

    DEBUG('S', "Guardando pagina virtual %d del addrSpace %s en pagina fisica %d\n", vpn, addrSpace->asid, physPage);

    timestamp++;
    entries[physPage] = CoreMapEntry(addrSpace, vpn, physPage, timestamp);
}

void CoreMap::freeSpaceEntries(AddrSpace* addrSpace){
    for(int i = 0; i < NumPhysPages; i++) {
        if(entries[i].addrSpace == addrSpace)
            entries[i] = CoreMapEntry();
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