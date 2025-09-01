#ifndef _UF_H_
#define _UF_H_

#include <ufcore/UFMRG.h>
#include <ufcore/UFEVIN.h>
#include <ufcore/UFPSOA.h>

// returned as a pointer from user code via the `ufExport` symbol
// Unicorn will then have access to the entire application,
// whilst the user recieves a UnicornInterface pointer with UFMRG, UFEVIN, UFGPU pointers
typedef struct UFApplication {
    UFVersion ver;
    UFGpuApi api;
    UFGpuCtx ctx;
    
    Vec2 windowPos;
    Vec2 windowSize;
    CString windowTitle;

    UFGPUInterface* gpuPtr;
    UFMRGInterface* mrgPtr;
    UFEVINInterface* evinPtr;
    UFPSOAInterface* psoaPtr;

    UF_API_METHOD(none, init_hook, none);
    UF_API_METHOD(none, exit_hook, none);
    UF_API_METHOD(none, update_hook, none);
    UF_API_METHOD(none, render_hook, none);
} UFApplication;

typedef UFApplication* (*ufExport)(none);

#endif // _UF_H_
