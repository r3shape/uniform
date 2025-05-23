#include <include/swarm/core/swarm_renderer.h>
#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/GPUAPI.h>
#include <include/swarm/core/GLAPI.h>

// GPU BACKENDS
GLAPI* gl = NULL;

// GPU STATE
GPUAPI* swarmGPU = NULL;
GPUFrame defaultFrame = { .buffer.type = GPU_BUFFER_FRAME };
GPUPhase defaultPhase = {0};
GPUResources resources = (GPUResources){0};
GPUState state = (GPUState){ .frame = &defaultFrame, .phase = &defaultPhase };

GPUHandle _createProgram(str vs, str fs) {
    GPUHandle handle = resources.programs++;
    GPUProgram* prog = &resources.programv[handle];
    *prog = (GPUProgram){
        .vertex = saneFile->read(vs, 0), .fragment = saneFile->read(fs, 0), .handle = handle
    }; swarmGPU->createProgram(prog);
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUProgram (handle=%u)", handle);
    return handle;
}

GPUHandle _createBuffer(GPUBufferType type, u32 format, u32 size, ptr data) {
    GPUHandle handle = resources.buffers++;
    GPUBuffer* buf = &resources.bufferv[handle];

    switch(type) {
        case(GPU_BUFFER_ELEMENT): {
            *buf = (GPUBuffer){
                .type = type,
                .size = size,
                .handle = handle,
                .data.element.ebo = 0,
                .data.element.elements = data,
                .data.element.count = size/sizeof(u32)
            }; swarmGPU->createElementBuffer(buf);

            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUBuffer (elements=%u, size=%d)", buf->data.element.count, size);
        } break;
        case(GPU_BUFFER_VERTEX): {
            *buf = (GPUBuffer){
                .type = type,
                .size = size,
                .handle = handle,
                .data.vertex.vbo = 0,
                .data.vertex.vertices = data,
                .data.vertex.format = format,
                .data.vertex.count = size/sizeof(f32)
            }; swarmGPU->createVertexBuffer(buf);

            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUBuffer (vertices=%u, size=%d)", buf->data.vertex.count, size);
        } break;
        case(GPU_BUFFER_TEXTURE): {
            *buf = (GPUBuffer){
                .type = type,
                .size = size,
                .handle = handle,
                .data.texture.tbo = 0,
                .data.texture.width = 0,
                .data.texture.height = 0,
                .data.texture.path = NULL,
                .data.texture.data = data,
                .data.texture.format = format,
            }; swarmGPU->createTextureBuffer(buf);

            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUBuffer (format=%u, width=%i, height=%i)", buf->data.texture.format, buf->data.texture.width, buf->data.texture.height);
        } break;
        case(GPU_BUFFER_FRAME): {
            *buf = (GPUBuffer){
                .type = type,
                .size = size,
                .handle = handle,
            }; swarmGPU->createFrameBuffer(buf);
        } break;
        case(GPU_BUFFER_UNIFORM):   // fall-through
        case(GPU_BUFFER_INVALID):   // fall-through
        default: break;
    }
    
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUBuffer (type=%d, handle=%u, size=%u)", type, handle, size);
    return handle;
}

GPUHandle _createPipeline(GPUHandle program, u32 mask) {
    GPUHandle handle = resources.pipelines++;
    GPUPipeline* pipe = &resources.pipelinev[handle];
    *pipe = (GPUPipeline){ .program = program, .mask = mask, .handle = handle, .binds = 0 };
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUPipeline (program=%u, handle=%u, mask=0x%X)", program, handle, mask);
    return handle;
}

none _pipeBindBufferImpl(GPUHandle buffer, GPUHandle pipeline) {
    if (buffer >= SWARM_GPU_RESOURCE_MAX || pipeline >= SWARM_GPU_RESOURCE_MAX) return;
    
    GPUPipeline* pipe = &resources.pipelinev[pipeline];
    if (pipe->binds + 1 >= 4) {
        saneLog->logFmt(SANE_LOG_ERROR, "[Renderer] Pipeline reched maxed binds (pipeline=%u, buffer=%u)", pipeline, buffer);
        return;
    }
    
    pipe->bindv[pipe->binds++] = buffer;
}

none _pipeBindProgramImpl(GPUHandle program, GPUHandle pipeline) {
    if (program >= SWARM_GPU_RESOURCE_MAX || pipeline >= SWARM_GPU_RESOURCE_MAX) return;
    GPUPipeline* pipe = &resources.pipelinev[pipeline];
    pipe->program = program;
}

none _createFrame(none) {
    state.frame->calls = 0;
    state.frame->phases = 0;
    state.frame->handle = resources.frames++;
    state.frame->buffer.handle = state.frame->handle;

    if (!state.frame->buffer.data.frame.fbo) {
        swarmGPU->createFrameBuffer(&state.frame->buffer);
    }
    
    saneMemory->arenaReset(&state.frame->arena);
    memset(state.frame->callv,  0, sizeof(state.frame->callv));
    memset(state.frame->phasev, 0, sizeof(state.frame->phasev));
    
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUFrame (handle=%u)", state.frame->handle);
}

GPUHandle _createPhase(GPUPhaseType type, f32 clearDepth, Vec3 clearColor) {
    GPUHandle handle = state.frame->phases++;
    GPUPhase* phase = &state.frame->phasev[handle];
    switch(type) {
        case(GPU_PHASE_OPAQUE): {
            *phase = (GPUPhase){ .handle = handle, .type = type, .data.color = clearColor};
        } break;
        case(GPU_PHASE_DEPTH): {
            *phase = (GPUPhase){ .handle = handle, .type = type, .data.depth = clearDepth};
        } break;
        case(GPU_PHASE_LIGHT):      // fall-through
        case(GPU_PHASE_SHADOW):     // fall-through
        case(GPU_PHASE_INVALID):    // fall-through
        default: break;
    }
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUPhase (handle=%u, type=%d)", phase->handle, phase->type);
    return handle;
}

GPUHandle _createCall(GPUNode node) {
    if (node.uniforms > 16) node.uniforms = 16;

    GPUHandle handle = state.frame->calls++;
    GPUCall* call = &state.frame->callv[handle];
    *call = (GPUCall){ .node = node, .key = 0 };

    GPUPipeline* pipeline = &resources.pipelinev[node.pipeline];
    GPUProgram* program = &resources.programv[pipeline->program];
    SSDK_FORI(0, node.uniforms, 1) {
        swarmGPU->setUniform(&call->node.uniformv[i], program);
    }

    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Created GPUCall (key=%u, vb=%u, eb=%u, pipeline=%u)",
        call->key, call->node.vertexBuffer, call->node.elementBuffer, call->node.pipeline);
    return handle;
}

f32 screenQuad[] = {
    // location   // tcoord
    -1.0f,  1.0f, 0.0,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0,  0.0f, 0.0f,
    1.0f, -1.0f, 0.0,  1.0f, 0.0f,
    
    -1.0f,  1.0f, 0.0,  0.0f, 1.0f,
    1.0f, -1.0f, 0.0,  1.0f, 0.0f,
    1.0f,  1.0f, 0.0,  1.0f, 1.0f
};
static GPUHandle screenQuadBuffer = SWARM_GPU_RESOURCE_MAX;
static GPUHandle screenQuadProgram = SWARM_GPU_RESOURCE_MAX;
none _render(none) {
    if (screenQuadBuffer == SWARM_GPU_RESOURCE_MAX) {
        screenQuadProgram = swarmRenderer->createProgram(
            "external/assets/shaders/default/screen.vert",
            "external/assets/shaders/default/screen.frag"
        );
        screenQuadBuffer = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, GPU_VERTEX_LOCATION|GPU_VERTEX_TEX, sizeof(screenQuad), screenQuad);
    }

    GPUFrame* frame = state.frame;
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] BEGIN GPUFrame (handle=%u, calls=%u, phases=%u)", frame->handle, frame->calls, frame->phases);
    SSDK_FORI(0, frame->phases, 1) {
        state.phase = &frame->phasev[i];
        saneLog->logFmt(
            SANE_LOG_INFO,
            "[Renderer] GPUPhase (handle=%u, type=%d, framebuffer=%u)",
            state.phase->handle, state.phase->type, frame->buffer.data.frame.fbo
        );

        // bind the framebuffer
        swarmGPU->bindBuffer(&frame->buffer);
        
        if (state.phase->type == GPU_PHASE_DEPTH) {
            if (state.backend == GPU_BACKEND_OPENGL) gl->enable(GL_DEPTH_TEST);
            swarmGPU->clearDepthBuffer(state.phase->data.depth);
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] GPUPhase Clear Depth: %0.1f", state.phase->data.depth);
        } else {
            swarmGPU->clearColorBuffer(state.phase->data.color);
            saneMath->vector.log3("[Renderer] GPUPhase Clear Color", state.phase->data.color);
        }
        
        SSDK_FORJ(0, frame->calls, 1) {
            GPUCall* call = &frame->callv[j];
            GPUNode* node = &call->node;
            if (node->phase != state.phase->handle) continue;

            GPUBuffer* elementBuffer = &resources.bufferv[node->elementBuffer];
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Node Element Buffer (handle=%u, size=%u)", node->elementBuffer, elementBuffer->size);

            GPUBuffer* vertexBuffer = &resources.bufferv[node->vertexBuffer];
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Node Vertex Buffer (handle=%u, size=%u)", node->vertexBuffer, vertexBuffer->size);

            GPUPipeline* pipeline = &resources.pipelinev[node->pipeline];
            saneLog->logFmt(SANE_LOG_INFO, "[Renderer] Pipeline: (handle=%u, program=%u)", pipeline->handle, pipeline->program);

            SSDK_FOR(GPUHandle, b, 0, pipeline->binds, 1) {
                swarmGPU->bindBuffer(&resources.bufferv[pipeline->bindv[b]]);
            }

            GPUProgram* program = &resources.programv[pipeline->program];
            swarmGPU->bindProgram(program);

            SSDK_FOR(GPUHandle, u, 0, node->uniforms, 1) {
                swarmGPU->sendUniform(node->uniformv[u].name, program);
            }
            
            swarmGPU->bindBuffer(vertexBuffer);
            if (elementBuffer->data.element.elements != NULL) {
                swarmGPU->readBuffer(elementBuffer);
            } else swarmGPU->readBuffer(vertexBuffer);
            
            saneLog->logFmt(
                SANE_LOG_INFO, "[Renderer] GPUCall (phase=%d, vb=%u, eb=%u, pipeline=%u, program=%u)",
                state.phase->type, node->vertexBuffer, node->elementBuffer, node->pipeline, program->program
            );
        }
        
        // bind default framebuffer
        swarmGPU->bindBuffer(&(GPUBuffer){
            .type = GPU_BUFFER_FRAME,
            .data.frame.fbo = 0
        });

        // preserve the screen-space quad from depth-test discard
        if (state.backend == GPU_BACKEND_OPENGL) gl->disable(GL_DEPTH_TEST);
        
        saneMath->vector.log3("[Renderer] GPUPhase Clear Color", state.phase->data.color);
        swarmGPU->clearColorBuffer(state.phase->data.color);
        
        swarmGPU->bindBuffer(&(GPUBuffer){
            .type = GPU_BUFFER_TEXTURE,
            .data.texture.slot = 0,
            .data.texture.type = GPU_TEXTURE_PLANE,
            .data.texture.tbo = frame->buffer.data.frame.tbo
        });
        
        // render screen quad sampling current phase's frame buffer color buffer
        swarmGPU->bindProgram(&resources.programv[screenQuadProgram]);
        swarmGPU->bindBuffer(&resources.bufferv[screenQuadBuffer]);
        swarmGPU->readBuffer(&resources.bufferv[screenQuadBuffer]);
    }
    
    saneLog->logFmt(SANE_LOG_INFO, "[Renderer] END GPUFrame (handle=%u, calls=%u, phases=%u)", frame->handle, frame->calls, frame->phases);
}

GPUProgram* _getProgramImpl(GPUHandle handle) {
    if (handle >= SWARM_GPU_RESOURCE_MAX) return NULL;
    return &resources.programv[handle];
}

GPUBuffer* _getBufferImpl(GPUHandle handle) {
    if (handle >= SWARM_GPU_RESOURCE_MAX) return NULL;
    return &resources.bufferv[handle];
}

none _initRenderer(GPUBackend backend) {
    state.backend = backend;
    switch(backend) {
        case GPU_BACKEND_OPENGL: {
            saneLog->log(SANE_LOG_INFO, "[Renderer] GPU Backend: OpenGL"); 
            if (!_initGLAPI()) {
                saneLog->log(SANE_LOG_ERROR, "[Renderer] Backend Initialization Failed");
                return;
            } else {
                // CONFIGURE OPENGL
                gl->enable(GL_BLEND);
                gl->blendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                saneLog->log(SANE_LOG_SUCCESS, "[Renderer] Backend Initialized");
            }
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

    swarmGPU->getBuffer = _getBufferImpl;
    swarmGPU->getProgram = _getProgramImpl;
    
    saneLog->log(SANE_LOG_SUCCESS, "[Renderer] Initialized");
}

none _shutdownRenderer(none) {
    switch(state.backend) {
        case GPU_BACKEND_OPENGL: {
            _exitGLAPI();
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
    .createProgram = _createProgram,
    .createPipeline = _createPipeline,
    
    .createCall = _createCall,
    .createFrame = _createFrame,
    .createPhase = _createPhase,

    .bindPipelineBuffer = _pipeBindBufferImpl,
    .bindPipelinerogram = _pipeBindProgramImpl
};
