#ifndef LOTUS_H
#define LOTUS_H

#include "version.h"
#include "../lotus.core/lotustypes.h"
#include "../lotus.core/defines.h"
#include "../lotus.core/gl/ltglapi.h"
#include "../lotus.core/utility/ltmath.h"
#include "../lotus.core/utility/ltarray.h"
#include "../lotus.core/utility/lthashmap.h"

typedef struct tagLotus {
    b8 init;
    char version[8];
    void* renderState;
    void* platformState;
} Lotus, LotusEngine, Lotus_s;

static Lotus_s Engine;

LOTUS_API b8 lotusInit(void);
LOTUS_API b8 ltPumpEvents(void);
LOTUS_API void ltSwapBuffers(void);
LOTUS_API void lotusExit(void);

LOTUS_API void ltClearColor(void);
LOTUS_API void ltSetClearColor(f32 r, f32 g, f32 b, f32 a);

LOTUS_API void ltSetShader(LTshaderProgram* shader);
LOTUS_API void ltSetTexture2D(LTtexture2D* texture);

LOTUS_API void ltDraw(LTvertexData* data);
LOTUS_API void ltDrawArrays(LTvertexData* data);
LOTUS_API void ltDrawIndexed(LTvertexData* data, void* offset);

#endif