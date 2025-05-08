#ifndef __SWARM_GLAPI_H__
#define __SWARM_GLAPI_H__

#include <GL/gl.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/include/stb_image.h>

#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/swarm_renderer.h>

// GL equivalent defines
#define GLAPI_STATIC_DRAW                0x88E4
#define GLAPI_ARRAY_BUFFER               0x8892
#define GLAPI_ELEMENT_BUFFER             0x8893

#define GLAPI_VERTEX_SHADER              0x8B31
#define GLAPI_FRAGMENT_SHADER            0x8B30
#define GLAPI_LINK_STATUS                0x8B82
#define GLAPI_COMPILE_STATUS             0x8B81
#define GLAPI_ACTIVE_ATTRIBUTES          0x8B89
#define GLAPI_SHADING_LANGUAGE_VERSION   0x8B8C

#define GLAPI_TEXT_UNIT0                 0x84C0
#define GLAPI_TEXT_UNIT1                 0x84C1
#define GLAPI_TEXT_UNIT2                 0x84C2
#define GLAPI_TEXT_UNIT3                 0x84C3
#define GLAPI_ACTIVE_TEXTURE             0x84E0
#define GLAPI_MAX_TEXTURE_UNITS          0x84E2

#define GLAPI_LINE                       0x1B01
#define GLAPI_FRONT_AND_BACK             0x0408

typedef struct GLAPI {
    // BUFFER FUNCTIONS
    void (*genBuffers)(i32 count, u32 *buffer);
    void (*bindBuffer)(u32 target, u32 buffer);
    void (*bufferData)(u32 target, i32 size, const void *data, u32 usage);
    
    // VERTEX ATTRIBUTE FUNCTIONS
    void (*deleteBuffers)(i32 count, const u32 *buffer);
    void (*mapBuffer)(u32 target, u32 access);
    void (*unmapBuffer)(u32 target);
    void (*bufferSubData)(u32 target, u32* offset, i32 size, const void *data);
    void (*getBufferParameteriv)(u32 target, u32 pname, u32 *params);
    
    // SHADER FUNCTIONS        
    void (*enableVertexAttribArray)(u32 index);
    void (*disableVertexAttribArray)(u32 index);
    void (*vertexAttribPointer)(u32 index, u32 size, u32 type, u8 normalized, i32 stride, const void *pointer);
    void (*bindVertexArray)(u32 array);
    void (*genVertexArrays)(i32 count, u32 *arrays);
    void (*deleteVertexArrays)(i32 count, const u32 *arrays);
    
    // TEXTURE FUNCTIONS        
    u32 (*createProgram)(void);
    void (*useProgram)(u32 program);
    void (*linkProgram)(u32 program);
    void (*deleteProgram)(u32 program);
    
    u32 (*createShader)(u32 type);
    void (*compileShader)(u32 shader);
    void (*attachShader)(u32 program, u32 shader);
    void (*detachShader)(u32 program, u32 shader);
    void (*deleteShader)(u32 shader);
    void (*shaderSource)(u32 shader, i32 count, const char **strings, const int *lengths);
    
    void (*getShaderiv)(u32 shader, u32 pname, u32 *params);
    void (*getShaderInfoLog)(u32 shader, i32 maxLength, i32 *length, char *infoLog);
    void (*getProgramiv)(u32 program, u32 pname, u32 *params);
    void (*getProgramInfoLog)(u32 program, i32 maxLength, i32 *length, char *infoLog);
    u32 (*getUniformLocation)(u32 program, const char* name);
    
    void (*uniform1f)(u32 location, f32 value);
    void (*uniform2fv)(u32 location, u32 count, const f32* value);
    void (*uniform3fv)(u32 location, u32 count, const f32* value);
    void (*uniform4fv)(u32 location, u32 count, const f32* value);
    void (*uniformMatrix4fv)(u32 location, u32 count, u32 transpose, const f32* value);
    
    void (*genTextures)(i32 count, u32 *textures);
    void (*bindTexture)(u32 target, u32 texture);
    void (*texParameteri)(u32 target, u32 pname, u32 param);
    void (*texImage2D)(u32 target, u32 level, u32 internalFormat, i32 width, i32 height, u32 border, u32 format, u32 type, const void *data);
    void (*activeTexture)(u32 texture);
    void (*deleteTextures)(i32 count, const u32 *textures);
    void (*generateMipmap)(u32 target);
    void (*generateTextureMipmap)(u32 texture);
    
    void (*genFramebuffers)(i32 count, u32 *framebuffers);
    void (*bindFramebuffer)(u32 target, u32 framebuffer);
    void (*framebufferTexture2d)(u32 target, u32 attachment, u32 textarget, u32 texture, u32 level);
    void (*deleteFramebuffers)(i32 count, const u32 *framebuffers);
    u32 (*checkFramebufferStatus)(u32 target);
    
    void (*clear)(i32 mask);
    void (*drawArrays)(u32 mode, u32 first, i32 count);
    void (*clearColor)(f32 r, f32 g, f32 b, f32 a);
    void (*drawElements)(u32 mode, i32 count, u32 type, const void *indices);
    void (*drawElementsBaseVertex)(u32 mode, i32 count, u32 type, const void *indices, u32 baseVertex);
    
    // STATE MANAGEMENT FUNCTIONS        
    void (*enable)(u32 cap);
    void (*disable)(u32 cap);
    void (*blendFunc)(u32 sfactor, u32 dfactor);
    void (*cullFace)(u32 mode);
    void (*depthFunc)(u32 func);
    void (*viewport)(u32 x, u32 y, i32 width, i32 height);
    void (*polygonMode)(u32 face, u32 mode);
    
    // UTILITY FUNCTIONS        
    u32 (*getError)(void);
    const u8 *(*getString)(const u32 name);
} GLAPI;
extern GLAPI* gl;
extern GPUAPI* swarmGPU;


// GPU API
none _clearColorBufferImpl(Vec3 color) {
    gl->clearColor(color.x/255, color.y/255, color.z/255, 1.0);
    gl->clear(GL_COLOR_BUFFER_BIT);
}

none _clearDepthBufferImpl(Vec3 depth) {
    gl->clearColor(depth.x/255, depth.y/255, depth.z/255, 1.0);
    gl->clear(GL_DEPTH_BUFFER_BIT);
}


none _createProgramImpl(GPUProgram* program) {
    u32 link = 0;
    u32 compile = 0;
    
    saneLog->logFmt(SANE_LOG_WARN, "[GLAPI] Raw vertex shader: %s", program->vertex);
    saneLog->logFmt(SANE_LOG_WARN, "[GLAPI] Raw fragment shader: %s", program->fragment);

    saneData->hashArray.create(32, &program->uniforms);
    if (!program->uniforms.map) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to allocate uniform HashArray");
        return; // error: failed to allocate uniform hashmap!
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Allocated uniform HashArray");
    
    program->program = gl->createProgram();
    i32 v_Shader = gl->createShader(GLAPI_VERTEX_SHADER);
    i32 f_Shader = gl->createShader(GLAPI_FRAGMENT_SHADER);
    
    gl->shaderSource(v_Shader, 1, &(cstr){program->vertex}, NULL);
    gl->compileShader(v_Shader);
    gl->getShaderiv(v_Shader, GLAPI_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile vertex-shader!
        saneData->hashArray.destroy(&program->uniforms);
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to compile vertex shader");
        return;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Compiled vertex shader");

    gl->shaderSource(f_Shader, 1, &(cstr){program->fragment}, NULL);
    gl->compileShader(f_Shader);
    gl->getShaderiv(f_Shader, GLAPI_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile fragment-shader!
        saneData->hashArray.destroy(&program->uniforms);
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to compile fragment shader");
        return;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Compiled fragment shader");
    
    gl->attachShader(program->program, v_Shader);
    gl->attachShader(program->program, f_Shader);
    gl->linkProgram(program->program);
    gl->getProgramiv(program->program, GLAPI_LINK_STATUS, &link);
    if (!link) {
        // error: failed to link shader program!
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to link shader");
        saneData->hashArray.destroy(&program->uniforms);
        return;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Linked shader");
    
    gl->deleteShader(v_Shader);
    gl->deleteShader(f_Shader);
}

none _destroyProgramImpl(GPUProgram* program) {
    gl->deleteProgram(program->program);
}

none _setUniformImpl(GPUUniform* uniform, GPUProgram* program) {
    if (!program || !program->program) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to set uniform (invalid program)");
        return;  // error: null ptr!
    }

    if (!uniform->name || uniform->type < 0 || uniform->type >= GPU_UNIFORM_TYPES) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to set uniform (invalid uniform)");
        return;  // error: uniform value error!
    }
    
    uniform->location = gl->getUniformLocation(program->program, uniform->name);
    if (!saneData->hashArray.put(uniform->name, uniform, &program->uniforms)) {
        saneLog->logFmt(SANE_LOG_ERROR, "[GLAPI] Failed to set uniform: %s", uniform->name);
    } else saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Set uniform (name=%s, location=%u)", uniform->name, uniform->location);
}

none _sendUniformImpl(cstr name, GPUProgram* program) {
    if (!program || !name) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to send uniform (invalid program/uniform name)");
        return;    // error: null ptr!
    }
    
    GPUUniform* uniform = saneData->hashArray.get(name, &program->uniforms);
    if (!uniform) {
        saneLog->logFmt(SANE_LOG_ERROR, "[GLAPI] Failed to send uniform: %s", name);
        return;  // error: failed to find uniform in hashmap!
    }

    switch (uniform->type) {
        case GPU_UNIFORM_NONE: break;
        case GPU_UNIFORM_TYPES: break;
        case GPU_UNIFORM_FLOAT: gl->uniform1f(uniform->location, uniform->value.f);   break;
        case GPU_UNIFORM_VEC2: gl->uniform2fv(uniform->location, 1, (f32*)&uniform->value.v2.x); break;
        case GPU_UNIFORM_VEC3: gl->uniform3fv(uniform->location, 1, (f32*)&uniform->value.v3.x); break;
        case GPU_UNIFORM_VEC4: gl->uniform4fv(uniform->location, 1, (f32*)&uniform->value.v4.x); break;
        case GPU_UNIFORM_MAT4: gl->uniformMatrix4fv(uniform->location, 1, 0, (f32*)&uniform->value.m4.m[0]); break;
        default: break;
    }

    saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Sent uniform (name=%s, location=%u)", uniform->name, uniform->location);
    return;
}


none _createVertexBufferImpl(u8 format, GPUBuffer* buffer) {
    if (!buffer || !buffer->data.vertex.vertices || !format) return;
    if ((format & ~((1 << GPU_VERTEX_ATTRIBS) - 1)) != 0 || !buffer->data.vertex.count || !buffer->data.vertex.vertices) {
        return;
    }

    gl->genVertexArrays(1, &buffer->data.vertex.vao);
    gl->genBuffers(1, &buffer->data.vertex.vbo);

    gl->bindVertexArray(buffer->data.vertex.vao);
    gl->bindBuffer(GLAPI_ARRAY_BUFFER, buffer->data.vertex.vbo);
    
    gl->bufferData(GLAPI_ARRAY_BUFFER, buffer->size, buffer->data.vertex.vertices, GLAPI_STATIC_DRAW);

    // configure vertex attributes
    u32 stride = 0;
    u32 offsets[GPU_VERTEX_ATTRIBS] = {0};
    const u32 attr_sizes[GPU_VERTEX_ATTRIBS] = {
        3, // GPU_VERTEX_LOCATION:  vec3
        3, // GPU_VERTEX_COLOR:     vec3
        2, // GPU_VERTEX_TEX:       vec2
        3  // GPU_VERTEX_NORMAL:    vec3
    };

    for (int i = 0; i < GPU_VERTEX_ATTRIBS; i++) {
        if ((format & (1 << i)) != 0) {
            // accumulate stride for enabled vertex attributes
            offsets[i] = stride;
            stride += attr_sizes[i];
        }
    }

    buffer->data.vertex.count = buffer->data.vertex.count/stride;

    for (int i = 0; i < GPU_VERTEX_ATTRIBS; i++) {
        if ((format & (1 << i)) != 0) {
            gl->vertexAttribPointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            gl->enableVertexAttribArray(i);
        }
    }

    gl->bindBuffer(GLAPI_ARRAY_BUFFER, 0);
    gl->bindVertexArray(0);
}

none _destroyVertexBufferImpl(GPUBuffer* buffer) {
    if (!buffer || !buffer->data.vertex.vertices) return;  // error: null ptr!
    gl->deleteVertexArrays(1, &buffer->data.vertex.vao);
    gl->deleteBuffers(1, &buffer->data.vertex.vbo);
    buffer->data.vertex.vao = 0;
    buffer->data.vertex.vbo = 0;
    buffer->data.vertex.count = 0;
    buffer->data.vertex.vertices = NULL;
}


none _createElementBufferImpl(GPUBuffer* buffer) {
    if (!buffer || !buffer->data.element.elements) return;  // error: null ptr!
    gl->genBuffers(1, &buffer->data.element.ebo);
    gl->bindBuffer(GLAPI_ELEMENT_BUFFER, buffer->data.element.ebo);
    gl->bufferData(GLAPI_ELEMENT_BUFFER, buffer->size, buffer->data.element.elements, GLAPI_STATIC_DRAW);
    gl->bindBuffer(GLAPI_ELEMENT_BUFFER, 0);
}

none _destroyElementBufferImpl(GPUBuffer* buffer) {
    if (!buffer || !buffer->data.element.elements) return;  // error: null ptr!
    gl->deleteVertexArrays(1, &buffer->data.element.ebo);
    buffer->data.element.ebo = 0;
    buffer->data.element.count = 0;
    buffer->data.element.elements = NULL;
}


none _createTexture2DImpl(GPUTexture* texture) {
    // GLAPI_Texture texture = { .id = 0, .width = 0, .height = 0, .channels = 0, .path = NULL, .raw = NULL };
    // if (!path) return texture;

    // texture.path = path;
    // stbi_set_flip_vertically_on_load(SSDK_TRUE);
    // texture.raw = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
    // if (!texture.raw) return texture;    // error: failed to allocate raw data buffer!

    // gl->genTextures(1, &texture.id);
    // gl->bindTexture(GL_TEXTURE_2D, texture.id);

    // // set texture wrapping options
    // gl->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x axis
    // gl->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y axis

    // // set texture filtering options (scaling up/down)
    // gl->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // gl->texParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // // generate the texture
    // gl->texImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.raw);
    // gl->generateMipmap(GL_TEXTURE_2D);

    // return texture;
}

none _destroyTexture2DImpl(GPUTexture* texture) {
    // texture->width = 0;
    // texture->height = 0;
    // texture->channels = 0;
    // texture ->path = NULL;
    // gl->deleteTextures(1, &texture->id);
    // stbi_image_free(texture->raw);
    // texture->id = 0;
}


none _bindBufferImpl(GPUBuffer* buffer) {
    if (!buffer) return;    // error: null ptr!
    switch(buffer->type) {
        case(GPU_BUFFER_ELEMENT): {
            gl->bindBuffer(GLAPI_ELEMENT_BUFFER, buffer->data.element.ebo);
        } break;
        case(GPU_BUFFER_VERTEX): {
            gl->bindVertexArray(buffer->data.vertex.vao);
            gl->bindBuffer(GLAPI_ARRAY_BUFFER, buffer->data.vertex.vbo);
        } break;
        case(GPU_BUFFER_UNIFORM):   // fall-through
        case(GPU_BUFFER_TEXTURE):   // fall-through
        case(GPU_BUFFER_RENDER):   // fall-through
        case(GPU_BUFFER_FRAME):   // fall-through
        case(GPU_BUFFER_INVALID):   // fall-through
        default: break;
    }
}

none _bindProgramImpl(GPUProgram* program) {
    if (!program || !program->program) return;  // error: null ptr/value error!
    gl->useProgram(program->program);
    saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Bound (program=%u)", program->program);
}

none _renderBufferImpl(GPUBuffer* buffer) {
    if (!buffer) return;    // error: null ptr!

    switch(buffer->type) {
        case(GPU_BUFFER_ELEMENT): {
            saneLog->logFmt(SANE_LOG_WARN, "[GLAPI] Rendering Element Buffer (count=%u)", buffer->data.element.count);
            gl->drawElements(GPU_TRIANGLE_MODE, buffer->data.element.count, GL_UNSIGNED_INT, buffer->data.element.elements);
        } break;
        case(GPU_BUFFER_VERTEX): {
            saneLog->logFmt(SANE_LOG_WARN, "[GLAPI] Rendering Vertex Buffer (count=%u)", buffer->data.vertex.count);
            gl->drawArrays(GPU_TRIANGLE_MODE, 0, buffer->data.vertex.count);
        } break;
        case(GPU_BUFFER_UNIFORM):   // fall-through
        case(GPU_BUFFER_TEXTURE):   // fall-through
        case(GPU_BUFFER_RENDER):   // fall-through
        case(GPU_BUFFER_FRAME):   // fall-through
        case(GPU_BUFFER_INVALID):   // fall-through
        default: break;
    }
}
// GPU API


byte _initGLAPI(none) {
    if (!gl) return SSDK_FALSE;   // error: null ptr!
    
    swarmGPU = malloc(sizeof(GPUAPI));
    if (!swarmGPU) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to allocate GPU API");
        return SSDK_FALSE;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Allocated GPU API");
    
    if (!swarmPlatform->createGLContext()) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to load context");
        return SSDK_FALSE;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Loaded Context");
    
    SwarmLib lib = swarmPlatform->loadLib(NULL, "opengl32");
    if (!lib.handle) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to load OpenGL");
        return SSDK_FALSE;  // error: failed to load opengl library!
    } else saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Loaded OpenGL v%s", glGetString(GL_VERSION));

    struct gl_func {
        ptr* function;
        cstr name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&gl->genBuffers, "glGenBuffers"},
        {(void**)&gl->bindBuffer, "glBindBuffer"},
        {(void**)&gl->bufferData, "glBufferData"},
        {(void**)&gl->mapBuffer, "glMapBuffer"},
        {(void**)&gl->unmapBuffer, "glUnmapBuffer"},
        {(void**)&gl->bufferSubData, "glBufferSubData"},
        {(void**)&gl->getBufferParameteriv, "glGetBufferParameteriv"},
        {(void**)&gl->deleteBuffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&gl->enableVertexAttribArray, "glEnableVertexAttribArray"},
        {(void**)&gl->disableVertexAttribArray, "glDisableVertexAttribArray"},
        {(void**)&gl->vertexAttribPointer, "glVertexAttribPointer"},
        {(void**)&gl->bindVertexArray, "glBindVertexArray"},
        {(void**)&gl->genVertexArrays, "glGenVertexArrays"},
        {(void**)&gl->deleteVertexArrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&gl->createProgram, "glCreateProgram"},
        {(void**)&gl->useProgram, "glUseProgram"},
        {(void**)&gl->linkProgram, "glLinkProgram"},
        {(void**)&gl->deleteProgram, "glDeleteProgram"},
        {(void**)&gl->createShader, "glCreateShader"},
        {(void**)&gl->compileShader, "glCompileShader"},
        {(void**)&gl->attachShader, "glAttachShader"},
        {(void**)&gl->detachShader, "glDetachShader"},
        {(void**)&gl->deleteShader, "glDeleteShader"},
        {(void**)&gl->shaderSource, "glShaderSource"},
        {(void**)&gl->getShaderiv, "glGetShaderiv"},
        {(void**)&gl->getShaderInfoLog, "glGetShaderInfoLog"},
        {(void**)&gl->getProgramiv, "glGetProgramiv"},
        {(void**)&gl->getProgramInfoLog, "glGetProgramInfoLog"},
        {(void**)&gl->getUniformLocation, "glGetUniformLocation"},
        
        {(void**)&gl->uniform1f, "glUniform1f"},
        {(void**)&gl->uniform2fv, "glUniform2fv"},
        {(void**)&gl->uniform3fv, "glUniform3fv"},
        {(void**)&gl->uniform4fv, "glUniform4fv"},
        {(void**)&gl->uniformMatrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&gl->genTextures, "glGenTextures"},
        {(void**)&gl->bindTexture, "glBindTexture"},
        {(void**)&gl->texParameteri, "glTexParameteri"},
        {(void**)&gl->texImage2D, "glTexImage2D"},
        {(void**)&gl->activeTexture, "glActiveTexture"},
        {(void**)&gl->deleteTextures, "glDeleteTextures"},
        {(void**)&gl->generateMipmap, "glGenerateMipmap"},
        {(void**)&gl->generateTextureMipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&gl->genFramebuffers, "glGenFramebuffers"},
        {(void**)&gl->bindFramebuffer, "glBindFramebuffer"},
        {(void**)&gl->framebufferTexture2d, "glFramebufferTexture2D"},
        {(void**)&gl->deleteFramebuffers, "glDeleteFramebuffers"},
        {(void**)&gl->checkFramebufferStatus, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&gl->clear, "glClear"},
        {(void**)&gl->clearColor, "glClearColor"},
        {(void**)&gl->drawArrays, "glDrawArrays"},
        {(void**)&gl->drawElements, "glDrawElements"},
        {(void**)&gl->drawElementsBaseVertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&gl->enable, "glEnable"},
        {(void**)&gl->disable, "glDisable"},
        {(void**)&gl->blendFunc, "glBlendFunc"},
        {(void**)&gl->cullFace, "glCullFace"},
        {(void**)&gl->depthFunc, "glDepthFunc"},
        {(void**)&gl->viewport, "glViewport"},
        {(void**)&gl->polygonMode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&gl->getError, "glGetError"},
        {(void**)&gl->getString, "glGetString"}
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = swarmPlatform->loadSymbol(functions[i].name, &lib);
        if (*functions[i].function == NULL) {
            saneLog->logFmt(SANE_LOG_ERROR, "[GLAPI] Failed to load: %s", functions[i].name);
        } else saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Loaded: %s", functions[i].name);
    }
    
    swarmPlatform->unloadLib(&lib);

    swarmGPU->clearColorBuffer = _clearColorBufferImpl;
    swarmGPU->clearDepthBuffer = _clearDepthBufferImpl;

    swarmGPU->createProgram = _createProgramImpl;
    swarmGPU->destroyProgram = _destroyProgramImpl;
    
    swarmGPU->setUniform = _setUniformImpl;
    swarmGPU->sendUniform = _sendUniformImpl;
    
    swarmGPU->createVertexBuffer = _createVertexBufferImpl;
    swarmGPU->destroyVertexBuffer = _destroyVertexBufferImpl;
    
    swarmGPU->createElementBuffer = _createElementBufferImpl;
    swarmGPU->destroyElementBuffer = _destroyElementBufferImpl;
    
    swarmGPU->createTexture2D = NULL; // _createTexture2DImpl;
    swarmGPU->destroyTexture2D = NULL; // _destroyTexture2DImpl;
    
    swarmGPU->bindBuffer = _bindBufferImpl;
    swarmGPU->bindProgram = _bindProgramImpl;
    swarmGPU->renderBuffer = _renderBufferImpl;

    return SSDK_TRUE;
}

#endif  // __SWARM_GLAPI_H__