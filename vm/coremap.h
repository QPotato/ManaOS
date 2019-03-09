#ifdef USE_TLB
#include "machine.h"

struct CoreMapEntry {
    AddrSpace* addrSpace;
    unsigned vpn;
    int timestamp;
    int physPage;

    CoreMapEntry();
    CoreMapEntry(AddrSpace* addrSpace, unsigned vpn, int timestamp);
}

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