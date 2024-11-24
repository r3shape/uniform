#ifndef LTGL_H
#define LTGL_H

#include <gl/GL.h>
#include <gl/GLU.h>

#include <stddef.h>
#include <stdbool.h>

#include "../lotustypes.h"

// GL equivalent defines
#define LTGL_STATIC_DRAW                0x88E4

#define LTGL_ARRAY_BUFFER               0x8892
#define LTGL_ELEMENT_ARRAY_BUFFER       0x8893

#define LTGL_VERTEX_SHADER              0x8B31
#define LTGL_FRAGMENT_SHADER            0x8B30

#define LTGL_LINK_STATUS                0x8B82
#define LTGL_COMPILE_STATUS             0x8B81
#define LTGL_ACTIVE_ATTRIBUTES          0x8B89

#define LTGL_LINE                       0x1B01
#define LTGL_FRONT_AND_BACK             0x0408

#define LTGL_SHADING_LANGUAGE_VERSION   0x8B8C

#define LTGL_TEXTURE0                   0x84C0
#define LTGL_TEXTURE1                   0x84C1
#define LTGL_TEXTURE2                   0x84C2
#define LTGL_TEXTURE3                   0x84C3
#define LTGL_ACTIVE_TEXTURE             0x84E0
#define LTGL_MAX_TEXTURE_UNITS          0x84E2

bool ltglLoadFunctions(void);
void *ltglGetFunction(const char *name);

// BUFFER FUNCTIONS

extern void (*ltglGenBuffers)(LTsizei count, LTuint *buffer);
extern void (*ltglBindBuffer)(LTenum target, LTuint buffer);
extern void (*ltglBufferData)(LTenum target, LTsizeiptr size, const LTvoid *data, LTenum usage);
extern void (*ltglDeleteBuffers)(LTsizei count, const LTuint *buffer);
extern void (*ltglMapBuffer)(LTenum target, LTenum access);
extern void (*ltglUnmapBuffer)(LTenum target);
extern void (*ltglBufferSubData)(LTenum target, LTintptr offset, LTsizeiptr size, const LTvoid *data);
extern void (*ltglGetBufferParameteriv)(LTenum target, LTenum pname, LTint *params);

// VERTEX ATTRIBUTE FUNCTIONS

extern void (*ltglEnableVertexAttribArray)(LTuint index);
extern void (*ltglDisableVertexAttribArray)(LTuint index);
extern void (*ltglVertexAttribPointer)(LTuint index, LTint size, LTenum type, LTboolean normalized, LTsizei stride, const LTvoid *pointer);
extern void (*ltglBindVertexArray)(LTuint array);
extern void (*ltglGenVertexArrays)(LTsizei count, LTuint *arrays);
extern void (*ltglDeleteVertexArrays)(LTsizei count, const LTuint *arrays);

// SHADER FUNCTIONS

extern LTuint (*ltglCreateProgram)(void);
extern void (*ltglUseProgram)(LTuint program);
extern void (*ltglLinkProgram)(LTuint program);
extern void (*ltglDeleteProgram)(LTuint program);

extern LTuint (*ltglCreateShader)(LTuint type);
extern void (*ltglCompileShader)(LTuint shader);
extern void (*ltglAttachShader)(LTuint program, LTuint shader);
extern void (*ltglDetachShader)(LTuint program, LTuint shader);
extern void (*ltglDeleteShader)(LTuint shader);
extern void (*ltglShaderSource)(LTuint shader, LTsizei count, const char **strings, const int *lengths);
extern void (*ltglGetShaderiv)(LTuint shader, LTenum pname, LTint *params);
extern void (*ltglGetShaderInfoLog)(LTuint shader, LTsizei maxLength, LTsizei *length, char *infoLog);
extern void (*ltglGetProgramiv)(LTuint program, LTenum pname, LTint *params);
extern void (*ltglGetProgramInfoLog)(LTuint program, LTsizei maxLength, LTsizei *length, char *infoLog);
extern LTuint (*ltglGetUniformLocation)(LTuint program, LTstring name);
extern void (*ltglUniform2fv)(LTuint location, LTuint count, const LTfloatptr value);
extern void (*ltglUniform3fv)(LTuint location, LTuint count, const LTfloatptr value);
extern void (*ltglUniform4fv)(LTuint location, LTuint count, const LTfloatptr value);
extern void (*ltglUniformMatrix4fv)(LTuint location, LTuint count, LTuint transpose, const LTfloatptr value);

// TEXTURE FUNCTIONS

extern void (*ltglGenTextures)(LTsizei count, LTuint *textures);
extern void (*ltglBindTexture)(LTenum target, LTuint texture);
extern void (*ltglTexParameteri)(LTenum target, LTenum pname, LTint param);
extern void (*ltglTexImage2D)(LTenum target, LTint level, LTint internalFormat, LTsizei width, LTsizei height, LTint border, LTenum format, LTenum type, const LTvoid *data);
extern void (*ltglActiveTexture)(LTenum texture);
extern void (*ltglDeleteTextures)(LTsizei count, const LTuint *textures);
extern void (*ltglGenerateMipmap)(LTenum target);
extern void (*ltglGenerateTextureMipmap)(LTuint texture);

// FRAMEBUFFER FUNCTIONS

extern void (*ltglGenFramebuffers)(LTsizei count, LTuint *framebuffers);
extern void (*ltglBindFramebuffer)(LTenum target, LTuint framebuffer);
extern void (*ltglFramebufferTexture2D)(LTenum target, LTenum attachment, LTenum textarget, LTuint texture, LTint level);
extern void (*ltglDeleteFramebuffers)(LTsizei count, const LTuint *framebuffers);
extern LTenum (*ltglCheckFramebufferStatus)(LTenum target);

// DRAWING FUNCTIONS

extern void (*ltglClear)(LTbitfield mask);
extern void (*ltglDrawArrays)(LTenum mode, LTint first, LTsizei count);
extern void (*ltglClearColor)(LTfloat r, LTfloat g, LTfloat b, LTfloat a);
extern void (*ltglDrawElements)(LTenum mode, LTsizei count, LTenum type, const LTvoid *indices);
extern void (*ltglDrawElementsBaseVertex)(LTenum mode, LTsizei count, LTenum type, const LTvoid *indices, LTint baseVertex);

// STATE MANAGEMENT FUNCTIONS

extern void (*ltglEnable)(LTenum cap);
extern void (*ltglDisable)(LTenum cap);
extern void (*ltglBlendFunc)(LTenum sfactor, LTenum dfactor);
extern void (*ltglCullFace)(LTenum mode);
extern void (*ltglDepthFunc)(LTenum func);
extern void (*ltglViewport)(LTint x, LTint y, LTsizei width, LTsizei height);
extern void (*ltglPolygonMode)(LTenum face, LTenum mode);

// UTILITY FUNCTIONS

extern LTenum (*ltglGetError)(void);
extern const LTubyte *(*ltglGetString)(const LTenum name);

#endif