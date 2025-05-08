#ifndef __SWARM_GLAPI_H__
#define __SWARM_GLAPI_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/swarm_renderer.h>

// GL equivalent defines
#define GLAPI_STATIC_DRAW                0x88E4
#define GLAPI_ARRAY_BUFFER               0x8892
#define GLAPI_ELEMENT_ARRAY_BUFFER       0x8893

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
    none (*createProgram)(GPUProgram* program);
    none (*destroyProgram)(GPUProgram* program);

    none (*setUniform)(GPUProgram* program, GPUUniform uniform);
    none (*sendUniform)(GPUProgram* program, cstr name);

    none (*createVertexBuffer)(u8 format, u32 count, f32 *vertices, GPUHandle* vertexBuffer);
    none (*destroyVertexBuffer)(GPUHandle* vertexBuffer);
    
    none (*createElementBuffer)(u32 count, u32* elements, GPUHandle* elementBuffer);
    none (*destroyElementBuffer)(GPUHandle* elementBuffer);
    
    none (*createTexture2D)(cstr path, GPUTextureFormat format, GPUHandle* texture);
    none (*destroyTexture2D)(GPUHandle* texture);

    struct GL {
        // BUFFER FUNCTIONS
        void (*glGenBuffers)(i32 count, u32 *buffer);
        void (*glBindBuffer)(u32 target, u32 buffer);
        void (*glBufferData)(u32 target, i32 size, const void *data, u32 usage);
        
        // VERTEX ATTRIBUTE FUNCTIONS
        void (*glDeleteBuffers)(i32 count, const u32 *buffer);
        void (*glMapBuffer)(u32 target, u32 access);
        void (*glUnmapBuffer)(u32 target);
        void (*glBufferSubData)(u32 target, u32* offset, i32 size, const void *data);
        void (*glGetBufferParameteriv)(u32 target, u32 pname, u32 *params);
        
        // SHADER FUNCTIONS        
        void (*glEnableVertexAttribArray)(u32 index);
        void (*glDisableVertexAttribArray)(u32 index);
        void (*glVertexAttribPointer)(u32 index, u32 size, u32 type, u8 normalized, i32 stride, const void *pointer);
        void (*glBindVertexArray)(u32 array);
        void (*glGenVertexArrays)(i32 count, u32 *arrays);
        void (*glDeleteVertexArrays)(i32 count, const u32 *arrays);
        
        // TEXTURE FUNCTIONS        
        u32 (*glCreateProgram)(void);
        void (*glUseProgram)(u32 program);
        void (*glLinkProgram)(u32 program);
        void (*glDeleteProgram)(u32 program);
        
        u32 (*glCreateShader)(u32 type);
        void (*glCompileShader)(u32 shader);
        void (*glAttachShader)(u32 program, u32 shader);
        void (*glDetachShader)(u32 program, u32 shader);
        void (*glDeleteShader)(u32 shader);
        void (*glShaderSource)(u32 shader, i32 count, const char **strings, const int *lengths);
        
        void (*glGetShaderiv)(u32 shader, u32 pname, u32 *params);
        void (*glGetShaderInfoLog)(u32 shader, i32 maxLength, i32 *length, char *infoLog);
        void (*glGetProgramiv)(u32 program, u32 pname, u32 *params);
        void (*glGetProgramInfoLog)(u32 program, i32 maxLength, i32 *length, char *infoLog);
        u32 (*glGetUniformLocation)(u32 program, const char* name);
        
        void (*glUniform1f)(u32 location, f32 value);
        void (*glUniform2fv)(u32 location, u32 count, const f32* value);
        void (*glUniform3fv)(u32 location, u32 count, const f32* value);
        void (*glUniform4fv)(u32 location, u32 count, const f32* value);
        void (*glUniformMatrix4fv)(u32 location, u32 count, u32 transpose, const f32* value);
        
        void (*glGenTextures)(i32 count, u32 *textures);
        void (*glBindTexture)(u32 target, u32 texture);
        void (*glTexParameteri)(u32 target, u32 pname, u32 param);
        void (*glTexImage2D)(u32 target, u32 level, u32 internalFormat, i32 width, i32 height, u32 border, u32 format, u32 type, const void *data);
        void (*glActiveTexture)(u32 texture);
        void (*glDeleteTextures)(i32 count, const u32 *textures);
        void (*glGenerateMipmap)(u32 target);
        void (*glGenerateTextureMipmap)(u32 texture);
        
        void (*glGenFramebuffers)(i32 count, u32 *framebuffers);
        void (*glBindFramebuffer)(u32 target, u32 framebuffer);
        void (*glFramebufferTexture2d)(u32 target, u32 attachment, u32 textarget, u32 texture, u32 level);
        void (*glDeleteFramebuffers)(i32 count, const u32 *framebuffers);
        u32 (*glCheckFramebufferStatus)(u32 target);
        
        void (*glClear)(i32 mask);
        void (*glDrawArrays)(u32 mode, u32 first, i32 count);
        void (*glClearColor)(f32 r, f32 g, f32 b, f32 a);
        void (*glDrawElements)(u32 mode, i32 count, u32 type, const void *indices);
        void (*glDrawElementsBaseVertex)(u32 mode, i32 count, u32 type, const void *indices, u32 baseVertex);
        
        // STATE MANAGEMENT FUNCTIONS        
        void (*glEnable)(u32 cap);
        void (*glDisable)(u32 cap);
        void (*glBlendFunc)(u32 sfactor, u32 dfactor);
        void (*glCullFace)(u32 mode);
        void (*glDepthFunc)(u32 func);
        void (*glViewport)(u32 x, u32 y, i32 width, i32 height);
        void (*glPolygonMode)(u32 face, u32 mode);
        
        // UTILITY FUNCTIONS        
        u32 (*glGetError)(void);
        const u8 *(*glGetString)(const u32 name);
    } GL;
} GLAPI;
extern GLAPI* gl;


// GENERAL GRAPHICS API
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
    
    program->program = gl->GL.glCreateProgram();
    i32 v_Shader = gl->GL.glCreateShader(GLAPI_VERTEX_SHADER);
    i32 f_Shader = gl->GL.glCreateShader(GLAPI_FRAGMENT_SHADER);
    
    gl->GL.glShaderSource(v_Shader, 1, &(cstr){program->vertex}, NULL);
    gl->GL.glCompileShader(v_Shader);
    gl->GL.glGetShaderiv(v_Shader, GLAPI_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile vertex-shader!
        saneData->hashArray.destroy(&program->uniforms);
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to compile vertex shader");
        return;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Compiled vertex shader");

    gl->GL.glShaderSource(f_Shader, 1, &(cstr){program->fragment}, NULL);
    gl->GL.glCompileShader(f_Shader);
    gl->GL.glGetShaderiv(f_Shader, GLAPI_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile fragment-shader!
        saneData->hashArray.destroy(&program->uniforms);
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to compile fragment shader");
        return;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Compiled fragment shader");
    
    gl->GL.glAttachShader(program->program, v_Shader);
    gl->GL.glAttachShader(program->program, f_Shader);
    gl->GL.glLinkProgram(program->program);
    gl->GL.glGetProgramiv(program->program, GLAPI_LINK_STATUS, &link);
    if (!link) {
        // error: failed to link shader program!
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to link shader");
        saneData->hashArray.destroy(&program->uniforms);
        return;
    } else saneLog->log(SANE_LOG_SUCCESS, "[GLAPI] Linked shader");
    
    gl->GL.glDeleteShader(v_Shader);
    gl->GL.glDeleteShader(f_Shader);
}

none _destroyProgramImpl(GPUProgram* program) {
    gl->GL.glDeleteProgram(program->program);
}

none _setUniformImpl(GPUProgram* program, GPUUniform uniform) {
    if (!program || !program->program) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to set uniform (invalid program)");
        return;  // error: null ptr!
    }

    if (!uniform.name || !uniform.value || uniform.type < 0 || uniform.type >= GPU_UNIFORM_TYPES) {
        saneLog->log(SANE_LOG_ERROR, "[GLAPI] Failed to set uniform (invalid uniform)");
        return;  // error: uniform value error!
    }

    if (!saneData->hashArray.put(uniform.name, &(GPUUniform){
        .type = uniform.type,
        .name = uniform.name,
        .value = uniform.value,
        .location = gl->GL.glGetUniformLocation(program->program, uniform.name)
    }, &program->uniforms)) {
        saneLog->logFmt(SANE_LOG_ERROR, "[GLAPI] Failed to set uniform: %s", uniform.name);
    } else saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Set uniform: %s", uniform.name);
}

none _sendUniformImpl(GPUProgram* program, cstr name) {
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
        case GPU_UNIFORM_FLOAT: gl->GL.glUniform1f(uniform->location, *(f32*)uniform->value); break;
        case GPU_UNIFORM_VEC2: gl->GL.glUniform2fv(uniform->location, 1, (f32*)uniform->value); break;
        case GPU_UNIFORM_VEC3: gl->GL.glUniform3fv(uniform->location, 1, (f32*)uniform->value); break;
        case GPU_UNIFORM_VEC4: gl->GL.glUniform4fv(uniform->location, 1, (f32*)uniform->value); break;
        case GPU_UNIFORM_MAT4: gl->GL.glUniformMatrix4fv(uniform->location, 1, 0, (f32*)uniform->value); break;
        default: break;
    }

    saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Sent uniform: %s", name);
    return;
}


// none _createVertexBufferImpl(u8 format, u32 count, f32 *vertices, GPUHandle* vertexBuffer) {
//     if (!_graphics_api || !gl->GL.glInit) return (GLAPI_Vertex_Data){0}; // error: null ptr!

//     if ((attrs & ~((1 << GPU_VERTEX_ATTRIBS) - 1)) != 0 || !vertices) {
//         return (GLAPI_Vertex_Data){0};
//     }

//     GLAPI_Vertex_Data vertexData;

//     // calculate stride and offsets dynamically
//     u32 stride = 0;
//     u32 offsets[GLAPI_VERTEX_ATTRIBS] = {0};
//     const u32 attr_sizes[GLAPI_VERTEX_ATTRIBS] = {
//         3, // GLAPI_LOCATION_ATTR:  vec3
//         3, // GLAPI_COLOR_ATTR:     vec3
//         2, // GLAPI_TCOORD_ATTR:    vec2
//         3  // GLAPI_NORMAL_ATTR:    vec3
//     };

//     for (int i = 0; i < GLAPI_VERTEX_ATTRIBS; i++) {
//         if ((attrs & (1 << i)) != 0) {
//             // accumulate stride for enabled vertex attributes
//             offsets[i] = stride;
//             stride += attr_sizes[i];
//         }
//     }

//     gl->GL.glGenVertexArrays(1, &vertexData.vao);
//     gl->GL.glGenBuffers(1, &vertexData.vbo);

//     gl->GL.glBindVertexArray(vertexData.vao);
//     gl->GL.glBindBuffer(GLAPI_ARRAY_BUFFER, vertexData.vbo);
    
//     size_t vertex_data_size = vertexCount * (stride * sizeof(f32));
//     gl->GL.glBufferData(GLAPI_ARRAY_BUFFER, vertex_data_size, vertices, GLAPI_STATIC_DRAW);

//     // generate EBO if indices are provided
//     if (indexCount > 0 && indices) {
//         gl->GL.glGenBuffers(1, &vertexData.ebo);

//         gl->GL.glBindBuffer(GLAPI_ELEMENT_ARRAY_BUFFER, vertexData.ebo);
        
//         size_t index_data_size = indexCount * sizeof(u32);
//         gl->GL.glBufferData(GLAPI_ELEMENT_ARRAY_BUFFER, index_data_size, indices, GLAPI_STATIC_DRAW);
        
//         vertexData.indices = indices;
//         vertexData.indexCount = indexCount;
//     } else {
//         vertexData.ebo = 0;
//         vertexData.indices = NULL;
//         vertexData.indexCount = 0;
//     }

//     // configure vertex attributes
//     for (int i = 0; i < GLAPI_VERTEX_ATTRIBS/2; i++) {
//         if ((attrs & (1 << i)) != 0) {
//             gl->GL.glVertexAttribPointer(
//                 i, 
//                 attr_sizes[i], 
//                 GL_FLOAT, 
//                 GL_FALSE, 
//                 stride * sizeof(f32), 
//                 (void*)(offsets[i] * sizeof(f32))
//             );
//             gl->GL.glEnableVertexAttribArray(i);
//         }
//     }

//     gl->GL.glBindBuffer(GLAPI_ARRAY_BUFFER, 0);
//     gl->GL.glBindVertexArray(0);

//     vertexData.attrs = attrs;
//     vertexData.vertices = vertices;
//     vertexData.vertexCount = vertexCount;

//     return vertexData;
// }

// none _destroyVertexBufferImpl(GPUHandle* vertexBuffer) {
//     if (!_graphics_api || !gl->GL.glInit) return;  // error: null ptr!

//     vertexData->attrs = 0;
    
//     gl->GL.glDeleteBuffers(1, &vertexData->vbo);
//     vertexData->vbo = 0;
    
//     gl->GL.glDeleteBuffers(1, &vertexData->ebo);
//     vertexData->ebo = 0;
    
//     gl->GL.glDeleteVertexArrays(1, &vertexData->vao);
//     vertexData->vao = 0;
    
//     vertexData->vertices = NULL;
//     vertexData->vertexCount = 0;
//     vertexData->indices = NULL;
//     vertexData->indexCount = 0;
// }


// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/include/stb_image.h>

// none _createTexture2DImpl(cstr path, GPUTextureFormat format, GPUHandle* texture) {
// GLAPI_Texture texture = { .id = 0, .width = 0, .height = 0, .channels = 0, .path = NULL, .raw = NULL };
//     if (!path) return texture;

//     texture.path = path;
//     stbi_set_flip_vertically_on_load(SSDK_TRUE);
//     texture.raw = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
//     if (!texture.raw) return texture;    // error: failed to allocate raw data buffer!

//     gl->GL.glGenTextures(1, &texture.id);
//     gl->GL.glBindTexture(GL_TEXTURE_2D, texture.id);

//     // set texture wrapping options
//     gl->GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x axis
//     gl->GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y axis

//     // set texture filtering options (scaling up/down)
//     gl->GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//     gl->GL.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//     // generate the texture
//     gl->GL.glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.raw);
//     gl->GL.glGenerateMipmap(GL_TEXTURE_2D);

//     return texture;
// }

// none _destroyTexture2DImpl(GPUHandle* texture) {
//     texture->width = 0;
//     texture->height = 0;
//     texture->channels = 0;
//     texture ->path = NULL;
//     gl->GL.glDeleteTextures(1, &texture->id);
//     stbi_image_free(texture->raw);
//     texture->id = 0;
// }


byte _initGLAPI(none) {
    if (!gl) return SSDK_FALSE;   // error: null ptr!
    
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
        {(void**)&gl->GL.glGenBuffers, "glGenBuffers"},
        {(void**)&gl->GL.glBindBuffer, "glBindBuffer"},
        {(void**)&gl->GL.glBufferData, "glBufferData"},
        {(void**)&gl->GL.glMapBuffer, "glMapBuffer"},
        {(void**)&gl->GL.glUnmapBuffer, "glUnmapBuffer"},
        {(void**)&gl->GL.glBufferSubData, "glBufferSubData"},
        {(void**)&gl->GL.glGetBufferParameteriv, "glGetBufferParameteriv"},
        {(void**)&gl->GL.glDeleteBuffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&gl->GL.glEnableVertexAttribArray, "glEnableVertexAttribArray"},
        {(void**)&gl->GL.glDisableVertexAttribArray, "glDisableVertexAttribArray"},
        {(void**)&gl->GL.glVertexAttribPointer, "glVertexAttribPointer"},
        {(void**)&gl->GL.glBindVertexArray, "glBindVertexArray"},
        {(void**)&gl->GL.glGenVertexArrays, "glGenVertexArrays"},
        {(void**)&gl->GL.glDeleteVertexArrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&gl->GL.glCreateProgram, "glCreateProgram"},
        {(void**)&gl->GL.glUseProgram, "glUseProgram"},
        {(void**)&gl->GL.glLinkProgram, "glLinkProgram"},
        {(void**)&gl->GL.glDeleteProgram, "glDeleteProgram"},
        {(void**)&gl->GL.glCreateShader, "glCreateShader"},
        {(void**)&gl->GL.glCompileShader, "glCompileShader"},
        {(void**)&gl->GL.glAttachShader, "glAttachShader"},
        {(void**)&gl->GL.glDetachShader, "glDetachShader"},
        {(void**)&gl->GL.glDeleteShader, "glDeleteShader"},
        {(void**)&gl->GL.glShaderSource, "glShaderSource"},
        {(void**)&gl->GL.glGetShaderiv, "glGetShaderiv"},
        {(void**)&gl->GL.glGetShaderInfoLog, "glGetShaderInfoLog"},
        {(void**)&gl->GL.glGetProgramiv, "glGetProgramiv"},
        {(void**)&gl->GL.glGetProgramInfoLog, "glGetProgramInfoLog"},
        {(void**)&gl->GL.glGetUniformLocation, "glGetUniformLocation"},
        
        {(void**)&gl->GL.glUniform1f, "glUniform1f"},
        {(void**)&gl->GL.glUniform2fv, "glUniform2fv"},
        {(void**)&gl->GL.glUniform3fv, "glUniform3fv"},
        {(void**)&gl->GL.glUniform4fv, "glUniform4fv"},
        {(void**)&gl->GL.glUniformMatrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&gl->GL.glGenTextures, "glGenTextures"},
        {(void**)&gl->GL.glBindTexture, "glBindTexture"},
        {(void**)&gl->GL.glTexParameteri, "glTexParameteri"},
        {(void**)&gl->GL.glTexImage2D, "glTexImage2D"},
        {(void**)&gl->GL.glActiveTexture, "glActiveTexture"},
        {(void**)&gl->GL.glDeleteTextures, "glDeleteTextures"},
        {(void**)&gl->GL.glGenerateMipmap, "glGenerateMipmap"},
        {(void**)&gl->GL.glGenerateTextureMipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&gl->GL.glGenFramebuffers, "glGenFramebuffers"},
        {(void**)&gl->GL.glBindFramebuffer, "glBindFramebuffer"},
        {(void**)&gl->GL.glFramebufferTexture2d, "glFramebufferTexture2D"},
        {(void**)&gl->GL.glDeleteFramebuffers, "glDeleteFramebuffers"},
        {(void**)&gl->GL.glCheckFramebufferStatus, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&gl->GL.glClear, "glClear"},
        {(void**)&gl->GL.glClearColor, "glClearColor"},
        {(void**)&gl->GL.glDrawArrays, "glDrawArrays"},
        {(void**)&gl->GL.glDrawElements, "glDrawElements"},
        {(void**)&gl->GL.glDrawElementsBaseVertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&gl->GL.glEnable, "glEnable"},
        {(void**)&gl->GL.glDisable, "glDisable"},
        {(void**)&gl->GL.glBlendFunc, "glBlendFunc"},
        {(void**)&gl->GL.glCullFace, "glCullFace"},
        {(void**)&gl->GL.glDepthFunc, "glDepthFunc"},
        {(void**)&gl->GL.glViewport, "glViewport"},
        {(void**)&gl->GL.glPolygonMode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&gl->GL.glGetError, "glGetError"},
        {(void**)&gl->GL.glGetString, "glGetString"}
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = swarmPlatform->loadSymbol(functions[i].name, &lib);
        if (*functions[i].function == NULL) {
            saneLog->logFmt(SANE_LOG_ERROR, "[GLAPI] Failed to load: %s", functions[i].name);
        } else saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Loaded: %s", functions[i].name);
    }
    
    swarmPlatform->unloadLib(&lib);

    gl->createProgram = _createProgramImpl;
    gl->destroyProgram = _destroyProgramImpl;
    
    gl->setUniform = _setUniformImpl;
    gl->sendUniform = _sendUniformImpl;
    
    gl->createVertexBuffer = NULL; // _createVertexBufferImpl;
    gl->destroyVertexBuffer = NULL; // _destroyVertexBufferImpl;
    
    gl->createElementBuffer = NULL; // _createElementBufferImpl;
    gl->destroyElementBuffer = NULL; // _destroyElementBufferImpl;
    
    gl->createTexture2D = NULL; // _createTexture2DImpl;
    gl->destroyTexture2D = NULL; // _destroyTexture2DImpl;

    return SSDK_TRUE;
}

#endif  // __SWARM_GLAPI_H__