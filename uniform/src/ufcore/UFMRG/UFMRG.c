#include <ufcore/UFCORE.h>

u8 ufInitMRG(UFResource window, UFGpuApi api, UFGPUInterface* gpuPtr, UFOSInterface* osPtr, UFMRGInterface* mrgInterface) {
    switch (api) {
        case UF_GPU_GL_API: {
            if (!ufInitGPU(window, osPtr, gpuPtr)) {
                r3_log_stdout(ERROR_LOG, "[UFMRG] Failed to initialize `UFGPU` API -- OpenGL init failed\n");
                return 0;
            }
        } break;
        case UF_GPU_DX_API: {
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Failed to select `UFGPU` API -- unsupported GPU API selected: %d\n", api);
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Initializing supported GPU API: %d\n", UF_GPU_GL_API);
            ufInitMRG(window, UF_GPU_GL_API, gpuPtr, osPtr, mrgInterface);
        } break;
        case UF_GPU_VK_API: {
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Failed to select `UFGPU` API -- unsupported GPU API selected: %d\n", api);
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Initializing supported GPU API: %d\n", UF_GPU_GL_API);
            ufInitMRG(window, UF_GPU_GL_API, gpuPtr, osPtr, mrgInterface);
        } break;
        case UF_GPU_INVALID_API:
        default: {
            r3_log_stdoutf(ERROR_LOG, "[UFMRG] Failed to initialize `UFGPU` API -- invalid GPU API selected: %d\n", api);
            return 0;
        }
    }

    r3_log_stdout(SUCCESS_LOG, "[UFMRG] Initialized\n");
    return 1;
}

u8 ufExitMRG(UFMRGInterface* mrgInterface) {
    r3_log_stdout(SUCCESS_LOG, "[UFMRG] Denitialized\n");
    return 1;
}

