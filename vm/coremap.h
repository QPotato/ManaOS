
#ifndef COREMAP_H
#define COREMAP_H

#ifdef USE_TLB
#include "machine.h"
#include "addrspace.h"

struct CoreMapEntry {
    AddrSpace* addrSpace;
    unsigned vpn;
    int timestamp;
    int physPage;

    CoreMapEntry();
    CoreMapEntry(AddrSpace* _addrSpace, unsigned _vpn, unsigned _physPage, int _timestamp);
};

class CoreMap {
    public:
        CoreMap();
        ~CoreMap();

        void savePage(AddrSpace* addrSpace, int vpn, int physPage);
        void freeSpaceEntries(AddrSpace* addrSpace);
        CoreMapEntry getNextToSwap();
    private:
        unsigned int timestamp;
        CoreMapEntry entries[NumPhysPages];

        CoreMapEntry randomSwapPolicy();
        CoreMapEntry LRUSwapPolicy();
};

#endif
#endif