#ifndef ECTYPES_H
#define ECTYPES_H

#include "defines.h"

/* UTILITY TYPES */

typedef struct tagKeyValue {
    const char* k;
    void* v;
} LTkeyValue;

typedef struct tagHashmap {
    int max;
    int count;
    LTkeyValue** map;
} LThashmap;

/* PIPELINE TYPES */

typedef enum LTGLuniformType {
    LOTUS_UNIFORM_NONE=0,
    LOTUS_UNIFORM_VEC2,
    LOTUS_UNIFORM_VEC3,
    LOTUS_UNIFORM_VEC4,
    LOTUS_UNIFORM_MAT4,
    LOTUS_UNIFORM_TYPES
} LTGLuniformType;

typedef struct tagVertexData {
    i32 nIndices;
    i32 nVertices;
    i32 vertexArray;
    i32 vertexBuffer;
    i32 elementBuffer;
} LTvertexData;

typedef struct tagShaderProgram {
    i32 program;
    LThashmap* uniforms;
} LTshaderProgram;

typedef struct tagShaderUniform {
    i32 location;
    void* value;
    const char* name;
} LTshaderUniform;

typedef struct tagTexture2D {
    i32 width;
    i32 height;
    i32 handle;
    i32 nChannels;
    u8* textureData;
} LTtexture2D;

/* MATH TYPES */

typedef struct tagVec2 {
    f32 x, y;
} LTvec2;

typedef struct tagVec2i {
    int x, y;
} LTvec2i;

typedef struct tagVec2b {
    b8 x, y;
} LTvec2b;

typedef struct tagVec3 {
    f32 x, y, z;
} LTvec3;

typedef struct tagVec3i {
    int x, y, z;
} LTvec3i;

typedef struct tagVec3b {
    b8 x, y, z;
} LTvec3b;

typedef struct tagVec4 {
    f32 x, y, z, w;
} LTvec4;

typedef struct tagVec4i {
    int x, y, z, w;
} LTvec4i;

typedef struct tagVec4b {
    b8 x, y, z, w;
} LTvec4b;

typedef struct tagMat4{
    f32 m[16];
} LTmat4;

/* ENGINE TYPES */

typedef enum LTerrorType {
    LOTUS_ERR_NONE=0,
    LOTUS_ERR_INIT,
    LOTUS_ERR_TYPE,
    LOTUS_ERR_FUNC,
    LOTUS_ERR_MALLOC,
    LOTUS_ERR_TYPES,
} LTerrorType;

typedef struct tagWindow {
    void* state;
    void* handle;
    char* title;
    LTvec2i size;
    LTvec2 location;
} LTwindow;

typedef struct tagPlatformState {
    LTwindow window;
} LTplatformState;

typedef struct tagPlatformLibrary {
    char* name;
    void* module;
} LTdynamicLib;

typedef struct tagRendererState{
    i32 mode;                                 // OpenGL draw mode
    i32 passes;                               // total successful render passes
    LTmat4 mProj;                             // current projection matrix
    i32 vpSize[2];                            // viewport size
    f32 clearColor[4];                        // color value populating the color buffer
    LTtexture2D* texture2D;                   // pointer to the texture handle currently bound for rendering
    LTvertexData* vertexData;                 // pointer to the vertex data currently being rendered
    LTshaderProgram* shaderProgram;           // pointer to the shader program currently being used
} LTrenderState;

#endif