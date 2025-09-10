#ifndef _UFOS_H_
#define _UFOS_H_

#define _UF_R3DEF_
#include <ufcore/UFDEF.h>

typedef enum UFWindowFlag {
    UF_WINDOW_FOCUSED       = (1 << 0),
    UF_WINDOW_SHOW_CURSOR   = (1 << 1),
    UF_WINDOW_BIND_CURSOR   = (1 << 2),
    UF_WINDOW_CENTER_CURSOR = (1 << 3)
} UFWindowFlag;

typedef struct UFLibrary {
    char* name;
    ptr handle;
} UFLibrary;

typedef struct UFOSInterface {
    UFVersion ver;

    UF_API_METHOD(UFResource, newWindow, u32 x, u32 y, u32 w, u32 h, UFGpuApi api);
    UF_API_METHOD(u8, delWindow, UFResource window);
    
    UF_API_METHOD(none, setWindowFlag, UFWindowFlag flag);
    UF_API_METHOD(none, getWindowFlag, UFWindowFlag flag);
    UF_API_METHOD(none, remWindowFlag, UFWindowFlag flag);

    UF_API_METHOD(none, getEvents, none);
    UF_API_METHOD(none, getInputs, none);

    UF_API_METHOD(none, swapBuffers, UFResource window);

    // refers to UFOSInternal's UFWindow.api
    UF_API_METHOD(u8, newGpuCtx, UFResource window);
    UF_API_METHOD(u8, delGpuCtx, UFResource window);

    UF_API_METHOD(u8, loadLibrary, char* path, char* name, UFLibrary* library);
    UF_API_METHOD(u8, loadSymbol, char* name, ptr* symbol, UFLibrary* library);
    UF_API_METHOD(u8, unloadLibrary, UFLibrary* library);
} UFOSInterface;

#endif // _UFOS_H_
