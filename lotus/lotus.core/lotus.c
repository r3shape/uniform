#include <stdio.h>

#include "../include/lotus.h"

#include "memory/ltmemory.h"
#include "platform/ltlogger.h"
#include "platform/ltplatform.h"
#include "renderer/ltrenderer.h"

static LTrenderState _renderState;
static LTplatformState _platformState;

b8 lotusInit(void) {
    ltMemoryInit();

    sprintf(Engine.version, "%d.%d.%d", LOTUS_YEAR, LOTUS_MINOR, LOTUS_PATCH);
    ltSetLogLevel(LOTUS_LOG_INFO);
    ltLogInfo("Lotus v%s\n", Engine.version);
    
    if (
        !ltPlatformInit(&_platformState, "Lotus Engine Test", 100, 100, 1280, 720)          ||
        !ltRendererInit(&_renderState, 1280, 720)                                        // ||
        ) {
            ltSetLogLevel(LOTUS_LOG_FATAL);
            ltLogError("failed to initialize Lotus");
            Engine.init = LOTUS_FALSE;
            return LOTUS_FALSE;
    } else {
        ltSetLogLevel(LOTUS_LOG_INFO);
        char* memStats = ltGetMemoryUsageStr();
        ltLogInfo("%s\n", memStats);
        _ltMemFree(memStats, LOTUS_FALSE);

        Engine.init = LOTUS_TRUE;
        Engine.renderState = &_renderState;
        Engine.platformState = &_platformState;
        return LOTUS_TRUE;
    }
}

b8 ltPumpEvents(void) {
    LTplatformState* platformState = (LTplatformState*)Engine.platformState;
    return ltPlatformPump(platformState);
}

void ltSetClearColor(f32 r, f32 g, f32 b, f32 a) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    ltRendererSetClearColor(renderState, r, g, b, a);
}

void ltClearColor(void) {
    ltRendererClearColor();
}

void ltSetShader(LTshaderProgram* shader) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    ltRendererSetShader(renderState, shader);
}

void ltSetTexture2D(LTtexture2D* texture) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    ltRendererSetTexture2D(renderState, texture);
}

void ltDraw(LTvertexData* data) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    ltRendererSetVertexData(renderState, data);
    ltRendererDraw((LTrenderState*)Engine.renderState);
}

void ltDrawArrays(LTvertexData* data) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    ltRendererSetVertexData(renderState, data);
    ltRendererDrawArrays((LTrenderState*)Engine.renderState);
}

void ltDrawIndexed(LTvertexData* data, void* offset) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    ltRendererSetVertexData(renderState, data);
    ltRendererDrawIndexed((LTrenderState*)Engine.renderState, offset);
}

void ltSwapBuffers(void) {
    LTplatformState* platformState = (LTplatformState*)Engine.platformState;
    ltPlatformSwapBuffers(platformState);
}

void lotusExit(void) {
    LTrenderState* renderState = (LTrenderState*)Engine.renderState;
    LTplatformState* platformState = (LTplatformState*)Engine.platformState;
    ltRendererExit(renderState);
    ltPlatformExit(platformState);
    ltMemoryExit();
}

