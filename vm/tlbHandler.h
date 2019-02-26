
#include "machine.h"
#include "userprog.h"
#include "system.h"
#include <stdlib.h>

#ifdef USE_TLB

namespace tlbHandler {
    void pageFaultHandler(UserProg* prog, int virtualAddress);
    void readOnlyHandler(UserProg* prog, int virtualAddress);
    void busErrorHandler(UserProg* prog, int virtualAddress);
    void tlbCleaner(UserProg* newUserProg);

    // private crap
    namespace
    {
        UserProg* lastUserProg = NULL;
    }
}

#endif //USE_TLB