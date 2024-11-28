#include <stdio.h>

#include "../include/lotus.h"

#include "memory/ltmemory.h"
#include "platform/ltlogger.h"
#include "platform/ltplatform.h"
#include "platform/event/ltevent.h"
#include "platform/input/ltinput.h"
#include "graphics/renderer/ltrenderer.h"

// defined at the application-level
void* ltGetApplication(void);
void* ltGetRenderState(void);
void* ltGetPlatformState(void);

void ltSetClearColor(f32 r, f32 g, f32 b, f32 a) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetClearColor(renderState, r, g, b, a);
}

void ltSetViewport(u32 x, u32 y, u32 w, u32 h) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetViewport(renderState, x, y, w, h);
}

void ltClearColor(void) {
    ltRendererClearColor();
}

void ltSetShader(LTshaderProgram* shader) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetShader(renderState, shader);
}

void ltSetTexture2D(LTtexture2D* texture) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetTexture2D(renderState, texture);
}

void ltDraw(LTvertexData* data) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetVertexData(renderState, data);
    ltRendererDraw(renderState);
}

void ltDrawArrays(LTvertexData* data) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetVertexData(renderState, data);
    ltRendererDrawArrays(renderState);
}

void ltDrawIndexed(LTvertexData* data, void* offset) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    ltRendererSetVertexData(renderState, data);
    ltRendererDrawIndexed(renderState, offset);
}

void ltSwapBuffers(void) {
    LTplatformState* platformState = (LTplatformState*)ltGetPlatformState();
    ltPlatformSwapBuffers(platformState);
}

void lotusExit(void) {
    LTrenderState* renderState = (LTrenderState*)ltGetRenderState();
    LTplatformState* platformState = (LTplatformState*)ltGetPlatformState();
    ltInputExit();
    ltEventExit();
    ltRendererExit(renderState);
    ltPlatformExit(platformState);
    ltMemoryExit();
}

