#include "ltgl.h"
#include "../../platform/ltlogger.h"

// BUFFER FUNCTIONS

void (*ltglGenBuffers)(LTsizei count, LTuint *buffer);
void (*ltglBindBuffer)(LTenum target, LTuint buffer);
void (*ltglBufferData)(LTenum target, LTsizeiptr size, const LTvoid *data, LTenum usage);
void (*ltglDeleteBuffers)(LTsizei count, const LTuint *buffer);
void (*ltglMapBuffer)(LTenum target, LTenum access);
void (*ltglUnmapBuffer)(LTenum target);
void (*ltglBufferSubData)(LTenum target, LTintptr offset, LTsizeiptr size, const LTvoid *data);
void (*ltglGetBufferParameteriv)(LTenum target, LTenum pname, LTint *params);

// VERTEX ATTRIBUTE FUNCTIONS

void (*ltglEnableVertexAttribArray)(LTuint index);
void (*ltglDisableVertexAttribArray)(LTuint index);
void (*ltglVertexAttribPointer)(LTuint index, LTint size, LTenum type, LTboolean normalized, LTsizei stride, const LTvoid *pointer);
void (*ltglBindVertexArray)(LTuint array);
void (*ltglGenVertexArrays)(LTsizei count, LTuint *arrays);
void (*ltglDeleteVertexArrays)(LTsizei count, const LTuint *arrays);

// SHADER FUNCTIONS

LTuint (*ltglCreateProgram)(void);
void (*ltglUseProgram)(LTuint program);
void (*ltglLinkProgram)(LTuint program);
void (*ltglDeleteProgram)(LTuint program);

LTuint (*ltglCreateShader)(LTuint type);
void (*ltglCompileShader)(LTuint shader);
void (*ltglAttachShader)(LTuint program, LTuint shader);
void (*ltglDetachShader)(LTuint program, LTuint shader);
void (*ltglDeleteShader)(LTuint shader);
void (*ltglShaderSource)(LTuint shader, LTsizei count, const char **strings, const int *lengths);
void (*ltglGetShaderiv)(LTuint shader, LTenum pname, LTint *params);
void (*ltglGetShaderInfoLog)(LTuint shader, LTsizei maxLength, LTsizei *length, char *infoLog);
void (*ltglGetProgramiv)(LTuint program, LTenum pname, LTint *params);
void (*ltglGetProgramInfoLog)(LTuint program, LTsizei maxLength, LTsizei *length, char *infoLog);
LTuint (*ltglGetUniformLocation)(LTuint program, LTstring name);
void (*ltglUniform2fv)(LTuint location, LTuint count, const LTfloatptr value);
void (*ltglUniform3fv)(LTuint location, LTuint count, const LTfloatptr value);
void (*ltglUniform4fv)(LTuint location, LTuint count, const LTfloatptr value);
void (*ltglUniformMatrix4fv)(LTuint location, LTuint count, LTuint transpose, const LTfloatptr value);

// TEXTURE FUNCTIONS

void (*ltglGenTextures)(LTsizei count, LTuint *textures);
void (*ltglBindTexture)(LTenum target, LTuint texture);
void (*ltglTexParameteri)(LTenum target, LTenum pname, LTint param);
void (*ltglTexImage2D)(LTenum target, LTint level, LTint internalFormat, LTsizei width, LTsizei height, LTint border, LTenum format, LTenum type, const LTvoid *data);
void (*ltglActiveTexture)(LTenum texture);
void (*ltglDeleteTextures)(LTsizei count, const LTuint *textures);
void (*ltglGenerateMipmap)(LTenum target);
void (*ltglGenerateTextureMipmap)(LTuint texture);

// FRAMEBUFFER FUNCTIONS

void (*ltglGenFramebuffers)(LTsizei count, LTuint *framebuffers);
void (*ltglBindFramebuffer)(LTenum target, LTuint framebuffer);
void (*ltglFramebufferTexture2D)(LTenum target, LTenum attachment, LTenum textarget, LTuint texture, LTint level);
void (*ltglDeleteFramebuffers)(LTsizei count, const LTuint *framebuffers);
LTenum (*ltglCheckFramebufferStatus)(LTenum target);

// DRAWING FUNCTIONS

void (*ltglClear)(LTbitfield mask);
void (*ltglDrawArrays)(LTenum mode, LTint first, LTsizei count);
void (*ltglClearColor)(LTfloat r, LTfloat g, LTfloat b, LTfloat a);
void (*ltglDrawElements)(LTenum mode, LTsizei count, LTenum type, const LTvoid *indices);
void (*ltglDrawElementsBaseVertex)(LTenum mode, LTsizei count, LTenum type, const LTvoid *indices, LTint baseVertex);

// STATE MANAGEMENT FUNCTIONS

void (*ltglEnable)(LTenum cap);
void (*ltglDisable)(LTenum cap);
void (*ltglBlendFunc)(LTenum sfactor, LTenum dfactor);
void (*ltglCullFace)(LTenum mode);
void (*ltglDepthFunc)(LTenum func);
void (*ltglViewport)(LTint x, LTint y, LTsizei width, LTsizei height);
void (*ltglPolygonMode)(LTenum face, LTenum mode);

// UTILITY FUNCTIONS

LTenum (*ltglGetError)(void);
const LTubyte *(*ltglGetString)(const LTenum name);

void *ltglGetFunction(const char *name) {
    void* proc = (void*)wglGetProcAddress(name);
    if (
        proc == NULL        ||
        (proc ==(void*)0x1) || 
        (proc ==(void*)0x2) || 
        (proc ==(void*)0x3) || 
        (proc ==(void*)-1)) {
        proc = (void*)GetProcAddress(LoadLibrary("opengl32.dll"), name);
    }; return proc;
}

bool ltglLoadFunctions(void) {
    bool result = true;

    struct {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&ltglGenBuffers, "glGenBuffers"},
        {(void**)&ltglBindBuffer, "glBindBuffer"},
        {(void**)&ltglBufferData, "glBufferData"},
        {(void**)&ltglMapBuffer, "glMapBuffer"},
        {(void**)&ltglUnmapBuffer, "glUnmapBuffer"},
        {(void**)&ltglBufferSubData, "glBufferSubData"},
        {(void**)&ltglGetBufferParameteriv, "glGetBufferParameteriv"},
        {(void**)&ltglDeleteBuffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&ltglEnableVertexAttribArray, "glEnableVertexAttribArray"},
        {(void**)&ltglDisableVertexAttribArray, "glDisableVertexAttribArray"},
        {(void**)&ltglVertexAttribPointer, "glVertexAttribPointer"},
        {(void**)&ltglBindVertexArray, "glBindVertexArray"},
        {(void**)&ltglGenVertexArrays, "glGenVertexArrays"},
        {(void**)&ltglDeleteVertexArrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&ltglCreateProgram, "glCreateProgram"},
        {(void**)&ltglUseProgram, "glUseProgram"},
        {(void**)&ltglLinkProgram, "glLinkProgram"},
        {(void**)&ltglDeleteProgram, "glDeleteProgram"},
        {(void**)&ltglCreateShader, "glCreateShader"},
        {(void**)&ltglCompileShader, "glCompileShader"},
        {(void**)&ltglAttachShader, "glAttachShader"},
        {(void**)&ltglDetachShader, "glDetachShader"},
        {(void**)&ltglDeleteShader, "glDeleteShader"},
        {(void**)&ltglShaderSource, "glShaderSource"},
        {(void**)&ltglGetShaderiv, "glGetShaderiv"},
        {(void**)&ltglGetShaderInfoLog, "glGetShaderInfoLog"},
        {(void**)&ltglGetProgramiv, "glGetProgramiv"},
        {(void**)&ltglGetProgramInfoLog, "glGetProgramInfoLog"},
        {(void**)&ltglGetUniformLocation, "glGetUniformLocation"},
        {(void**)&ltglUniform2fv, "glUniform2fv"},
        {(void**)&ltglUniform3fv, "glUniform3fv"},
        {(void**)&ltglUniform4fv, "glUniform4fv"},
        {(void**)&ltglUniformMatrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&ltglGenTextures, "glGenTextures"},
        {(void**)&ltglBindTexture, "glBindTexture"},
        {(void**)&ltglTexParameteri, "glTexParameteri"},
        {(void**)&ltglTexImage2D, "glTexImage2D"},
        {(void**)&ltglActiveTexture, "glActiveTexture"},
        {(void**)&ltglDeleteTextures, "glDeleteTextures"},
        {(void**)&ltglGenerateMipmap, "glGenerateMipmap"},
        {(void**)&ltglGenerateTextureMipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&ltglGenFramebuffers, "glGenFramebuffers"},
        {(void**)&ltglBindFramebuffer, "glBindFramebuffer"},
        {(void**)&ltglFramebufferTexture2D, "glFramebufferTexture2D"},
        {(void**)&ltglDeleteFramebuffers, "glDeleteFramebuffers"},
        {(void**)&ltglCheckFramebufferStatus, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&ltglClear, "glClear"},
        {(void**)&ltglClearColor, "glClearColor"},
        {(void**)&ltglDrawArrays, "glDrawArrays"},
        {(void**)&ltglDrawElements, "glDrawElements"},
        {(void**)&ltglDrawElementsBaseVertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&ltglEnable, "glEnable"},
        {(void**)&ltglDisable, "glDisable"},
        {(void**)&ltglBlendFunc, "glBlendFunc"},
        {(void**)&ltglCullFace, "glCullFace"},
        {(void**)&ltglDepthFunc, "glDepthFunc"},
        {(void**)&ltglViewport, "glViewport"},
        {(void**)&ltglPolygonMode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&ltglGetError, "glGetError"},
        {(void**)&ltglGetString, "glGetString"}
    };

    // Load all functions
    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = ltglGetFunction(functions[i].name);
        if (!*functions[i].function) {
            ltSetLogLevel(LOTUS_LOG_ERROR);
            ltLogError("failed to load GL function: %s\n", functions[i].name);
            result = false;
        }
    }

    return result;
}
