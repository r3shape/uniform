#ifndef _UFAPP_H_
#define _UFAPP_H_

#define _UF_R3DEF_
#include <ufcore/UFDEF.h>

typedef struct UFApplication {
    UFVersion version;
    UFGpuApi api;
    UFGpuCtx ctx;

    UF_API_METHOD(ptr, alloc, u64 size, u8 align);
    UF_API_METHOD(none, dealloc, ptr memory);
} UFApplication;

#endif // _UFAPP_H_
