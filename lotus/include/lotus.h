#ifndef LOTUS_H
#define LOTUS_H

#include "version.h"
#include "../lotus.core/defines.h"
#include "../lotus.core/lotustypes.h"
#include "../lotus.core/utility/ltmath.h"
#include "../lotus.core/utility/ltarray.h"
#include "../lotus.core/utility/lthashmap.h"
#include "../lotus.core/graphics/gl/ltglapi.h"

typedef struct tagLotus {
    b8 init;
    char version[8];
    void* renderState;
    void* platformState;
} Lotus, LotusEngine, Lotus_s;

static Lotus_s Engine;

LOTUS_API_ENTRY b8 lotusInit(void);
LOTUS_API_ENTRY b8 ltPumpEvents(void);
LOTUS_API_ENTRY void ltSwapBuffers(void);
LOTUS_API_ENTRY void lotusExit(void);

LOTUS_API_ENTRY void ltClearColor(void);
LOTUS_API_ENTRY void ltSetClearColor(f32 r, f32 g, f32 b, f32 a);

LOTUS_API_ENTRY void ltSetShader(LTshaderProgram* shader);
LOTUS_API_ENTRY void ltSetTexture2D(LTtexture2D* texture);

LOTUS_API_ENTRY void ltDraw(LTvertexData* data);
LOTUS_API_ENTRY void ltDrawArrays(LTvertexData* data);
LOTUS_API_ENTRY void ltDrawIndexed(LTvertexData* data, void* offset);

#endif