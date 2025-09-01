#include <ufcore/UFEVIN.h>

u8 ufInitEVIN(UFEVINInterface* evinInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFEVIN] Initialized\n");
    return 1;
}

u8 ufExitEVIN(UFEVINInterface* evinInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFEVIN] Deinitialized\n");
    return 1;
}

