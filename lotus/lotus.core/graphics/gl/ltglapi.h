#ifndef LTGLAPI_H
#define LTGLAPI_H

#include "../../lotustypes.h"
#include "../../defines.h"

typedef enum LTGLuniformType {
    LOTUS_UNIFORM_NONE=0,
    LOTUS_UNIFORM_VEC2,
    LOTUS_UNIFORM_VEC3,
    LOTUS_UNIFORM_VEC4,
    LOTUS_UNIFORM_MAT4,
    LOTUS_UNIFORM_TYPES
} LTGLuniformType;

LOTUS_API_ENTRY LTtexture2D ltglTexture2D(const char* texturePath);
LOTUS_API_ENTRY LTshaderProgram ltglShaderProgram(const char* vertexSrc, const char* fragmentSrc);
LOTUS_API_ENTRY LTvertexData ltglVertexData(u32 nAttributes, u32 nVertices, u32 nIndices, f32* vertices, u32* indices);

LOTUS_API_ENTRY LTshaderUniform ltglGetUniform(LTshaderProgram program, const char* name);
LOTUS_API_ENTRY void ltglSetUniform(LTshaderProgram program, LTGLuniformType type, const char* name, void* value);

#endif