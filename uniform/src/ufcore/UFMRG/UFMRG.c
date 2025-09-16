#include <ufcore/UFCORE.h>

u8 ufInitMRG(UFGPUInterface* gpuPtr, UFMRGInterface* mrgInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFMRG] Initialized\n");
    return 1;
}

u8 ufExitMRG(UFMRGInterface* mrgInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFMRG] Denitialized\n");
    return 1;
}

