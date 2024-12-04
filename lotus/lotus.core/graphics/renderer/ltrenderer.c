#include "ltrenderer.h"

#include "../gl/ltgl.h"
#include "../../utility/ltmath.h"
#include "../../memory/ltmemory.h"
#include "../../platform/ltlogger.h"

b8 ltRendererInit(LTrenderState* state, u32 vpWidth, u32 vpHeight) {
    if (!ltglLoadFunctions()) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to load modern OpenGL functions", 0);
        return LOTUS_FALSE;
    }
    
    state->passes = 0;
    state->mode = GL_TRIANGLES;
    state->vpSize[0] = vpWidth;
    state->vpSize[1] = vpHeight;
    
    state->texture2D = NULL;
    state->vertexData = NULL;
    state->mProj = ltIdentity();
    state->shaderProgram = NULL;

    ltglEnable(GL_BLEND);
    ltglEnable(GL_CULL_FACE);
    ltglCullFace(GL_BACK);
    ltglEnable(GL_DEPTH_TEST);
    ltglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Lotus doesnt use gl stencils, so disable the tests
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
    state->texture2D = NULL;
    state->vertexData = NULL;
    state->shaderProgram = NULL;
}

void ltRendererSetViewport(LTrenderState *state, i32 x, i32 y, i32 width, i32 height) {
    state->vpSize[0] = width;
    state->vpSize[1] = height;
    ltglViewport(x, y, width, height);
}

void ltRendererSetClearColor(LTrenderState *state, f32 r, f32 g, f32 b, f32 a) {
    state->clearColor[0] = r;
    state->clearColor[1] = g;
    state->clearColor[2] = b;
    state->clearColor[3] = a;
    ltglClearColor(r, g, b, a);
}

void ltRendererClearColor(void) {
    ltglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ltRendererSetVertexData(LTrenderState *state, LTvertexData* data) {
    if (data == NULL) return;
    state->vertexData = data;
}

void ltRendererSetTexture2D(LTrenderState *state, LTtexture2D* texture) {
    if (texture == NULL) return;
    state->texture2D = texture;
}

void ltRendererSetShader(LTrenderState *state, LTshaderProgram* program) {
    if (program == NULL) return;
    state->shaderProgram = program;
}

void ltRendererDrawArrays(LTrenderState *state) {
    if (!state->vertexData) return;
    if (state->shaderProgram) ltglUseProgram(state->shaderProgram->program);
    if (state->texture2D) ltglBindTexture(GL_TEXTURE_2D, state->texture2D->handle);
    ltglBindVertexArray(state->vertexData->vertexArray);
    ltglDrawArrays(state->mode, 0, state->vertexData->nVertices);
    ltglBindTexture(GL_TEXTURE_2D, 0);
    ltglBindVertexArray(0);
}

void ltRendererDrawIndexed(LTrenderState *state, void* offset) {
    if (!state->vertexData) return;
    if (state->shaderProgram) ltglUseProgram(state->shaderProgram->program);
    if (state->texture2D) ltglBindTexture(GL_TEXTURE_2D, state->texture2D->handle);
    ltglBindVertexArray(state->vertexData->vertexArray);
    ltglDrawElements(state->mode, state->vertexData->nIndices, GL_UNSIGNED_INT, offset);
    ltglBindTexture(GL_TEXTURE_2D, 0);
    ltglBindVertexArray(0);
}

void ltRendererDraw(LTrenderState *state) {
    if (!state->vertexData) return;
    if (state->shaderProgram) ltglUseProgram(state->shaderProgram->program);
    if (state->texture2D) ltglBindTexture(GL_TEXTURE_2D, state->texture2D->handle);
    ltglBindVertexArray(state->vertexData->vertexArray);
    if (state->vertexData->nIndices > 0) {
        ltglDrawElements(state->mode, state->vertexData->nIndices, GL_UNSIGNED_INT, (void*)0);
    } else { ltglDrawArrays(state->mode, 0, state->vertexData->nVertices); }
    ltglBindTexture(GL_TEXTURE_2D, 0);
    ltglBindVertexArray(0);
}
