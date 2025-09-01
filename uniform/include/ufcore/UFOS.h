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

typedef struct UFWindow {
    u32 flags;
    u32 x; u32 y;
    u32 w; u32 h;
    UFGpuApi api;
} UFWindow;

typedef struct UFLibrary {
    char* name;
    ptr handle;
} UFLibrary;

typedef struct UFOSInterface {
    UFVersion ver;

    UF_API_METHOD(none, newWindow, u32 x, u32 y, u32 w, u32 h, UFGpuApi api);
    UF_API_METHOD(none, delWindow, none);
    
    UF_API_METHOD(none, setWindowFlag, UFWindowFlag flag);
    UF_API_METHOD(none, getWindowFlag, UFWindowFlag flag);
    UF_API_METHOD(none, remWindowFlag, UFWindowFlag flag);

    UF_API_METHOD(none, getEvents, none);
    UF_API_METHOD(none, getInputs, none);

    UF_API_METHOD(none, swapBuffers, none);

    // refers to UFOSInternal's UFWindow.api
    UF_API_METHOD(none, newGpuCtx, none);
    UF_API_METHOD(none, delGpuCtx, none);

    UF_API_METHOD(none, loadLibrary, char* path, char* name, UFLibrary* library);
    UF_API_METHOD(none, loadSymbol, char* name, ptr* symbol, UFLibrary* library);
    UF_API_METHOD(none, unloadLibrary, UFLibrary* library);
} UFOSInterface;

#endif // _UFOS_H_
