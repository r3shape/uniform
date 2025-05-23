#ifndef __GPUAPI_H__
#define __GPUAPI_H__

#include <include/swarm/core/swarm_renderer.h>

typedef struct GPUAPI {
    none (*exit)(none);

    none (*clearDepthBuffer)(f32 depth);
    none (*clearColorBuffer)(Vec3 color);

    GPUProgram* (*getProgram)(GPUHandle handle);
    none (*createProgram)(GPUProgram* program);
    none (*destroyProgram)(GPUProgram* program);
    
    none (*sendUniform)(cstr name, GPUProgram* program);
    none (*setUniform)(GPUUniform* uniform, GPUProgram* program);
    
    GPUBuffer* (*getBuffer)(GPUHandle handle);
    none (*createVertexBuffer)(GPUBuffer* buffer);
    none (*destroyVertexBuffer)(GPUBuffer* buffer);
    
    none (*createElementBuffer)(GPUBuffer* buffer);
    none (*destroyElementBuffer)(GPUBuffer* buffer);
    
    none (*createTextureBuffer)(GPUBuffer* buffer);
    none (*destroyTextureBuffer)(GPUBuffer* buffer);
    
    none (*createRenderBuffer)(GPUBuffer* buffer);
    none (*destroyRenderBuffer)(GPUBuffer* buffer);
    
    none (*createFrameBuffer)(GPUBuffer* buffer);
    none (*destroyFrameBuffer)(GPUBuffer* buffer);

    none (*bindBuffer)(GPUBuffer* buffer);
    none (*readBuffer)(GPUBuffer* buffer);
    none (*writeBuffer)(GPUBuffer* buffer);
    none (*bindProgram)(GPUProgram* program);
} GPUAPI;

#endif // __GPUAPI_H__