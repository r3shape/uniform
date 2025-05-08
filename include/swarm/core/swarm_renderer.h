#ifndef __SWARM_RENDERER_H__
#define __SWARM_RENDERER_H__

#include <include/swarm/core/globals.h>

#define SWARM_GPU_RESOURCE_MAX 1024

typedef u32 GPUHandle;

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
    GPU_BUFFER_RENDER,
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

typedef struct GPUBuffer {
    u32 size;
    GPUHandle handle;
    GPUBufferType type;
    union {
        struct {
            GPUHandle ebo;
            u32* elements;
            u32 count;
        } element;
        
        struct {
            GPUHandle vbo;
            GPUHandle vao;
            f32* vertices;
            u32 count;
            u8 format;
        } vertex;
    } data;
} GPUBuffer;

typedef enum GPUVertexAttribute {
    GPU_VERTEX_LOCATION = 1 << 0, // 1 (0b0001)
    GPU_VERTEX_COLOR    = 1 << 1, // 2 (0b0010)
    GPU_VERTEX_TEX      = 1 << 2, // 4 (0b0100)
    GPU_VERTEX_NORMAL   = 1 << 3, // 8 (0b1000)
    GPU_VERTEX_ATTRIBS = 4
} GPUVertexAttribute;

typedef enum GPUUniformType {
    GPU_UNIFORM_NONE=0,
    GPU_UNIFORM_FLOAT,
    GPU_UNIFORM_VEC2,
    GPU_UNIFORM_VEC3,
    GPU_UNIFORM_VEC4,
    GPU_UNIFORM_MAT4,
    GPU_UNIFORM_TYPES
} GPUUniformType;

typedef struct GPUUniform {
    cstr name;
    u32 location;
    GPUUniformType type;
    union {
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
    ptr data;
    u32 width;
    u32 height;
    u32 format;
    GPUHandle handle;
    GPUTextureType type;
} GPUTexture;

typedef struct GPUPipeline {
    u32 mask;
    GPUHandle handle;
    GPUHandle program;
    // GPUHandle bindings[4];
} GPUPipeline;

typedef enum GPUPhaseType {
    GPU_PHASE_INVALID,
    GPU_PHASE_DEPTH,
    GPU_PHASE_OPAQUE,
    GPU_PHASE_LIGHT,
    GPU_PHASE_SHADOW
} GPUPhaseType;

typedef struct GPUPhase {
    // u32 reads;
    // u32 writes;
    Vec3 clearColor;
    Vec3 clearDepth;
    GPUHandle handle;
    GPUHandle framebuffer;
    // GPUHandle readv[8];
    // GPUHandle writev[8];
    GPUPhaseType type;
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
} GPUFrame;

typedef struct GPUResources {
    GPUBuffer bufferv[SWARM_GPU_RESOURCE_MAX];
    GPUTexture texturev[SWARM_GPU_RESOURCE_MAX];
    GPUProgram programv[SWARM_GPU_RESOURCE_MAX];
    GPUPipeline pipelinev[SWARM_GPU_RESOURCE_MAX];
    u32 buffers, textures, programs, pipelines, frames;
} GPUResources;

typedef struct GPUState {
    GPUFrame* frame;
    GPUPhase* phase;
    GPUBackend backend;
} GPUState;

typedef struct GPUAPI {
    none (*clearColorBuffer)(Vec3 color);
    none (*clearDepthBuffer)(Vec3 depth);

    GPUProgram* (*getProgram)(GPUHandle handle);
    none (*createProgram)(GPUProgram* program);
    none (*destroyProgram)(GPUProgram* program);
    
    none (*sendUniform)(cstr name, GPUProgram* program);
    none (*setUniform)(GPUUniform* uniform, GPUProgram* program);
    
    GPUBuffer* (*getBuffer)(GPUHandle handle);
    none (*createVertexBuffer)(u8 format, GPUBuffer* buffer);
    none (*destroyVertexBuffer)(GPUBuffer* buffer);
    
    none (*createElementBuffer)(GPUBuffer* buffer);
    none (*destroyElementBuffer)(GPUBuffer* buffer);
    
    none (*createTexture2D)(GPUTexture* texture);
    none (*destroyTexture2D)(GPUTexture* texture);

    none (*bindBuffer)(GPUBuffer* buffer);
    none (*renderBuffer)(GPUBuffer* buffer);
    none (*bindProgram)(GPUProgram* program);
    none (*bindTexture)(GPUHandle texture, u32 slot);
} GPUAPI;

typedef struct Renderer {
    none (*init)(GPUBackend backend);
    none (*shutdown)(none);
    none (*render)(none);
    
    GPUHandle (*createFrame)(none);
    none (*commitFrame)(GPUHandle handle);
    GPUHandle (*createCall)(GPUNode node);
    GPUHandle (*createProgram)(str vs, str fs);
    GPUHandle (*createPipeline)(GPUHandle program, u32 mask);
    GPUHandle (*createBuffer)(GPUBufferType type, u8 format, u32 size, ptr data);
    GPUHandle (*createTexture)(GPUTextureType type, GPUTextureFormat fmt, u32 w, u32 h, ptr data);
    GPUHandle (*createPhase)(GPUPhaseType type, Vec3 clearColor, Vec3 clearDepth, GPUHandle framebuffer);
} Renderer;

extern Renderer* swarmRenderer;

#endif  // __SWARM_RENDERER_H__