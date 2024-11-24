#include "ltrenderer.h"

#include "../gl/ltgl.h"
#include "../platform/ltlogger.h"
#include "../platform/ltplatform.h"

typedef struct tagRendererInternal{
    i32 mode;                               // OpenGL draw mode
    i32 passes;                             // total successful render passes
    i32 vpSize[2];                          // viewport size
    f32 clearColor[4];                      // color value populating the color buffer
    LTtexture2D* texture2D;           // pointer to the texture handle currently bound for rendering
    LTvertexData* vertexData;         // pointer to the vertex data currently being rendered
    LTshaderProgram* shaderProgram;   // pointer to the shader program currently being used
} LTrendererInternal;

b8 ltRendererInit(LTrenderState *state, u32 vpWidth, u32 vpHeight) {
    state->internal = (LTrendererInternal*)malloc(sizeof(LTrendererInternal));
    if (!state->internal) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to allocate internal render state structure", 0);
        return LOTUS_FALSE;
    }
    
    if (!ltglLoadFunctions()) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to load modern OpenGL functions", 0);
        return LOTUS_FALSE;
    }
    
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;

    internalState->passes = 0;
    internalState->mode = GL_TRIANGLES;
    internalState->vpSize[0] = vpWidth;
    internalState->vpSize[1] = vpHeight;
    internalState->texture2D = NULL;
    internalState->vertexData = NULL;
    internalState->shaderProgram = NULL;

    ltglEnable(GL_BLEND);
    ltglEnable(GL_CULL_FACE);
    ltglCullFace(GL_BACK);
    ltglEnable(GL_DEPTH_TEST);
    ltglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ltlipse doesnt use gl stencils, so disable the tests
    ltglDisable(GL_STENCIL_TEST);

    ltRendererSetClearColor(state, 0.03, 0.24, 0.5, 1.0);
    ltRendererSetViewport(state, 0, 0, vpWidth, vpHeight);

    ltSetLogLevel(LOTUS_LOG_INFO);
    ltLogInfo("OpenGL Vendor: %s\n", ltglGetString(GL_VENDOR));
    ltLogInfo("OpenGL Version: %s\n", ltglGetString(GL_VERSION));
    ltLogInfo("OpenGL Renderer: %s\n", ltglGetString(GL_RENDERER));
    ltLogInfo("GLSL Version: %s\n", ltglGetString(LTGL_SHADING_LANGUAGE_VERSION));

    return LOTUS_TRUE;
}

void ltRendererExit(LTrenderState *state) {
    free(state->internal);
}

void ltRendererSetViewport(LTrenderState *state, i32 x, i32 y, i32 width, i32 height) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    internalState->vpSize[0] = width;
    internalState->vpSize[1] = height;
    ltglViewport(x, y, width, height);
}

void ltRendererSetClearColor(LTrenderState *state, f32 r, f32 g, f32 b, f32 a) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    internalState->clearColor[0] = r;
    internalState->clearColor[1] = g;
    internalState->clearColor[2] = b;
    internalState->clearColor[3] = a;
    ltglClearColor(r, g, b, a);
}

void ltRendererClearColor(void) {
    ltglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ltRendererSetVertexData(LTrenderState *state, LTvertexData* data) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    internalState->vertexData = data;
}

void ltRendererSetTexture2D(LTrenderState *state, LTtexture2D* texture) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    internalState->texture2D = texture;
}

void ltRendererSetShader(LTrenderState *state, LTshaderProgram* program) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    internalState->shaderProgram = program;
}

void ltRendererDrawArrays(LTrenderState *state) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    if (!internalState->vertexData) return;
    if (internalState->shaderProgram) ltglUseProgram(internalState->shaderProgram->program);
    if (internalState->texture2D) ltglBindTexture(GL_TEXTURE_2D, internalState->texture2D->handle);
    ltglBindVertexArray(internalState->vertexData->vertexArray);
    ltglDrawArrays(internalState->mode, 0, internalState->vertexData->nVertices);
    ltglBindTexture(GL_TEXTURE_2D, 0);
    ltglBindVertexArray(0);
}

void ltRendererDrawIndexed(LTrenderState *state, void* offset) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    if (!internalState->vertexData) return;
    if (internalState->shaderProgram) ltglUseProgram(internalState->shaderProgram->program);
    if (internalState->texture2D) ltglBindTexture(GL_TEXTURE_2D, internalState->texture2D->handle);
    ltglBindVertexArray(internalState->vertexData->vertexArray);
    ltglDrawElements(internalState->mode, internalState->vertexData->nIndices, GL_UNSIGNED_INT, offset);
    ltglBindTexture(GL_TEXTURE_2D, 0);
    ltglBindVertexArray(0);
}

void ltRendererDraw(LTrenderState *state) {
    LTrendererInternal* internalState = (LTrendererInternal*)state->internal;
    if (!internalState->vertexData) return;
    if (internalState->shaderProgram) ltglUseProgram(internalState->shaderProgram->program);
    if (internalState->texture2D) ltglBindTexture(GL_TEXTURE_2D, internalState->texture2D->handle);
    ltglBindVertexArray(internalState->vertexData->vertexArray);
    if (internalState->vertexData->nIndices > 0) {
        ltglDrawElements(internalState->mode, internalState->vertexData->nIndices, GL_UNSIGNED_INT, (void*)0);
    } else { ltglDrawArrays(internalState->mode, 0, internalState->vertexData->nVertices); }
    ltglBindTexture(GL_TEXTURE_2D, 0);
    ltglBindVertexArray(0);
}
