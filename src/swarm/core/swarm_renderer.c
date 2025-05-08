#include <include/swarm/core/swarm_renderer.h>
#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/GLAPI.h>

// GPU BACKENDS
GLAPI* gl = NULL;

// GPU STATE
GPUFrame defaultFrame = {0};
GPUPhase defaultPhase = {0};
GPUResources resources = (GPUResources){0};
GPUState state = (GPUState){ .frame = &defaultFrame, .phase = &defaultPhase };

GPUHandle _createProgram(const char* vs, const char* fs) {
    GPUHandle handle = resources.programs++;
    GPUProgram* prog = &resources.programv[handle];
    *prog = (GPUProgram){ .vertex = vs, .fragment = fs, .handle = handle };
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUProgram (handle=%u)", handle);
    return handle;
}

GPUHandle _createBuffer(GPUBufferType type, u32 size, ptr data) {
    GPUHandle handle = resources.buffers++;
    GPUBuffer* buf = &resources.bufferv[handle];

    switch(type) {
        case(GPU_BUFFER_ELEMENT): {
            *buf = (GPUBuffer){
                .type = type,
                .size = size,
                .handle = handle,
                .data.element.ebo = handle,   // e.g GLAPI ebo stored here
                .data.element.elements = data,
                .data.element.count = size/sizeof(u32)
            };
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUBuffer (elements=%u, size=%d)", buf->data.element.count, size);
        } break;
        case(GPU_BUFFER_VERTEX): {
            *buf = (GPUBuffer){
                .type = type,
                .size = size,
                .handle = handle,
                .data.vertex.vbo = handle,   // e.g GLAPI vao stored here
                .data.vertex.vertices = data,
                .data.vertex.count = size/sizeof(f32)
            };
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUBuffer (vertices=%u, size=%d)", buf->data.vertex.count, size);
        } break;
        case(GPU_BUFFER_UNIFORM):   // fall-through
        case(GPU_BUFFER_TEXTURE):   // fall-through
        case(GPU_BUFFER_RENDER):   // fall-through
        case(GPU_BUFFER_FRAME):   // fall-through
        case(GPU_BUFFER_INVALID):   // fall-through
        default: break;
    }
    
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUBuffer (type=%d, handle=%u, size=%u)", type, handle, size);
    return handle;
}

GPUHandle _createTexture(GPUTextureType type, GPUTextureFormat fmt, u32 w, u32 h, ptr data) {
    GPUHandle handle = resources.textures++;
    GPUTexture* tex = &resources.texturev[handle];
    *tex = (GPUTexture){ .type = type, .format = fmt, .width = w, .height = h, .data = data, .handle = handle };
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUTexture (type=%d, format=%d, handle=%u, w=%u, h=%u)", type, handle, w, h);
    return handle;
}

GPUHandle _createPipeline(GPUHandle program, u32 mask) {
    GPUHandle handle = resources.pipelines++;
    GPUPipeline* pipe = &resources.pipelinev[handle];
    *pipe = (GPUPipeline){ .program = program, .mask = mask, .handle = handle };
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUPipeline (program=%u, handle=%u, mask=0x%X)", program, handle, mask);
    return handle;
}

GPUHandle _createFrame(none) {
    state.frame->calls = 0;
    state.frame->phases = 0;
    state.frame->handle = resources.frames++;
    saneMemory->arenaReset(&state.frame->arena);
    memset(state.frame->callv,  0, sizeof(state.frame->callv));
    memset(state.frame->phasev, 0, sizeof(state.frame->phasev));
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUFrame (handle=%u)", state.frame->handle);
    return state.frame->handle;
}

GPUHandle _createPhase(GPUPhaseType type, Vec3 clearColor, Vec3 clearDepth, GPUHandle framebuffer) {
    GPUHandle handle = state.frame->phases++;
    GPUPhase* phase = &state.frame->phasev[handle];
    *phase = (GPUPhase){ .handle = handle, .type = type, .clearColor = clearColor, .clearDepth = clearDepth, .framebuffer = framebuffer };
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUPhase (handle=%u, type=%d, framebuffer=%u)", handle, type, framebuffer);
    return handle;
}

GPUHandle _createCall(GPUNode node) {
    GPUHandle handle = state.frame->calls++;
    GPUCall* call = &state.frame->callv[handle];
    *call = (GPUCall){ .node = node, .key = 0 };
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUCall (key=%u, vb=%u, eb=%u, pipeline=%u)",
        call->key, call->node.vertexBuffer, call->node.elementBuffer, call->node.pipeline);
    return handle;
}

none _render(none) {
    GPUFrame* frame = state.frame;
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] BEGIN GPUFrame (handle=%u, calls=%u, phases=%u)", frame->handle, frame->calls, frame->phases);
    SSDK_FORI(0, frame->phases, 1) {
        state.phase = &frame->phasev[i];
        saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUPhase (handle=%u, type=%d, framebuffer=%u)",
            state.phase->handle, state.phase->type, state.phase->framebuffer);
        saneMath->vector.log3("[Renderer] GPUPhase Clear Color", state.phase->clearColor);
        saneMath->vector.log3("[Renderer] GPUPhase Clear Depth", state.phase->clearDepth);

        SSDK_FORJ(0, frame->calls, 1) {
            GPUCall* call = &frame->callv[j];
            GPUNode* node = &call->node;
            if (node->phase != state.phase->handle) continue;
            
            GPUBuffer* elementBuffer = &resources.bufferv[node->elementBuffer];
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Node Element Buffer Handle: %u", node->elementBuffer);
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Element Buffer: (handle=%u, size=%d, object=%u, elements=%u)",
                elementBuffer->handle, elementBuffer->size, elementBuffer->data.element.ebo, elementBuffer->data.element.count);
            SSDK_FORI(0, elementBuffer->data.element.count, 1) {
                saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Element Buffer: (element)%u", elementBuffer->data.element.elements[i]);
            }

            GPUBuffer* vertexBuffer = &resources.bufferv[node->vertexBuffer];
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Node Vertex Buffer Handle: %u", node->vertexBuffer);
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Vertex Buffer: (handle=%u, size=%d, object=%u, verts=%u)",
                vertexBuffer->handle, vertexBuffer->size, vertexBuffer->data.vertex.vbo, vertexBuffer->data.vertex.count);
            SSDK_FORI(0, vertexBuffer->data.vertex.count, 1) {
                saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Vertex Buffer: (vertex)%0.1f", vertexBuffer->data.vertex.vertices[i]);
            }
            
            saneLog->logFmt(
                SANE_LOG_INFO, "[Renderer] GPUCall (phase=%d, vb=%u, eb=%u, pipeline=%u)",
                state.phase->type, node->vertexBuffer, node->elementBuffer, node->pipeline
            );
        }
    }
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] END GPUFrame (handle=%u, calls=%u, phases=%u)", frame->handle, frame->calls, frame->phases);
}

none _commitFrame(GPUHandle handle) {
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Commited GPUFrame (current=%u, next=%u)", handle, resources.frames);
}

none _initRenderer(GPUBackend backend) {
    state.backend = backend;
    switch(backend) {
        case GPU_BACKEND_OPENGL: {
            gl = saneMemory->alloc(sizeof(GLAPI), 8);
            if (gl == NULL || !_initGLAPI()) {
                saneLog->log(SANE_LOG_ERROR, "[GLAPI] Initialization Failed");
                return;
            } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Initialized");
        } break;
        case GPU_BACKEND_VULKAN:    // fall-through
        case GPU_BACKEND_DIRECTX:   // fall-through
        case GPU_BACKEND_INVALID:   // fall-through
        default: break;
    }

    if (!saneMemory->createAllocator(SANE_ALLOC_ARENA, SSDK_KB, 8, &defaultFrame.arena)) {
        saneLog->log(SANE_LOG_ERROR, "[Renderer] Initialization Failed | Error Creating Arena Allocator");
        return;
    }
    
    saneLog->log(SANE_LOG_SUCCESS, "[Renderer] Initialized");
}

none _shutdownRenderer(none) {
    switch(state.backend) {
        case GPU_BACKEND_OPENGL: {
            saneMemory->dealloc(gl);
            swarmPlatform->destroyGLContext();
            saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Shutdown");
        }
        case GPU_BACKEND_VULKAN:    // fall-through
        case GPU_BACKEND_DIRECTX:   // fall-through
        case GPU_BACKEND_INVALID:   // fall-through
        default: break;
    }

    saneMemory->destroyAllocator(&defaultFrame.arena);

    saneLog->log(SANE_LOG_SUCCESS, "[Renderer] Shutdown");
    swarmRenderer = NULL;
}

Renderer* swarmRenderer = &(Renderer){
    .init = _initRenderer,
    .render = _render,
    .shutdown = _shutdownRenderer,
    
    .createBuffer = _createBuffer,
    .createTexture = _createTexture,
    .createProgram = _createProgram,
    .createPipeline = _createPipeline,
    
    .createCall = _createCall,
    .createFrame = _createFrame,
    .createPhase = _createPhase,
    .commitFrame = _commitFrame,

    .bindBuffer = NULL,
    .bindProgram = NULL,
    .bindTexture = NULL,
    .bindPipeline = NULL
};
