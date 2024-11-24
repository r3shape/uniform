#ifndef LTRENDERER_H
#define LTRENDERER_H

#include "../defines.h"
#include "../gl/ltglapi.h"

typedef struct tagRenderState {
    void *internal;
} LTrenderState;

void ltRendererExit(LTrenderState *state);
b8 ltRendererInit(LTrenderState *state, u32 vpWidth, u32 vpHeight);

// State management
void ltRendererSetClearColor(LTrenderState *state, f32 r, f32 g, f32 b, f32 a);
void ltRendererSetViewport(LTrenderState *state, i32 x, i32 y, i32 width, i32 height);

// Drawing
void ltRendererClearColor(void);
void ltRendererDraw(LTrenderState *state);
void ltRendererDrawArrays(LTrenderState *state);
void ltRendererDrawIndexed(LTrenderState *state, void* offset);
void ltRendererSetVertexData(LTrenderState *state, LTvertexData* data);
void ltRendererSetTexture2D(LTrenderState *state, LTtexture2D* texture);
void ltRendererSetShader(LTrenderState *state, LTshaderProgram* program);

#endif