#include "../include/graphics/lotus_gl.h"

#include "../include/utility/lotus_array.h"
#include "../include/platform/lotus_logger.h"

static Lotus_Graphics_API internal_graphics_api = {0};
static Lotus_Graphics_State internal_graphics_state = {0};

Lotus_Graphics_State* _graphics_initialize_impl(void) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = LOTUS_TRIANGLE_MODE;
    internal_graphics_state.shader = NULL;
    internal_graphics_state.projection = lotus_identity();
    internal_graphics_state.clear_color = LOTUS_COLOR4(133, 161, 172, 255);

    // OpenGL State Config
    internal_graphics_api.GL_API.enable(GL_DEPTH_TEST);
    internal_graphics_api.GL_API.enable(GL_BLEND);
    internal_graphics_api.GL_API.blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return &internal_graphics_state;
}

void _graphics_shutdown_impl(void) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = LOTUS_DRAW_MODES;
    internal_graphics_state.shader = NULL;
    internal_graphics_state.projection = lotus_identity();
    internal_graphics_state.clear_color = LOTUS_COLOR4(0, 0, 0, 0);

    internal_graphics_api.GL_API.disable(GL_DEPTH_TEST);
    internal_graphics_api.GL_API.disable(GL_BLEND);
}


Lotus_Vertex_Data _graphics_make_vertex_data_impl(f32* vertices, ubyte4 vertex_count, ubyte4* indices, ubyte4 index_count, ubyte attrs) {
    if ((attrs & ~((1 << LOTUS_VERTEX_ATTRIBS) - 1)) != 0 || !vertices) {
        return (Lotus_Vertex_Data){0};
    }

    Lotus_Vertex_Data vertex_data;

    // calculate stride and offsets dynamically
    ubyte4 stride = 0;
    ubyte4 offsets[LOTUS_VERTEX_ATTRIBS] = {0};
    const ubyte4 attr_sizes[LOTUS_VERTEX_ATTRIBS] = {
        3, // LOTUS_LOCATION_ATTR:  vec3
        3, // LOTUS_COLOR_ATTR:     vec3
        2, // LOTUS_TCOORD_ATTR:    vec2
        2  // LOTUS_NORMAL_ATTR:    vec2
    };

    for (int i = 0; i < LOTUS_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            // accumulate stride for enabled vertex attributes
            offsets[i] = stride;
            stride += attr_sizes[i];
        }
    }

    internal_graphics_api.GL_API.gen_vertex_arrays(1, &vertex_data.vao);
    internal_graphics_api.GL_API.gen_buffers(1, &vertex_data.vbo);

    internal_graphics_api.GL_API.bind_vertex_array(vertex_data.vao);
    internal_graphics_api.GL_API.bind_buffer(LOTUS_ARRAY_BUFFER, vertex_data.vbo);
    
    size_t vertex_data_size = vertex_count * (stride * sizeof(f32));
    internal_graphics_api.GL_API.buffer_data(LOTUS_ARRAY_BUFFER, vertex_data_size, vertices, LOTUS_STATIC_DRAW);

    // generate EBO if indices are provided
    if (index_count > 0 && indices) {
        internal_graphics_api.GL_API.gen_buffers(1, &vertex_data.ebo);

        internal_graphics_api.GL_API.bind_buffer(LOTUS_ELEMENT_ARRAY_BUFFER, vertex_data.ebo);
        
        size_t index_data_size = index_count * sizeof(ubyte4);
        internal_graphics_api.GL_API.buffer_data(LOTUS_ELEMENT_ARRAY_BUFFER, index_data_size, indices, LOTUS_STATIC_DRAW);
        
        vertex_data.indices = indices;
        vertex_data.index_count = index_count;
    } else {
        vertex_data.ebo = 0;
        vertex_data.indices = NULL;
        vertex_data.index_count = 0;
    }

    // configure vertex attributes dynamically
    for (int i = 0; i < LOTUS_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            internal_graphics_api.GL_API.vertex_attrib_pointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            internal_graphics_api.GL_API.enable_vertex_attrib_array(i);
        }
    }

    internal_graphics_api.GL_API.bind_buffer(LOTUS_ARRAY_BUFFER, 0);
    internal_graphics_api.GL_API.bind_vertex_array(0);

    vertex_data.attrs = attrs;
    vertex_data.vertices = vertices;
    vertex_data.vertex_count = vertex_count;

    return vertex_data;
}

void _graphics_destroy_vertex_data_impl(Lotus_Vertex_Data* vertex_data) {
    vertex_data->attrs = 0;
    
    internal_graphics_api.GL_API.delete_buffers(1, &vertex_data->vbo);
    vertex_data->vbo = 0;
    
    internal_graphics_api.GL_API.delete_buffers(1, &vertex_data->ebo);
    vertex_data->ebo = 0;
    
    internal_graphics_api.GL_API.delete_vertex_arrays(1, &vertex_data->vao);
    vertex_data->vao = 0;
    
    vertex_data->vertices = NULL;
    vertex_data->vertex_count = 0;
    vertex_data->indices = NULL;
    vertex_data->index_count = 0;
}


Lotus_Shader _graphics_make_shader_impl(const char* vertex_shader, const char* fragment_shader) {
    lotus_set_log_level(LOTUS_LOG_ERROR);
    ubyte4 link = 0;
    ubyte4 compile = 0;

    Lotus_Shader program = {.uniforms=lotus_make_hashmap(16)};
    if (!program.uniforms) {
        lotus_log_error("failed to allocate uniform hashmap!\n");
        return (Lotus_Shader){0};
    }

    program.program = internal_graphics_api.GL_API.create_program();
    sbyte4 v_Shader = internal_graphics_api.GL_API.create_shader(LOTUS_VERTEX_SHADER);
    sbyte4 f_Shader = internal_graphics_api.GL_API.create_shader(LOTUS_FRAGMENT_SHADER);

    internal_graphics_api.GL_API.shader_source(v_Shader, 1, &vertex_shader, NULL);
    internal_graphics_api.GL_API.compile_shader(v_Shader);
    internal_graphics_api.GL_API.get_shaderiv(v_Shader, LOTUS_COMPILE_STATUS, &compile);
    if (!compile) {
        lotus_log_error("failed to compile vertex-shader!\n");
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }
    
    internal_graphics_api.GL_API.shader_source(f_Shader, 1, &fragment_shader, NULL);
    internal_graphics_api.GL_API.compile_shader(f_Shader);
    internal_graphics_api.GL_API.get_shaderiv(f_Shader, LOTUS_COMPILE_STATUS, &compile);
    if (!compile) {
        lotus_log_error("failed to compile fragment-shader!\n");
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }

    internal_graphics_api.GL_API.attach_shader(program.program, v_Shader);
    internal_graphics_api.GL_API.attach_shader(program.program, f_Shader);
    internal_graphics_api.GL_API.link_program(program.program);
    internal_graphics_api.GL_API.get_programiv(program.program, LOTUS_LINK_STATUS, &link);
    if (!link) {
        lotus_log_error("failed to link shader program!\n");
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }

    internal_graphics_api.GL_API.delete_shader(v_Shader);
    internal_graphics_api.GL_API.delete_shader(f_Shader);
    return program;

}

void _graphics_destroy_shader_impl(Lotus_Shader* shader) {
    lotus_destroy_hashmap(shader->uniforms);
    internal_graphics_api.GL_API.delete_program(shader->program);
}


void _graphics_set_uniform_impl(Lotus_Shader* shader, const char* name, void* value) {
    lotus_set_hashmap(shader->uniforms, name, value);
}

Lotus_Uniform _graphics_get_uniform_impl(Lotus_Shader* shader, const char* name) {
    sbyte4 location = internal_graphics_api.GL_API.get_uniform_location(shader->program, name);
    if (location < 0) return (Lotus_Uniform){0};
    
    void* value = lotus_get_hashmap(shader->uniforms, name);
    if (!value) return (Lotus_Uniform){0};
    
    return (Lotus_Uniform){.location=location, .name=name, .value=value};
}

void _graphics_send_uniform_impl(Lotus_Shader* shader, Lotus_Uniform_Type type, const char* name) {
    Lotus_Uniform uniform = internal_graphics_api.get_uniform(shader, name);
    if (uniform.location < 0) return;
    
    internal_graphics_api.GL_API.use_program(shader->program);
    switch (type) {
        case LOTUS_UNIFORM_NONE: break;
        case LOTUS_UNIFORM_TYPES: break;
        case LOTUS_UNIFORM_VEC2: internal_graphics_api.GL_API.uniform2fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_VEC3: internal_graphics_api.GL_API.uniform3fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_VEC4: internal_graphics_api.GL_API.uniform4fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_MAT4: internal_graphics_api.GL_API.uniform_matrix4fv(uniform.location, 1, 0, uniform.value); break;
        default: break;
    }
}


void _graphics_set_color_impl(Lotus_Vec4 color4) {
    internal_graphics_state.clear_color = color4;
}

void _graphics_set_mode_impl(Lotus_Draw_Mode mode) {
    if (internal_graphics_state.mode != mode) {
        internal_graphics_state.mode = mode;
    }
}

void _graphics_set_shader_impl(Lotus_Shader* shader) {
    internal_graphics_state.shader = (shader->program < 0) ? NULL : shader;
    internal_graphics_api.set_uniform(internal_graphics_state.shader, "u_projection", &internal_graphics_state.projection);
}


void _graphics_draw_begin_impl(Lotus_Draw_Mode mode, Lotus_Vec4 color4, Lotus_Mat4 projection) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = mode;
    internal_graphics_state.projection = projection;
    internal_graphics_state.clear_color = color4;
}

void _graphics_draw_impl(Lotus_Vertex_Data vertex_data) {
    if (vertex_data.vertex_count <= 0 || !vertex_data.vertices || vertex_data.vao < 0) {
        return; // error: invalid vertex data for draw call
    }; if (internal_graphics_state.draws >= LOTUS_MAX_DRAW_CALLS) {
        return; // error: exceeded max draw buffers
    }
    
    ubyte8 index = internal_graphics_state.draws;
 
    Lotus_Draw_Call* draw_call = &internal_graphics_state.draw_calls[index];
    draw_call->mode = internal_graphics_state.mode;
    draw_call->shader = internal_graphics_state.shader;
    draw_call->vertex_data = vertex_data;

    internal_graphics_state.draws++;
}

void _graphics_draw_end_impl(void) {
    internal_graphics_api.GL_API.clear_color(
        internal_graphics_state.clear_color.x,
        internal_graphics_state.clear_color.y,
        internal_graphics_state.clear_color.z,
        internal_graphics_state.clear_color.w
    ); internal_graphics_api.GL_API.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (ubyte8 i = 0; i < internal_graphics_state.draws; ++i) {
        Lotus_Draw_Call draw_call = internal_graphics_state.draw_calls[i];


        if (internal_graphics_state.shader && internal_graphics_state.shader->program >= 0) {
            internal_graphics_api.GL_API.use_program(internal_graphics_state.shader->program);
            internal_graphics_api.send_uniform(internal_graphics_state.shader, LOTUS_UNIFORM_MAT4, "u_model");
            internal_graphics_api.send_uniform(internal_graphics_state.shader, LOTUS_UNIFORM_MAT4, "u_projection");
        }

        internal_graphics_api.GL_API.bind_vertex_array(draw_call.vertex_data.vao);
        if (draw_call.vertex_data.indices && draw_call.vertex_data.index_count > 0) {
            internal_graphics_api.GL_API.draw_elements(internal_graphics_state.mode, draw_call.vertex_data.index_count, GL_UNSIGNED_INT, NULL);
        } else {
            internal_graphics_api.GL_API.draw_arrays(internal_graphics_state.mode, 0, draw_call.vertex_data.vertex_count);
        }

        internal_graphics_api.GL_API.bind_vertex_array(0);
    }
    internal_graphics_state.draws = 0;
}


#if defined(LOTUS_PLATFORM_WINDOWS)
#include <Windows.h>
#include <windowsx.h>
static void* _get_gl_fn(const char* name) {
    void* proc = (void*)wglGetProcAddress(name);
    if (proc == NULL || (proc == (void*)0x1) || (proc == (void*)0x2) || (proc == (void*)0x3) || (proc == (void*)-1)) {
        proc = (void*)GetProcAddress(LoadLibrary("opengl32.dll"), name);
    }; return proc;
}
#endif

Lotus_Graphics_API* lotus_init_graphics(void) {
    lotus_set_log_level(LOTUS_LOG_FATAL);

    internal_graphics_api = (Lotus_Graphics_API) {
        .initialize = _graphics_initialize_impl,
        .shutdown = _graphics_shutdown_impl,
        
        .make_vertex_data = _graphics_make_vertex_data_impl,
        .destroy_vertex_data = _graphics_destroy_vertex_data_impl,

        .make_shader = _graphics_make_shader_impl,
        .destroy_shader = _graphics_destroy_shader_impl,
        
        .set_uniform = _graphics_set_uniform_impl,
        .get_uniform = _graphics_get_uniform_impl,
        .send_uniform = _graphics_send_uniform_impl,
        
        .set_color = _graphics_set_color_impl,
        .set_mode = _graphics_set_mode_impl,
        .set_shader = _graphics_set_shader_impl,
        
        .draw_begin = _graphics_draw_begin_impl,
        .draw = _graphics_draw_impl,
        .draw_end = _graphics_draw_end_impl,

        .GL_API = {NULL}
    };

    struct {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.gen_buffers, "glGenBuffers"},
        {(void**)&internal_graphics_api.GL_API.bind_buffer, "glBindBuffer"},
        {(void**)&internal_graphics_api.GL_API.buffer_data, "glBufferData"},
        {(void**)&internal_graphics_api.GL_API.map_buffer, "glMapBuffer"},
        {(void**)&internal_graphics_api.GL_API.unmap_buffer, "glUnmapBuffer"},
        {(void**)&internal_graphics_api.GL_API.buffer_sub_data, "glBufferSubData"},
        {(void**)&internal_graphics_api.GL_API.get_buffer_parameteriv, "glGetBufferParameteriv"},
        {(void**)&internal_graphics_api.GL_API.delete_buffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {(void**)&internal_graphics_api.GL_API.disable_vertex_attrib_array, "glDisableVertexAttribArray"},
        {(void**)&internal_graphics_api.GL_API.vertex_attrib_pointer, "glVertexAttribPointer"},
        {(void**)&internal_graphics_api.GL_API.bind_vertex_array, "glBindVertexArray"},
        {(void**)&internal_graphics_api.GL_API.gen_vertex_arrays, "glGenVertexArrays"},
        {(void**)&internal_graphics_api.GL_API.delete_vertex_arrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.create_program, "glCreateProgram"},
        {(void**)&internal_graphics_api.GL_API.use_program, "glUseProgram"},
        {(void**)&internal_graphics_api.GL_API.link_program, "glLinkProgram"},
        {(void**)&internal_graphics_api.GL_API.delete_program, "glDeleteProgram"},
        {(void**)&internal_graphics_api.GL_API.create_shader, "glCreateShader"},
        {(void**)&internal_graphics_api.GL_API.compile_shader, "glCompileShader"},
        {(void**)&internal_graphics_api.GL_API.attach_shader, "glAttachShader"},
        {(void**)&internal_graphics_api.GL_API.detach_shader, "glDetachShader"},
        {(void**)&internal_graphics_api.GL_API.delete_shader, "glDeleteShader"},
        {(void**)&internal_graphics_api.GL_API.shader_source, "glShaderSource"},
        {(void**)&internal_graphics_api.GL_API.get_shaderiv, "glGetShaderiv"},
        {(void**)&internal_graphics_api.GL_API.get_shader_info_log, "glGetShaderInfoLog"},
        {(void**)&internal_graphics_api.GL_API.get_programiv, "glGetProgramiv"},
        {(void**)&internal_graphics_api.GL_API.get_program_info_log, "glGetProgramInfoLog"},
        {(void**)&internal_graphics_api.GL_API.get_uniform_location, "glGetUniformLocation"},
        {(void**)&internal_graphics_api.GL_API.uniform2fv, "glUniform2fv"},
        {(void**)&internal_graphics_api.GL_API.uniform3fv, "glUniform3fv"},
        {(void**)&internal_graphics_api.GL_API.uniform4fv, "glUniform4fv"},
        {(void**)&internal_graphics_api.GL_API.uniform_matrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.gen_textures, "glGenTextures"},
        {(void**)&internal_graphics_api.GL_API.bind_texture, "glBindTexture"},
        {(void**)&internal_graphics_api.GL_API.tex_parameteri, "glTexParameteri"},
        {(void**)&internal_graphics_api.GL_API.tex_image2d, "glTexImage2D"},
        {(void**)&internal_graphics_api.GL_API.active_texture, "glActiveTexture"},
        {(void**)&internal_graphics_api.GL_API.delete_textures, "glDeleteTextures"},
        {(void**)&internal_graphics_api.GL_API.generate_mipmap, "glGenerateMipmap"},
        {(void**)&internal_graphics_api.GL_API.generate_texture_mipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.gen_framebuffers, "glGenFramebuffers"},
        {(void**)&internal_graphics_api.GL_API.bind_framebuffer, "glBindFramebuffer"},
        {(void**)&internal_graphics_api.GL_API.framebuffer_texture2d, "glFramebufferTexture2D"},
        {(void**)&internal_graphics_api.GL_API.delete_framebuffers, "glDeleteFramebuffers"},
        {(void**)&internal_graphics_api.GL_API.check_framebuffer_status, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.clear, "glClear"},
        {(void**)&internal_graphics_api.GL_API.clear_color, "glClearColor"},
        {(void**)&internal_graphics_api.GL_API.draw_arrays, "glDrawArrays"},
        {(void**)&internal_graphics_api.GL_API.draw_elements, "glDrawElements"},
        {(void**)&internal_graphics_api.GL_API.draw_elements_base_vertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.enable, "glEnable"},
        {(void**)&internal_graphics_api.GL_API.disable, "glDisable"},
        {(void**)&internal_graphics_api.GL_API.blend_func, "glBlendFunc"},
        {(void**)&internal_graphics_api.GL_API.cull_face, "glCullFace"},
        {(void**)&internal_graphics_api.GL_API.depth_func, "glDepthFunc"},
        {(void**)&internal_graphics_api.GL_API.viewport, "glViewport"},
        {(void**)&internal_graphics_api.GL_API.polygon_mode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&internal_graphics_api.GL_API.get_error, "glGetError"},
        {(void**)&internal_graphics_api.GL_API.get_string, "glGetString"}
    };

    // Load all functions
    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = _get_gl_fn(functions[i].name);
        if (!*functions[i].function) {
            lotus_log_fatal("Failed to load GL function: %s\n", functions[i].name);
        }
    }    

    return &internal_graphics_api;
}
