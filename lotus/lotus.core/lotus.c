#include <stdio.h>

#include "../include/lotus.h"
#include "../include/version.h"
#include "memory/ltmemory.h"
#include "platform/ltlogger.h"
#include "platform/ltplatform.h"
#include "platform/event/ltevent.h"
#include "platform/input/ltinput.h"
#include "graphics/renderer/ltrenderer.h"

b8 lotusInit(void) {
    ltMemoryInit();

    sprintf(Engine.version, "%d.%d.%d", LOTUS_YEAR, LOTUS_MINOR, LOTUS_PATCH);
    ltSetLogLevel(LOTUS_LOG_INFO);
    ltLogInfo("Lotus v%s\n", Engine.version);

    if (
        !ltEventInit()                                                                      ||
        !ltInputInit()                                                                      ||
        !ltPlatformInit(&Engine.platformState, "Lotus Engine Test", 100, 100, 1280, 720)          ||
        !ltRendererInit(&Engine.renderState, 1280, 720)                                        // ||
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
        return LOTUS_TRUE;
    }
}

LTrenderState* ltGetRenderState(void) { return &Engine.renderState; }

LTplatformState* ltGetPlatformState(void) { return &Engine.platformState; }

b8 ltPumpEvents(void) {
    return ltPlatformPump(&Engine.platformState);
}

void ltSetClearColor(f32 r, f32 g, f32 b, f32 a) {
    ltRendererSetClearColor(&Engine.renderState, r, g, b, a);
}

void ltSetViewport(u32 x, u32 y, u32 w, u32 h) {
    ltRendererSetViewport(&Engine.renderState, x, y, w, h);
}

void ltClearColor(void) {
    ltRendererClearColor();
}

void ltSetShader(LTshaderProgram* shader) {
    ltRendererSetShader(&Engine.renderState, shader);
}

void ltSetTexture2D(LTtexture2D* texture) {
    ltRendererSetTexture2D(&Engine.renderState, texture);
}

void ltDraw(LTvertexData* data) {
    ltRendererSetVertexData(&Engine.renderState, data);
    ltRendererDraw(&Engine.renderState);
}

void ltDrawArrays(LTvertexData* data) {
    ltRendererSetVertexData(&Engine.renderState, data);
    ltRendererDrawArrays(&Engine.renderState);
}

void ltDrawIndexed(LTvertexData* data, void* offset) {
    ltRendererSetVertexData(&Engine.renderState, data);
    ltRendererDrawIndexed(&Engine.renderState, offset);
}

void ltSwapBuffers(void) {
    ltPlatformSwapBuffers(&Engine.platformState);
}

void lotusExit(void) {
    ltInputExit();
    ltEventExit();
    ltRendererExit(&Engine.renderState);
    ltPlatformExit(&Engine.platformState);
    ltMemoryExit();
}

