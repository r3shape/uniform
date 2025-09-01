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
    UFPSOAInterface psoa;

    UFLibrary gpuLibrary;
    UFLibrary appLibrary;
    UFApplication* application;
} Unicorn = {0};


u8 ufInitRuntime(CString library) {
    if (!ufInitOS(&Unicorn.os)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to initialize `UFOS` API\n");
        return 0;
    }

    if (!ufInitEVIN(&Unicorn.evin)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to initialize `UFEVIN` API\n");
        return 0;
    }

    if (!ufInitPSOA(&Unicorn.psoa)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to initialize `UFPSOA` API\n");
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

    /* validate that the `UFApplication` methods are non-null values */
    if (!Unicorn.application->init_hook   ||
        !Unicorn.application->exit_hook   ||
        !Unicorn.application->update_hook ||
        !Unicorn.application->render_hook) {
        r3_log_stdout(WARN_LOG, "[Unicorn] Incomplete `UFApplication` dynamic library loaded -- Application missing function pointers\n");
        return 0;
    }

    /* retrieve `UFApplication` pointer from export symbol and configure Unicorn/UFCORE */
    Unicorn.application = ((ufExport)export)();
    Unicorn.os.newWindow(
        (u32)VEC_X(Unicorn.application->windowPos),
        (u32)VEC_Y(Unicorn.application->windowPos),
        (u32)VEC_X(Unicorn.application->windowSize),
        (u32)VEC_Y(Unicorn.application->windowSize),
        Unicorn.application->api
    );

    // assign application pointers to pre-initialized UFGPU,UFMRG,UFEVIN,UFPSOA interfaces
    Unicorn.application->gpuPtr = &Unicorn.gpu;
    Unicorn.application->mrgPtr = &Unicorn.mrg;
    Unicorn.application->evinPtr = &Unicorn.evin;
    Unicorn.application->psoaPtr = &Unicorn.psoa;

    // initialize the MRG and GPU APIs after a window has been created ( OGL context needs a window first :| )
    if (!ufInitMRG(Unicorn.application->api, &Unicorn.gpu, &Unicorn.mrg)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFMRG` API\n");
        return 0;
    }

    Unicorn.init = 1;

    r3_log_stdout(SUCCESS_LOG, "[Unicorn] Initialized\n");
    return 1;
}

none ufExitRuntime(none) {
    if (!ufExitOS(&Unicorn.os)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFOS` API\n");
    }

    if (!ufExitEVIN(&Unicorn.evin)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFEVIN` API\n");
    }

    if (!ufExitPSOA(&Unicorn.psoa)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFPSOA` API\n");
    }

    // calls ufExitGPU
    if (!ufExitMRG(&Unicorn.mrg)) {
        r3_log_stdout(ERROR_LOG, "[Unicorn] Failed to deinitialize `UFMRG` API\n");
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
        
        Unicorn.os.swapBuffers();
    } while (Unicorn.init);
    Unicorn.application->exit_hook();

    ufExitRuntime();
    return 0;
}
