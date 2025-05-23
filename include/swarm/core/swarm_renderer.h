#ifndef __SWARM_RENDERER_H__
#define __SWARM_RENDERER_H__

#include <include/swarm/core/globals.h>

#define SWARM_GPU_RESOURCE_MAX 1024

typedef u32 GPUHandle;

#define GL_TEXTURE0                 0x84C0
#define GL_TEXTURE1                 0x84C1
#define GL_TEXTURE2                 0x84C2
#define GL_TEXTURE3                 0x84C3

typedef enum GPUBackend {
    GPU_BACKEND_INVALID,
    GPU_BACKEND_OPENGL,
    GPU_BACKEND_VULKAN,
    GPU_BACKEND_DIRECTX
} GPUBackend;

typedef enum GPUBufferType {
    GPU_BUFFER_INVALID,
    GPU_BUFFER_UNIFORM,
    GPU_BUFFER_ELEMENT,
    GPU_BUFFER_TEXTURE,
    GPU_BUFFER_VERTEX,
    GPU_BUFFER_FRAME
} GPUBufferType;

typedef enum GPUTextureType {
    GPU_TEXTURE_INVALID,
    GPU_TEXTURE_PLANE,
    GPU_TEXTURE_CUBE
} GPUTextureType;

typedef enum GPUTextureFormat {
    GPU_TEXTURE_FORMAT_INVALID,
    GPU_TEXTURE_FORMAT_RGBA     = 0x1908,
    GPU_TEXTURE_FORMAT_RGB      = 0x1907
} GPUTextureFormat;

typedef enum GPUMode {
    GPU_MODE_INVALID,
    GPU_POINT_MODE      = 0x0000,
    GPU_LINE_MODE       = 0x0001,
    GPU_TRIANGLE_MODE   = 0x0004,
    GPU_QUAD_MODE       = 0x0007
} GPUMode;

typedef enum GPUVertexAttribute {
    GPU_VERTEX_LOCATION = 1 << 0, // 1 (0b0001)
    GPU_VERTEX_COLOR    = 1 << 1, // 2 (0b0010)
    GPU_VERTEX_TEX      = 1 << 2, // 4 (0b0100)
    GPU_VERTEX_NORMAL   = 1 << 3, // 8 (0b1000)
    GPU_VERTEX_ATTRIBS = 4
} GPUVertexAttribute;

typedef enum GPUUniformType {
    GPU_UNIFORM_NONE=0,
    GPU_UNIFORM_INT,
    GPU_UNIFORM_FLOAT,
    GPU_UNIFORM_VEC2,
    GPU_UNIFORM_VEC3,
    GPU_UNIFORM_VEC4,
    GPU_UNIFORM_MAT4,
    GPU_UNIFORM_TYPES
} GPUUniformType;

typedef enum GPUPhaseType {
    GPU_PHASE_INVALID,
    GPU_PHASE_DEPTH,
    GPU_PHASE_OPAQUE,
    GPU_PHASE_LIGHT,
    GPU_PHASE_SHADOW
} GPUPhaseType;

typedef struct GPUBuffer {
    u32 size;
    GPUHandle handle;
    GPUBufferType type;
    union {
        struct {
            u32* elements;
            GPUHandle ebo;
            u32 count;
        } element;
        
        struct {
            f32* vertices;
            GPUHandle vbo;
            GPUHandle vao;
            u32 format;
            u32 count;
        } vertex;

        struct {
            cstr path;
            ptr data;
            u32 slot;
            i32 width;
            i32 height;
            i32 channels;
            GPUHandle tbo;
            GPUTextureType type;
            GPUTextureFormat format;
        } texture;

        struct {
            u32 width;
            u32 height;
            GPUHandle rbo;
        } render;
        
        struct {
            GPUHandle rbo;
            GPUHandle tbo;
            GPUHandle fbo;
        } frame;
    } data;
} GPUBuffer;

typedef struct GPUUniform {
    cstr name;
    u32 location;
    GPUUniformType type;
    union {
        i32 i;
        f32 f;
        Vec2 v2;
        Vec3 v3;
        Vec4 v4;
        Mat4 m4;
    } value;
} GPUUniform;

typedef struct GPUProgram {
    str vertex;
    str fragment;
    GPUHandle handle;
    GPUHandle program;
    HashArray uniforms;
} GPUProgram;

typedef struct GPUTexture {
    cstr path;
    u32 slot;
    GPUTextureType type;
} GPUTexture;

typedef struct GPUPipeline {
    u32 mask;
    u32 binds;
    GPUHandle handle;
    GPUHandle texture;
    GPUHandle program;
    GPUHandle bindv[4];
} GPUPipeline;

typedef struct GPUPhase {
    // u32 reads;
    // u32 writes;
    GPUHandle handle;
    GPUPhaseType type;
    union {
        f32 depth;
        Vec3 color;
    } data;
    // GPUHandle readv[8];
    // GPUHandle writev[8];
} GPUPhase;

typedef struct GPUNode {
    GPUUniform uniformv[16];
    GPUHandle elementBuffer;
    GPUHandle vertexBuffer;
    GPUHandle pipeline;
    GPUHandle phase;
    u8 uniforms;
} GPUNode;

typedef struct GPUCall {
    GPUNode node;
    u32 key;
} GPUCall;

typedef struct GPUFrame {
    u32 calls;
    u32 phases;
    Allocator arena;
    GPUHandle handle;
    GPUPhase phasev[32];
    GPUCall callv[1024];
    GPUBuffer buffer;
} GPUFrame;

typedef struct GPUResources {
    GPUBuffer bufferv[SWARM_GPU_RESOURCE_MAX];
    GPUProgram programv[SWARM_GPU_RESOURCE_MAX];
    GPUPipeline pipelinev[SWARM_GPU_RESOURCE_MAX];
    u32 buffers, programs, pipelines, frames;
} GPUResources;

typedef struct GPUState {
    GPUFrame* frame;
    GPUPhase* phase;
    GPUBackend backend;
} GPUState;

typedef struct Renderer {
    none (*init)(GPUBackend backend);
    none (*shutdown)(none);
    none (*render)(none);
    
    none (*createFrame)(none);
    GPUHandle (*createCall)(GPUNode node);
    GPUHandle (*createProgram)(str vs, str fs);
    GPUHandle (*createPipeline)(GPUHandle program, u32 mask);
    GPUHandle (*createPhase)(GPUPhaseType type, f32 clearDepth, Vec3 clearColor);
    GPUHandle (*createBuffer)(GPUBufferType type, u32 format, u32 size, ptr data);

    none (*bindPipelineBuffer)(GPUHandle buffer, GPUHandle pipeline);
    none (*bindPipelinerogram)(GPUHandle program, GPUHandle pipeline);
} Renderer;

extern Renderer* swarmRenderer;

#endif  // __SWARM_RENDERER_H__