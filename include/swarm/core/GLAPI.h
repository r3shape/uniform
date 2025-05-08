#ifndef __SWARM_GLAPI_H__
#define __SWARM_GLAPI_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/swarm_renderer.h>

typedef struct GLAPI {
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
} GLAPI;
extern GLAPI* gl;

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
        {(void**)&gl->glGenBuffers, "glGenBuffers"},
        {(void**)&gl->glBindBuffer, "glBindBuffer"},
        {(void**)&gl->glBufferData, "glBufferData"},
        {(void**)&gl->glMapBuffer, "glMapBuffer"},
        {(void**)&gl->glUnmapBuffer, "glUnmapBuffer"},
        {(void**)&gl->glBufferSubData, "glBufferSubData"},
        {(void**)&gl->glGetBufferParameteriv, "glGetBufferParameteriv"},
        {(void**)&gl->glDeleteBuffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&gl->glEnableVertexAttribArray, "glEnableVertexAttribArray"},
        {(void**)&gl->glDisableVertexAttribArray, "glDisableVertexAttribArray"},
        {(void**)&gl->glVertexAttribPointer, "glVertexAttribPointer"},
        {(void**)&gl->glBindVertexArray, "glBindVertexArray"},
        {(void**)&gl->glGenVertexArrays, "glGenVertexArrays"},
        {(void**)&gl->glDeleteVertexArrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&gl->glCreateProgram, "glCreateProgram"},
        {(void**)&gl->glUseProgram, "glUseProgram"},
        {(void**)&gl->glLinkProgram, "glLinkProgram"},
        {(void**)&gl->glDeleteProgram, "glDeleteProgram"},
        {(void**)&gl->glCreateShader, "glCreateShader"},
        {(void**)&gl->glCompileShader, "glCompileShader"},
        {(void**)&gl->glAttachShader, "glAttachShader"},
        {(void**)&gl->glDetachShader, "glDetachShader"},
        {(void**)&gl->glDeleteShader, "glDeleteShader"},
        {(void**)&gl->glShaderSource, "glShaderSource"},
        {(void**)&gl->glGetShaderiv, "glGetShaderiv"},
        {(void**)&gl->glGetShaderInfoLog, "glGetShaderInfoLog"},
        {(void**)&gl->glGetProgramiv, "glGetProgramiv"},
        {(void**)&gl->glGetProgramInfoLog, "glGetProgramInfoLog"},
        {(void**)&gl->glGetUniformLocation, "glGetUniformLocation"},
        
        {(void**)&gl->glUniform1f, "glUniform1f"},
        {(void**)&gl->glUniform2fv, "glUniform2fv"},
        {(void**)&gl->glUniform3fv, "glUniform3fv"},
        {(void**)&gl->glUniform4fv, "glUniform4fv"},
        {(void**)&gl->glUniformMatrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&gl->glGenTextures, "glGenTextures"},
        {(void**)&gl->glBindTexture, "glBindTexture"},
        {(void**)&gl->glTexParameteri, "glTexParameteri"},
        {(void**)&gl->glTexImage2D, "glTexImage2D"},
        {(void**)&gl->glActiveTexture, "glActiveTexture"},
        {(void**)&gl->glDeleteTextures, "glDeleteTextures"},
        {(void**)&gl->glGenerateMipmap, "glGenerateMipmap"},
        {(void**)&gl->glGenerateTextureMipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&gl->glGenFramebuffers, "glGenFramebuffers"},
        {(void**)&gl->glBindFramebuffer, "glBindFramebuffer"},
        {(void**)&gl->glFramebufferTexture2d, "glFramebufferTexture2D"},
        {(void**)&gl->glDeleteFramebuffers, "glDeleteFramebuffers"},
        {(void**)&gl->glCheckFramebufferStatus, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&gl->glClear, "glClear"},
        {(void**)&gl->glClearColor, "glClearColor"},
        {(void**)&gl->glDrawArrays, "glDrawArrays"},
        {(void**)&gl->glDrawElements, "glDrawElements"},
        {(void**)&gl->glDrawElementsBaseVertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&gl->glEnable, "glEnable"},
        {(void**)&gl->glDisable, "glDisable"},
        {(void**)&gl->glBlendFunc, "glBlendFunc"},
        {(void**)&gl->glCullFace, "glCullFace"},
        {(void**)&gl->glDepthFunc, "glDepthFunc"},
        {(void**)&gl->glViewport, "glViewport"},
        {(void**)&gl->glPolygonMode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&gl->glGetError, "glGetError"},
        {(void**)&gl->glGetString, "glGetString"}
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = swarmPlatform->loadSymbol(functions[i].name, &lib);
        if (*functions[i].function == NULL) {
            saneLog->logFmt(SANE_LOG_ERROR, "[GLAPI] Failed to load: %s", functions[i].name);
        } else saneLog->logFmt(SANE_LOG_SUCCESS, "[GLAPI] Loaded: %s", functions[i].name);
    }
    
    swarmPlatform->unloadLib(&lib);
    return SSDK_TRUE;
}

#endif  // __SWARM_GLAPI_H__