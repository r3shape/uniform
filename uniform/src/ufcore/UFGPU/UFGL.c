#include <ufcore/UFGPU.h>
#include <ufcore/UFOS.h>

#include <gl/GL.h>
#include <gl/GLU.h>

// GL equivalent defines
#define UFGL_STATIC_DRAW                0x88E4
#define UFGL_ARRAY_BUFFER               0x8892
#define UFGL_ELEMENT_BUFFER             0x8893

#define UFGL_VERTEX_SHADER              0x8B31
#define UFGL_FRAGMENT_SHADER            0x8B30

#define UFGL_LINK_STATUS                0x8B82
#define UFGL_COMPILE_STATUS             0x8B81
#define UFGL_ACTIVE_ATTRIBUTES          0x8B89
#define UFGL_SHADING_LANGUAGE_VERSION   0x8B8C

#define UFGL_TEXT_UNIT0                 0x84C0
#define UFGL_TEXT_UNIT1                 0x84C1
#define UFGL_TEXT_UNIT2                 0x84C2
#define UFGL_TEXT_UNIT3                 0x84C3
#define UFGL_ACTIVE_TEXTURE             0x84E0
#define UFGL_MAX_TEXTURE_UNITS          0x84E2

#define UFGL_POINTS                     0x0000
#define UFGL_LINES                      0x0001
#define UFGL_LINE_LOOP                  0x0002
#define UFGL_LINE_STRIP                 0x0003
#define UFGL_TRIANGLES                  0x0004
#define UFGL_TRIANGLE_STRIP             0x0005
#define UFGL_TRIANGLE_FAN               0x0006
#define UFGL_QUADS                      0x0007
#define UFGL_QUAD_STRIP                 0x0008
#define UFGL_POLYGON                    0x0009

#define UFGL_LINE                       0x1B01
#define UFGL_FRONT_AND_BACK             0x0408

#define UFGL_FRAMEBUFFER 0x8D40
#define UFGL_RENDERBUFFER 0x8D41

#define UFGL_DEPTH_ATTACHMENT 0x8D00
#define UFGL_DEPTH24_STENCIL8 0x88F0

#define UFGL_COLOR_ATTACHMENT0 0x8CE0
#define UFGL_COLOR_ATTACHMENT1 0x8CE1
#define UFGL_COLOR_ATTACHMENT2 0x8CE2
#define UFGL_COLOR_ATTACHMENT3 0x8CE3
#define UFGL_COLOR_ATTACHMENT4 0x8CE4
#define UFGL_COLOR_ATTACHMENT5 0x8CE5
#define UFGL_COLOR_ATTACHMENT6 0x8CE6
#define UFGL_COLOR_ATTACHMENT7 0x8CE7
#define UFGL_COLOR_ATTACHMENT8 0x8CE8
#define UFGL_COLOR_ATTACHMENT9 0x8CE9
#define UFGL_COLOR_ATTACHMENT10 0x8CEA
#define UFGL_COLOR_ATTACHMENT11 0x8CEB
#define UFGL_COLOR_ATTACHMENT12 0x8CEC
#define UFGL_COLOR_ATTACHMENT13 0x8CED
#define UFGL_COLOR_ATTACHMENT14 0x8CEE
#define UFGL_COLOR_ATTACHMENT15 0x8CEF

#define UFGL_STENCIL_ATTACHMENT 0x8D20
#define UFGL_DEPTH_STENCIL_ATTACHMENT 0x821A

static struct UFGL {
    Array buffers;
    Array programs;

    // BUFFER FUNCTIONS
    UF_API_METHOD(none, genBuffers, s32 n, u32* buffers);
    UF_API_METHOD(none, bindBuffer, u32 target, u32 buffer);
    UF_API_METHOD(none, bufferData, u32 target, s32 size, const ptr data, u32 usage);
    UF_API_METHOD(ptr,  mapBuffer, u32 target, u32 access);
    UF_API_METHOD(none, unmapBuffer, u32 target);
    UF_API_METHOD(none, bufferSubData, u32 target, s32 offset, s32 size, const ptr data);
    UF_API_METHOD(none, getBufferParameteriv, u32 target, u32 pname, s32* params);
    UF_API_METHOD(none, deleteBuffers, s32 n, const u32* buffers);

    // VAO FUNCTIONS
    UF_API_METHOD(none, genVertexArrays, s32 n, u32* arrays);
    UF_API_METHOD(none, bindVertexArray, u32 array);
    UF_API_METHOD(none, deleteVertexArrays, s32 n, const u32* arrays);
    UF_API_METHOD(none, enableVertexAttribArray, u32 index);
    UF_API_METHOD(none, disableVertexAttribArray, u32 index);
    UF_API_METHOD(none, vertexAttribPointer, u32 index, s32 size, u32 type, u32 normalized, s32 stride, const ptr pointer);

    // SHADER FUNCTIONS
    UF_API_METHOD(u32,  createShader, u32 type);
    UF_API_METHOD(none, shaderSource, u32 shader, s32 count, CString* string, const s32* length);
    UF_API_METHOD(none, compileShader, u32 shader);
    UF_API_METHOD(none, deleteShader, u32 shader);
    UF_API_METHOD(none, getShaderiv, u32 shader, u32 pname, s32* params);
    UF_API_METHOD(none, getShaderInfoLog, u32 shader, s32 maxLength, s32* length, CString infoLog);

    UF_API_METHOD(u32,  createProgram, void);
    UF_API_METHOD(none, attachShader, u32 program, u32 shader);
    UF_API_METHOD(none, detachShader, u32 program, u32 shader);
    UF_API_METHOD(none, linkProgram, u32 program);
    UF_API_METHOD(none, useProgram, u32 program);
    UF_API_METHOD(none, deleteProgram, u32 program);
    UF_API_METHOD(none, getProgramiv, u32 program, u32 pname, s32* params);
    UF_API_METHOD(none, getProgramInfoLog, u32 program, s32 maxLength, s32* length, CString infoLog);
    UF_API_METHOD(s32,  getUniformLocation, u32 program, CString name);

    // UNIFORMS
    UF_API_METHOD(none, uniform1i, s32 location, s32 v0);
    UF_API_METHOD(none, uniform1f, s32 location, f32 v0);
    UF_API_METHOD(none, uniform2fv, s32 location, s32 count, const f32* value);
    UF_API_METHOD(none, uniform3fv, s32 location, s32 count, const f32* value);
    UF_API_METHOD(none, uniform4fv, s32 location, s32 count, const f32* value);
    UF_API_METHOD(none, uniformMatrix4fv, s32 location, s32 count, u32 transpose, const f32* value);

    // TEXTURES
    UF_API_METHOD(none, genTextures, s32 n, u32* textures);
    UF_API_METHOD(none, bindTexture, u32 target, u32 texture);
    UF_API_METHOD(none, texParameteri, u32 target, u32 pname, s32 param);
    UF_API_METHOD(none, texImage2d, u32 target, s32 level, s32 internalFormat, s32 width, s32 height, s32 border, u32 format, u32 type, const ptr data);
    UF_API_METHOD(none, activeTexture, u32 texture);
    UF_API_METHOD(none, deleteTextures, s32 n, const u32* textures);
    UF_API_METHOD(none, generateMipmap, u32 target);

    // FRAMEBUFFERS + RENDERBUFFERS
    UF_API_METHOD(none, genFramebuffers, s32 n, u32* ids);
    UF_API_METHOD(u32,  checkFramebufferStatus, u32 target);
    UF_API_METHOD(none, bindFramebuffer, u32 target, u32 framebuffer);
    UF_API_METHOD(none, framebufferTexture2d, u32 target, u32 attachment, u32 textarget, u32 texture, s32 level);
    UF_API_METHOD(none, framebufferRenderbuffer, u32 target, u32 attachment, u32 renderbuffertarget, u32 renderbuffer);
    UF_API_METHOD(none, deleteFramebuffers, s32 n, const u32* ids);

    UF_API_METHOD(none, genRenderbuffers, s32 n, u32* renderbuffers);
    UF_API_METHOD(none, bindRenderbuffer, u32 target, u32 renderbuffer);
    UF_API_METHOD(none, renderbufferStorage, u32 target, u32 internalFormat, s32 width, s32 height);
    UF_API_METHOD(none, deleteRenderbuffers, s32 n, const u32* renderbuffers);

    // DRAWING
    UF_API_METHOD(none, drawArrays, u32 mode, s32 first, s32 count);
    UF_API_METHOD(none, drawElements, u32 mode, s32 count, u32 type, const ptr indices);

    // STATE
    UF_API_METHOD(none, enable, u32 cap);
    UF_API_METHOD(none, disable, u32 cap);
    UF_API_METHOD(none, blendFunc, u32 sfactor, u32 dfactor);
    UF_API_METHOD(none, cullFace, u32 mode);
    UF_API_METHOD(none, depthFunc, u32 func);
    UF_API_METHOD(none, viewport, s32 x, s32 y, s32 width, s32 height);
    UF_API_METHOD(none, polygonMode, u32 face, u32 mode);
    UF_API_METHOD(none, clear, u32 mask);
    UF_API_METHOD(none, clearColor, f32 r, f32 g, f32 b, f32 a);
    UF_API_METHOD(none, clearDepth, f64 depth);

    // INFO / DEBUG
    UF_API_METHOD(u32, getError, void);
    UF_API_METHOD(CString, getString, u32 name);
} UFGL = {NULL};


// TODO: program map array for safe deletes (every index > deleted -= 1)
// Programs
UFResource newProgram(UFGPUProgramDesc program) {
    u32 count = r3_arr_count(&UFGL.programs);
    switch (program.type) {
        case (UF_GPU_VERTEX_PROGRAM): {
            if (!program.vertex.vertexPath || !program.vertex.fragmentPath) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- incomplete program descriptor\n");
                return 0;
            }

            s32 link_status = 0;
            s32 compile_status = 0;
            
            // allocate program source buffers
            Buffer vertexBuffer = {0};
            Buffer fragmentBuffer = {0};
            if (!r3_buf_alloc(UF_GPU_PROGRAM_BUFFER_SIZE, &vertexBuffer) ||
                !r3_buf_alloc(UF_GPU_PROGRAM_BUFFER_SIZE, &fragmentBuffer)) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- source buffer alloc failed\n");
                return 0;
            }
            
            // load program source buffers
            if (!r3_buf_load(program.vertex.vertexPath, &vertexBuffer) ||
                !r3_buf_load(program.vertex.fragmentPath, &fragmentBuffer)) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- source buffer load failed\n");
                return 0;
            }

            s32 vertex_shader = UFGL.createShader(UFGL_VERTEX_SHADER);
            s32 fragment_shader = UFGL.createShader(UFGL_FRAGMENT_SHADER);

            // compile and validate vertex shader
            UFGL.shaderSource(vertex_shader, 1, &(CString){vertexBuffer.data}, NULL);
            UFGL.compileShader(vertex_shader);
            UFGL.getShaderiv(vertex_shader, UFGL_COMPILE_STATUS, &compile_status);
            if (!compile_status) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- failed to compile vertex shader\n");
                UFGL.deleteShader(vertex_shader);
                return 0;
            } else { r3_log_stdout(INFO_LOG, "[UFGPU] `newProgram` -- compiled vertex shader\n"); }

            // compile and validate fragment shader
            UFGL.shaderSource(fragment_shader, 1, &(CString){fragmentBuffer.data}, NULL);
            UFGL.compileShader(fragment_shader);
            UFGL.getShaderiv(fragment_shader, UFGL_COMPILE_STATUS, &compile_status);
            if (!compile_status) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- failed to compile fragment shader\n");
                UFGL.deleteShader(vertex_shader);
                UFGL.deleteShader(fragment_shader);
                return 0;
            } else { r3_log_stdout(INFO_LOG, "[UFGPU] `newProgram` -- compiled fragment shader\n"); }

            // link and validate shader
            u32 glProgram = UFGL.createProgram();
            UFGL.attachShader(glProgram, vertex_shader);
            UFGL.attachShader(glProgram, fragment_shader);
            UFGL.linkProgram(glProgram);
            UFGL.getProgramiv(glProgram, UFGL_LINK_STATUS, &link_status);
            if (!link_status) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- failed to link program\n");
                UFGL.deleteShader(vertex_shader);
                UFGL.deleteShader(fragment_shader);
                return 0;
            }

            // write program into internal array
            if (!r3_arr_write(count, &glProgram, &UFGL.programs)) {
                r3_log_stdout(ERROR_LOG, "[UFGPU] Failed `newProgram` -- program array write failed\n");
                UFGL.deleteShader(fragment_shader);
                UFGL.deleteShader(vertex_shader);
                UFGL.deleteProgram(glProgram);
                return 0;
            }

            // cleanup
            r3_buf_dealloc(&vertexBuffer);
            r3_buf_dealloc(&fragmentBuffer);
            UFGL.deleteShader(vertex_shader);
            UFGL.deleteShader(fragment_shader);
        } break;
        case (UF_GPU_COMPUTE_PROGRAM):  // fall through default
        case (UF_GPU_GEOMETRY_PROGRAM): // fall through default
        case (UF_GPU_PROGRAM_TYPES):    // fall through default
        default: {
            r3_log_stdoutf(ERROR_LOG, "[UFGPU] Failed `newProgram` -- invalid program type: %d\n", program.type);
            return 0;
        }
    }

    return r3_arr_count(&UFGL.programs);
}

u8 bindProgram(UFResource program) {
    u8 count = r3_arr_count(&UFGL.programs);
    if (!program || program > count || program > UF_GPU_RESOURCE_MAX) {
        r3_log_stdoutf(ERROR_LOG, "[UFGPU] Failed `delProgram` -- program not found: %d\n", program);
        return 0;
    }

    u32 glProgram = 0;
    if (!r3_arr_read(program - 1, &glProgram, &UFGL.programs)) {
        r3_log_stdoutf(ERROR_LOG, "[UFGPU] Failed `delProgram` -- program array read failed: %d\n", program);
        return 0;
    }

    UFGL.useProgram(glProgram);
    return 1;
}

u8 delProgram(UFResource program) {
    u8 count = r3_arr_count(&UFGL.programs);
    if (!program || program > count || program > UF_GPU_RESOURCE_MAX) {
        r3_log_stdoutf(ERROR_LOG, "[UFGPU] Failed `delProgram` -- program not found: %d\n", program);
        return 0;
    }

    u32 glProgram = 0;
    if (!r3_arr_pull(program - 1, &glProgram, &UFGL.programs)) {
        r3_log_stdoutf(ERROR_LOG, "[UFGPU] Failed `delProgram` -- program array pull failed: %d\n", program);
        return 0;
    }
    
    UFGL.deleteProgram(glProgram);
    return 1;
}


// Uniforms
// backend may support setting uniforms by index/slot
none setUniformInt(UFResource program, s32 slot, void* data) { return; }
none sendUniformInt(UFResource program, s32 slot) { return; }

// backend may support setting uniforms by string alias
none setUniformStr(UFResource program, char* alias, void* data) { return; }
none sendUniformStr(UFResource program, char* alias) { return; }


// TODO: buffer map array for safe deletes (every index > deleted -= 1)
// Buffers
none clearDepthBuffer(f32 depth) {
    UFGL.clearDepth(depth);
    UFGL.clear(GL_DEPTH_BUFFER_BIT);
}

none clearColorBuffer(Vec4 color) {
    UFGL.clearColor(color.data[0] / 255, color.data[1] / 255, color.data[2] / 255, color.data[3] / 255);
    UFGL.clear(GL_COLOR_BUFFER_BIT);
}


UFResource newBuffer(UFGPUBufferDesc buffer) { return 0; }
none delBuffer(UFResource buffer) { return; }


none bindBuffer(UFResource buffer) { return; }
// 'draw' equivalent for vertex/element buffers
none readBuffer(UFResource buffer) { return; }
// 'update' equivalent
none writeBuffer(u64 size, ptr data, UFResource buffer) { return; }


u8 ufInitGPU(UFResource window, UFOSInterface* osPtr, UFGPUInterface* gpuInterface) {
    if (!osPtr->newGpuCtx(window)) {
        r3_log_stdout(ERROR_LOG, "[UFGPU] Failed init -- `newGpuCtx` failed\n");
        return 0;
    }
    
    if (!r3_arr_alloc(UF_GPU_RESOURCE_MAX, sizeof(u32), &UFGL.buffers) ||
        !r3_arr_alloc(UF_GPU_RESOURCE_MAX, sizeof(u32), &UFGL.programs)) {
            r3_log_stdout(ERROR_LOG, "[UFGPU] Failed init -- internal array alloc failed\n");
            return 0;
        }

    struct gl_func {
        ptr* function;
        CString name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(ptr*)&UFGL.genBuffers, "glGenBuffers"},
        {(ptr*)&UFGL.bindBuffer, "glBindBuffer"},
        {(ptr*)&UFGL.bufferData, "glBufferData"},
        {(ptr*)&UFGL.mapBuffer, "glMapBuffer"},
        {(ptr*)&UFGL.unmapBuffer, "glUnmapBuffer"},
        {(ptr*)&UFGL.bufferSubData, "glBufferSubData"},
        {(ptr*)&UFGL.getBufferParameteriv, "glGetBufferParameteriv"},
        {(ptr*)&UFGL.deleteBuffers, "glDeleteBuffers"},

        // VAO FUNCTIONS
        {(ptr*)&UFGL.genVertexArrays, "glGenVertexArrays"},
        {(ptr*)&UFGL.bindVertexArray, "glBindVertexArray"},
        {(ptr*)&UFGL.deleteVertexArrays, "glDeleteVertexArrays"},
        {(ptr*)&UFGL.enableVertexAttribArray, "glEnableVertexAttribArray"},
        {(ptr*)&UFGL.disableVertexAttribArray, "glDisableVertexAttribArray"},
        {(ptr*)&UFGL.vertexAttribPointer, "glVertexAttribPointer"},

        // SHADER FUNCTIONS
        {(ptr*)&UFGL.createShader, "glCreateShader"},
        {(ptr*)&UFGL.shaderSource, "glShaderSource"},
        {(ptr*)&UFGL.compileShader, "glCompileShader"},
        {(ptr*)&UFGL.deleteShader, "glDeleteShader"},
        {(ptr*)&UFGL.getShaderiv, "glGetShaderiv"},
        {(ptr*)&UFGL.getShaderInfoLog, "glGetShaderInfoLog"},

        {(ptr*)&UFGL.createProgram, "glCreateProgram"},
        {(ptr*)&UFGL.attachShader, "glAttachShader"},
        {(ptr*)&UFGL.detachShader, "glDetachShader"},
        {(ptr*)&UFGL.linkProgram, "glLinkProgram"},
        {(ptr*)&UFGL.useProgram, "glUseProgram"},
        {(ptr*)&UFGL.deleteProgram, "glDeleteProgram"},
        {(ptr*)&UFGL.getProgramiv, "glGetProgramiv"},
        {(ptr*)&UFGL.getProgramInfoLog, "glGetProgramInfoLog"},
        {(ptr*)&UFGL.getUniformLocation, "glGetUniformLocation"},

        // UNIFORMS
        {(ptr*)&UFGL.uniform1i, "glUniform1i"},
        {(ptr*)&UFGL.uniform1f, "glUniform1f"},
        {(ptr*)&UFGL.uniform2fv, "glUniform2fv"},
        {(ptr*)&UFGL.uniform3fv, "glUniform3fv"},
        {(ptr*)&UFGL.uniform4fv, "glUniform4fv"},
        {(ptr*)&UFGL.uniformMatrix4fv, "glUniformMatrix4fv"},

        // TEXTURES
        {(ptr*)&UFGL.genTextures, "glGenTextures"},
        {(ptr*)&UFGL.bindTexture, "glBindTexture"},
        {(ptr*)&UFGL.texParameteri, "glTexParameteri"},
        {(ptr*)&UFGL.texImage2d, "glTexImage2D"},
        {(ptr*)&UFGL.activeTexture, "glActiveTexture"},
        {(ptr*)&UFGL.deleteTextures, "glDeleteTextures"},
        {(ptr*)&UFGL.generateMipmap, "glGenerateMipmap"},

        // FRAMEBUFFERS + RENDERBUFFERS
        {(ptr*)&UFGL.genFramebuffers, "glGenFramebuffers"},
        {(ptr*)&UFGL.bindFramebuffer, "glBindFramebuffer"},
        {(ptr*)&UFGL.framebufferTexture2d, "glFramebufferTexture2D"},
        {(ptr*)&UFGL.framebufferRenderbuffer, "glFramebufferRenderbuffer"},
        {(ptr*)&UFGL.checkFramebufferStatus, "glCheckFramebufferStatus"},
        {(ptr*)&UFGL.deleteFramebuffers, "glDeleteFramebuffers"},

        {(ptr*)&UFGL.genRenderbuffers, "glGenRenderbuffers"},
        {(ptr*)&UFGL.bindRenderbuffer, "glBindRenderbuffer"},
        {(ptr*)&UFGL.renderbufferStorage, "glRenderbufferStorage"},
        {(ptr*)&UFGL.deleteRenderbuffers, "glDeleteRenderbuffers"},

        // DRAWING
        {(ptr*)&UFGL.drawArrays, "glDrawArrays"},
        {(ptr*)&UFGL.drawElements, "glDrawElements"},

        // STATE
        {(ptr*)&UFGL.enable, "glEnable"},
        {(ptr*)&UFGL.disable, "glDisable"},
        {(ptr*)&UFGL.blendFunc, "glBlendFunc"},
        {(ptr*)&UFGL.cullFace, "glCullFace"},
        {(ptr*)&UFGL.depthFunc, "glDepthFunc"},
        {(ptr*)&UFGL.viewport, "glViewport"},
        {(ptr*)&UFGL.polygonMode, "glPolygonMode"},
        {(ptr*)&UFGL.clear, "glClear"},
        {(ptr*)&UFGL.clearColor, "glClearColor"},
        {(ptr*)&UFGL.clearDepth, "glClearDepth"},

        // INFO / DEBUG
        {(ptr*)&UFGL.getError, "glGetError"},
        {(ptr*)&UFGL.getString, "glGetString"}
    };

    UFLibrary libGL;
    if (!osPtr->loadLibrary(NULL, "opengl32", &libGL)) {
        osPtr->delGpuCtx(window);
        r3_log_stdout(ERROR_LOG, "[UFGPU] Failed init -- `loadLibrary` failed\n");
        return 0;
    } else r3_log_stdoutf(SUCCESS_LOG, "[UFGPU] OpenGL v%s %s %s\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER));

    FOR_I(0, sizeof(functions) / sizeof(functions[0]), 1) {
        if (!osPtr->loadSymbol(functions[i].name, functions[i].function, &libGL) || *functions[i].function == NULL) {
            r3_log_stdoutf(WARN_LOG, "[UFGPU] Failed `loadSymbol` --%s\n", functions[i].name);
        }
    }
    
    osPtr->unloadLibrary(&libGL);

    // populate UFGPUInterface
    gpuInterface->newProgram = newProgram;
    gpuInterface->bindProgram = bindProgram;
    gpuInterface->delProgram = delProgram;

    gpuInterface->setUniformInt = setUniformInt;
    gpuInterface->sendUniformInt = sendUniformInt;
    
    gpuInterface->setUniformStr = setUniformStr;
    gpuInterface->sendUniformStr = sendUniformStr;
    
    gpuInterface->clearDepthBuffer = clearDepthBuffer;
    gpuInterface->clearColorBuffer = clearColorBuffer;
    
    gpuInterface->newBuffer = newBuffer;
    gpuInterface->delBuffer = delBuffer;
    
    gpuInterface->bindBuffer = bindBuffer;
    gpuInterface->readBuffer = readBuffer;
    gpuInterface->writeBuffer = writeBuffer;

    r3_log_stdout(SUCCESS_LOG, "[UFGPU] Initialized\n");
    return 1;
}

u8 ufExitGPU(UFGPUInterface* gpuInterface) {
    if (!r3_arr_dealloc(&UFGL.buffers) ||
        !r3_arr_dealloc(&UFGL.programs)) {
            r3_log_stdout(ERROR_LOG, "[UFGPU] Error during exit -- internal array dealloc failed\n");
        }

    gpuInterface->newProgram = NULL;
    gpuInterface->bindProgram = NULL;
    gpuInterface->delProgram = NULL;

    gpuInterface->setUniformInt = NULL;
    gpuInterface->sendUniformInt = NULL;
    
    gpuInterface->setUniformStr = NULL;
    gpuInterface->sendUniformStr = NULL;
    
    gpuInterface->clearDepthBuffer = NULL;
    gpuInterface->clearColorBuffer = NULL;
    
    gpuInterface->newBuffer = NULL;
    gpuInterface->delBuffer = NULL;
    
    gpuInterface->bindBuffer = NULL;
    gpuInterface->readBuffer = NULL;
    gpuInterface->writeBuffer = NULL;

    r3_log_stdout(SUCCESS_LOG, "[UFGPU] Deinitialized\n");
    return 1;
}
