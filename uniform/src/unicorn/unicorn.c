#define _UF_R3DEF_
#include <unicorn/unicorn.h>
#include <ufcore/UFCORE.h>
#include <uniform.h>

struct Unicorn {
    u8 init;
    UFVersion ver;

    UFOSInterface os;
    UFGPUInterface gpu;
    UFMRGInterface mrg;
    UFEVINInterface evin;

    UFLibrary gpuLibrary;
    UFLibrary appLibrary;
    UFApplication* application;
} Unicorn = {0};


none _ufDefaultExitHook(UFEventCode code, UFEvent event) {
    Unicorn.init = 0;
    r3_log_stdout(INFO_LOG, "[Unicorn] Exiting Uniform Runtime\n");
}


u8 ufInitRuntime(CString library) {
    if (!ufInitEVIN(&Unicorn.evin)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to initialize `UFEVIN` API\n");
        return 0;
    }

    if (!ufInitOS(&Unicorn.evin, &Unicorn.os)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to initialize `UFOS` API\n");
        return 0;
    }

    /* use UFOS to load the UFApplication dynamic library and resolve the export symbol */
    Unicorn.os.loadLibrary(UF_DEFAULT_USER_PATH, library, &Unicorn.appLibrary);
    if (!Unicorn.appLibrary.handle) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to load `UFApplication` dynamic library\n");
        return 0;
    }

    ptr export;
    Unicorn.os.loadSymbol("ufExport", &export, &Unicorn.appLibrary);
    if (!export) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to load `ufExport` symbol from `UFApplication` dynamic library\n");
        return 0;
    }

    /* retrieve `UFApplication` pointer from export symbol and configure Unicorn/UFCORE */
    /* validate that the `UFApplication` methods are non-null values */
    Unicorn.application = ((UFExport)export)();
    if (!Unicorn.application->init_hook   ||
        !Unicorn.application->exit_hook   ||
        !Unicorn.application->update_hook ||
        !Unicorn.application->render_hook) {
        r3_log_stdout(WARN_LOG, "[Unicorn] Incomplete `UFApplication` dynamic library loaded -- Application missing function pointers\n");
        return 0;
    }

    /* assign application pointers to pre-initialized UFGPU,UFMRG,UFEVIN,UFPSOA interfaces */
    Unicorn.application->gpuPtr = &Unicorn.gpu;
    Unicorn.application->mrgPtr = &Unicorn.mrg;
    Unicorn.application->evinPtr = &Unicorn.evin;

    /* create and assign a window resource handle to loaded `UFApplication` */
    Unicorn.application->window = Unicorn.os.newWindow(
        (u32)VEC_X(Unicorn.application->windowPos),
        (u32)VEC_Y(Unicorn.application->windowPos),
        (u32)VEC_X(Unicorn.application->windowSize),
        (u32)VEC_Y(Unicorn.application->windowSize),
        Unicorn.application->api
    );
    
    /* initialize the GPU APIs after a window has been created ( OGL context needs a window first :| ) */
    switch (Unicorn.application->api) {
        case UF_GPU_DX_API: {
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Failed to select `UFGPU` API -- unsupported GPU API selected: %d\n", Unicorn.application->api);
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Initializing supported GPU API: %d\n", UF_GPU_GL_API);
        }
        case UF_GPU_VK_API: {
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Failed to select `UFGPU` API -- unsupported GPU API selected: %d\n", Unicorn.application->api);
            r3_log_stdoutf(WARN_LOG, "[UFMRG] Initializing supported GPU API: %d\n", UF_GPU_GL_API);
        }
        case UF_GPU_GL_API: {
            if (!ufInitGPU(Unicorn.application->window, &Unicorn.os, &Unicorn.gpu)) {
                r3_log_stdout(ERROR_LOG, "[UFMRG] Failed to initialize `UFGPU` API -- OpenGL init failed\n");
                return 0;
            }
        } break;
        case UF_GPU_INVALID_API:    // fall through default
        default: {
            r3_log_stdoutf(ERROR_LOG, "[UFMRG] Failed to initialize `UFGPU` API -- invalid GPU API selected: %d\n", Unicorn.application->api);
            return 0;
        }
    }

    if (!ufInitMRG(&Unicorn.gpu, &Unicorn.mrg)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFMRG` API\n");
        return 0;
    }

    // register default event hooks
    Unicorn.evin.newHook(UF_EVENT_EXIT, _ufDefaultExitHook);

    Unicorn.init = 1;
    r3_log_stdout(SUCCESS_LOG, "[Unicorn] Initialized\n");

    return 1;
}

none ufExitRuntime(none) {
    if (!Unicorn.os.unloadLibrary(&Unicorn.appLibrary)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to unload `UFApplication` dynamic library\n");
    }

    if (!ufExitOS(&Unicorn.os)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFOS` API\n");
    }

    if (!ufExitEVIN(&Unicorn.evin)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFEVIN` API\n");
    }

    if (!ufExitMRG(&Unicorn.mrg)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFMRG` API\n");
    }

    if (!ufExitGPU(&Unicorn.gpu)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFGPU` API\n");
    }
    
    Unicorn.init = 0;
    
    r3_log_stdout(SUCCESS_LOG, "[Unicorn] Deinitialized\n");
    return;
}


s32 main(s32 argc, CString* argv) {
    if (!ufInitRuntime(argv[1])) { ufExitRuntime(); }

    Unicorn.application->init_hook();
    do {
        Unicorn.os.getEvents();
        Unicorn.os.getInputs();
        
        Unicorn.application->update_hook();
        Unicorn.application->render_hook();
        
        Unicorn.os.swapBuffers(Unicorn.application->window);
    } while (Unicorn.init);
    Unicorn.application->exit_hook();

    ufExitRuntime();
    return 0;
}
