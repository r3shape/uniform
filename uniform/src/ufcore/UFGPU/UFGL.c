#include <ufcore/UFGPU/UFGPU.h>
#include <ufcore/UFGPU/UFGL.h>

u8 ufInitGPU(UFGPUInterface* gpuInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFGPU] Initialized\n");
    return 1;
}

u8 ufExitGPU(UFGPUInterface* gpuInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFGPU] Deinitialized\n");
    return 1;
}

