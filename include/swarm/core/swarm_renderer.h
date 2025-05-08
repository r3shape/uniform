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
    GPU_TEXTURE_FORMAT_RGBA,
    GPU_TEXTURE_FORMAT_RGB
} GPUTextureFormat;

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
            f32* vertices;
            u32 count;
        } vertex;
    } data;
} GPUBuffer;

typedef struct GPUProgram {
    cstr vertex;
    cstr fragment;
    GPUHandle handle;
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
    GPUHandle phase;
    GPUHandle pipeline;
    GPUHandle elementBuffer;
    GPUHandle vertexBuffer;
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

typedef struct Renderer {
    none (*init)(GPUBackend backend);
    none (*shutdown)(none);
    none (*render)(none);
    
    GPUHandle (*createFrame)(none);
    GPUHandle (*createCall)(GPUNode node);
    none (*commitFrame)(GPUHandle handle);
    GPUHandle (*createPipeline)(GPUHandle program, u32 mask);
    GPUHandle (*createProgram)(const char* vs, const char* fs);
    GPUHandle (*createBuffer)(GPUBufferType type, u32 size, ptr data);
    GPUHandle (*createTexture)(GPUTextureType type, GPUTextureFormat fmt, u32 w, u32 h, ptr data);
    GPUHandle (*createPhase)(GPUPhaseType type, Vec3 clearColor, Vec3 clearDepth, GPUHandle framebuffer);
    
    none (*bindProgram)(GPUHandle program);
    none (*bindPipeline)(GPUHandle pipeline);
    none (*bindTexture)(GPUHandle texture, u32 slot);
    none (*bindBuffer)(GPUHandle buffer, GPUBufferType type);
} Renderer;
extern Renderer* swarmRenderer;

#endif  // __SWARM_RENDERER_H__