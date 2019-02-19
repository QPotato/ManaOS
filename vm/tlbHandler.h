#include "copyright.h"
#include "machine.h"
#include "userprog.h"
#include <stdlib.h>

namespace tlbHandler {
    void pageFaultHandler(UserProg* prog, int virtualAddress);
    void readOnlyHandler(UserProg* prog, int virtualAddress);
    void busErrorHandler(UserProg* prog, int virtualAddress);
}
