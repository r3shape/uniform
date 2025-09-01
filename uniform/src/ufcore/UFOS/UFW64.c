#include <ufcore/UFOS.h>

u8 ufInitOS(UFOSInterface* osInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFOS] Initialized\n");
    return 1;
}


u8 ufExitOS(UFOSInterface* osInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFOS] Deinitialized\n");
    return 1;
}
