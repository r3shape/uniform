#include "../../include/graphics/lotus_gl.h"
#include "../../include/memory/lotus_memory.h"

#include <stdio.h>

Lotus_Graphics_API *lotus_graphics_api = NULL;
static Lotus_Graphics_State internal_graphics_state = {0};

/* GRAPHICS API IMPLEMENTATION */
Lotus_Graphics_State *_graphics_get_state_impl(void) { return &internal_graphics_state; }

void _graphics_cleanup_impl(void) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = LOTUS_DRAW_MODES;
    internal_graphics_state.shader = NULL;
    internal_graphics_state.projection = lotus_identity();
    internal_graphics_state.clearColor = LOTUS_COLOR4(0, 0, 0, 0);

    lotus_graphics_api->GL_API.disable(GL_DEPTH_TEST);
    lotus_graphics_api->GL_API.disable(GL_BLEND);
}


Lotus_Vertex_Data _graphics_make_vertex_data_impl(f32 *vertices, ubyte4 vertexCount, ubyte4 *indices, ubyte4 indexCount, ubyte attrs) {
    if ((attrs & ~((1 << LOTUS_VERTEX_ATTRIBS) - 1)) != 0 || !vertices) {
        return (Lotus_Vertex_Data){0};
    }

    Lotus_Vertex_Data vertexData;

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

    lotus_graphics_api->GL_API.gen_vertex_arrays(1, &vertexData.vao);
    lotus_graphics_api->GL_API.gen_buffers(1, &vertexData.vbo);

    lotus_graphics_api->GL_API.bind_vertex_array(vertexData.vao);
    lotus_graphics_api->GL_API.bind_buffer(LOTUS_ARRAY_BUFFER, vertexData.vbo);
    
    size_t vertex_data_size = vertexCount * (stride * sizeof(f32));
    lotus_graphics_api->GL_API.buffer_data(LOTUS_ARRAY_BUFFER, vertex_data_size, vertices, LOTUS_STATIC_DRAW);

    // generate EBO if indices are provided
    if (indexCount > 0 && indices) {
        lotus_graphics_api->GL_API.gen_buffers(1, &vertexData.ebo);

        lotus_graphics_api->GL_API.bind_buffer(LOTUS_ELEMENT_ARRAY_BUFFER, vertexData.ebo);
        
        size_t index_data_size = indexCount * sizeof(ubyte4);
        lotus_graphics_api->GL_API.buffer_data(LOTUS_ELEMENT_ARRAY_BUFFER, index_data_size, indices, LOTUS_STATIC_DRAW);
        
        vertexData.indices = indices;
        vertexData.indexCount = indexCount;
    } else {
        vertexData.ebo = 0;
        vertexData.indices = NULL;
        vertexData.indexCount = 0;
    }

    // configure vertex attributes dynamically
    for (int i = 0; i < LOTUS_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            lotus_graphics_api->GL_API.vertex_attrib_pointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            lotus_graphics_api->GL_API.enable_vertex_attrib_array(i);
        }
    }

    lotus_graphics_api->GL_API.bind_buffer(LOTUS_ARRAY_BUFFER, 0);
    lotus_graphics_api->GL_API.bind_vertex_array(0);

    vertexData.attrs = attrs;
    vertexData.vertices = vertices;
    vertexData.vertexCount = vertexCount;

    return vertexData;
}

void _graphics_destroy_vertex_data_impl(Lotus_Vertex_Data *vertexData) {
    vertexData->attrs = 0;
    
    lotus_graphics_api->GL_API.delete_buffers(1, &vertexData->vbo);
    vertexData->vbo = 0;
    
    lotus_graphics_api->GL_API.delete_buffers(1, &vertexData->ebo);
    vertexData->ebo = 0;
    
    lotus_graphics_api->GL_API.delete_vertex_arrays(1, &vertexData->vao);
    vertexData->vao = 0;
    
    vertexData->vertices = NULL;
    vertexData->vertexCount = 0;
    vertexData->indices = NULL;
    vertexData->indexCount = 0;
}


Lotus_Shader _graphics_create_shader_impl(const char *vertex_shader, const char *fragment_shader) {
    ubyte4 link = 0;
    ubyte4 compile = 0;

    Lotus_Shader program = {.uniforms=lotus_create_hashmap(16)};
    if (!program.uniforms) {
        // error: failed to allocate uniform hashmap!
        return (Lotus_Shader){0};
    }

    program.program = lotus_graphics_api->GL_API.create_program();
    sbyte4 v_Shader = lotus_graphics_api->GL_API.create_shader(LOTUS_VERTEX_SHADER);
    sbyte4 f_Shader = lotus_graphics_api->GL_API.create_shader(LOTUS_FRAGMENT_SHADER);

    lotus_graphics_api->GL_API.shader_source(v_Shader, 1, &vertex_shader, NULL);
    lotus_graphics_api->GL_API.compile_shader(v_Shader);
    lotus_graphics_api->GL_API.get_shaderiv(v_Shader, LOTUS_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile vertex-shader!
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }
    
    lotus_graphics_api->GL_API.shader_source(f_Shader, 1, &fragment_shader, NULL);
    lotus_graphics_api->GL_API.compile_shader(f_Shader);
    lotus_graphics_api->GL_API.get_shaderiv(f_Shader, LOTUS_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile fragment-shader!
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }

    lotus_graphics_api->GL_API.attach_shader(program.program, v_Shader);
    lotus_graphics_api->GL_API.attach_shader(program.program, f_Shader);
    lotus_graphics_api->GL_API.link_program(program.program);
    lotus_graphics_api->GL_API.get_programiv(program.program, LOTUS_LINK_STATUS, &link);
    if (!link) {
        // error: failed to link shader program!
        lotus_destroy_hashmap(program.uniforms);
        return (Lotus_Shader){0};
    }

    lotus_graphics_api->GL_API.delete_shader(v_Shader);
    lotus_graphics_api->GL_API.delete_shader(f_Shader);
    return program;

}

void _graphics_destroy_shader_impl(Lotus_Shader *shader) {
    lotus_destroy_hashmap(shader->uniforms);
    lotus_graphics_api->GL_API.delete_program(shader->program);
}


ubyte _graphics_set_uniform_impl(Lotus_Shader *shader, const char *name, void *value) {
    if (!shader || !name) return LOTUS_FALSE;   // error: value error!
    return lotus_set_hashmap(shader->uniforms, name, value);
}

Lotus_Uniform _graphics_get_uniform_impl(Lotus_Shader *shader, const char *name) {
    ubyte4 location = lotus_graphics_api->GL_API.get_uniform_location(shader->program, name);
    if (location < 0) {
        return (Lotus_Uniform){ .location = 0, .value = NULL, .name = NULL };
    }
    
    void *value = lotus_get_hashmap(shader->uniforms, name);
    if (!value) {
        return (Lotus_Uniform){ .location = 0, .value = NULL, .name = NULL };
    }
    
    return (Lotus_Uniform){.location = location, .value = value, .name = name};
}

void _graphics_send_uniform_impl(Lotus_Shader *shader, Lotus_Uniform_Type type, const char *name) {
    Lotus_Uniform uniform = lotus_graphics_api->get_uniform(shader, name);
    if (uniform.location < 0 || !uniform.value) {
        return;
    }
    
    lotus_graphics_api->GL_API.use_program(shader->program);
    switch (type) {
        case LOTUS_UNIFORM_NONE: break;
        case LOTUS_UNIFORM_TYPES: break;
        case LOTUS_UNIFORM_VEC2: lotus_graphics_api->GL_API.uniform2fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_VEC3: lotus_graphics_api->GL_API.uniform3fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_VEC4: lotus_graphics_api->GL_API.uniform4fv(uniform.location, 1, uniform.value); break;
        case LOTUS_UNIFORM_MAT4: lotus_graphics_api->GL_API.uniform_matrix4fv(uniform.location, 1, 0, uniform.value); break;
        default: break;
    }
}

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/graphics/stb_image.h"

Lotus_Texture2D _create_texture2D_impl(char* path, Lotus_Texture_Format format) {
    Lotus_Texture2D texture = { .id = 0, .width = 0, .height = 0, .channels = 0, .path = NULL, .raw = NULL };
    if (!path) return texture;

    texture.path = path;
    stbi_set_flip_vertically_on_load(LOTUS_TRUE);
    texture.raw = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
    if (!texture.raw) return texture;    // error: failed to allocate raw data buffer!

    lotus_graphics_api->GL_API.gen_textures(1, &texture.id);
    lotus_graphics_api->GL_API.bind_texture(GL_TEXTURE_2D, texture.id);

    // set texture wrapping options
    lotus_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x axis
    lotus_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y axis

    // set texture filtering options (scaling up/down)
    lotus_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    lotus_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generate the texture
    lotus_graphics_api->GL_API.tex_image2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.raw);
    lotus_graphics_api->GL_API.generate_mipmap(GL_TEXTURE_2D);

    return texture;
}

void _destroy_texture2D_impl(Lotus_Texture2D* texture) {
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;
    texture ->path = NULL;
    lotus_graphics_api->GL_API.delete_textures(1, &texture->id);
    stbi_image_free(texture->raw);
    texture->id = 0;
}

void _graphics_set_color_impl(Lotus_Vec4 color4) {
    internal_graphics_state.clearColor = color4;
}

void _graphics_set_mode_impl(Lotus_Draw_Mode mode) {
    if (internal_graphics_state.mode != mode) {
        internal_graphics_state.mode = mode;
    }
}

void _graphics_set_shader_impl(Lotus_Shader *shader) {
    internal_graphics_state.shader = (shader->program == 0) ? NULL : shader;
    lotus_graphics_api->set_uniform(internal_graphics_state.shader, "u_projection", &internal_graphics_state.projection);
}


void _graphics_draw_begin_impl(Lotus_Draw_Mode mode, Lotus_Vec4 color4, Lotus_Mat4 projection) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = mode;
    internal_graphics_state.projection = projection;
    internal_graphics_state.clearColor = color4;
}

void _graphics_draw_data_impl(Lotus_Vertex_Data vertexData) {
    if (vertexData.vertexCount <= 0 || !vertexData.vertices || vertexData.vao < 0) {
        return; // error: invalid vertex data for draw call
    };

    if (internal_graphics_state.shader && internal_graphics_state.shader->program > 0) {
        lotus_graphics_api->GL_API.use_program(internal_graphics_state.shader->program);
        lotus_graphics_api->send_uniform(internal_graphics_state.shader, LOTUS_UNIFORM_MAT4, "u_model");
        lotus_graphics_api->send_uniform(internal_graphics_state.shader, LOTUS_UNIFORM_MAT4, "u_view");
        lotus_graphics_api->send_uniform(internal_graphics_state.shader, LOTUS_UNIFORM_MAT4, "u_projection");
    }

    lotus_graphics_api->GL_API.bind_vertex_array(vertexData.vao);
    if (vertexData.indices && vertexData.indexCount > 0) lotus_graphics_api->GL_API.draw_elements(internal_graphics_state.mode, vertexData.indexCount, GL_UNSIGNED_INT, NULL);
    else lotus_graphics_api->GL_API.draw_arrays(internal_graphics_state.mode, 0, vertexData.vertexCount);
    
    lotus_graphics_api->GL_API.bind_vertex_array(0);
}

void _graphics_draw_clear_impl(void) {
    lotus_graphics_api->GL_API.clear_color(
        internal_graphics_state.clearColor.x,
        internal_graphics_state.clearColor.y,
        internal_graphics_state.clearColor.z,
        internal_graphics_state.clearColor.w
    ); lotus_graphics_api->GL_API.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/* Graphics State Setters/Toggles */
void _graphics_wireframe_mode_impl(ubyte toggle) {
    if (toggle) {
        lotus_graphics_api->GL_API.polygon_mode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        lotus_graphics_api->GL_API.polygon_mode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
/* GRAPHICS API IMPLEMENTATION */

#if defined(LOTUS_PLATFORM_WINDOWS)
#include <Windows.h>
#include <windowsx.h>
static void *_get_gl_fn(const char *name) {
    void *proc = (void*)wglGetProcAddress(name);
    if (proc == NULL || (proc == (void*)0x1) || (proc == (void*)0x2) || (proc == (void*)0x3) || (proc == (void*)-1)) {
        proc = (void*)GetProcAddress(LoadLibrary("opengl32.dll"), name);
    }; return proc;
}

void _load_gl_functions(void) {
    struct gl_func {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.gen_buffers, "glGenBuffers"},
        {(void**)&lotus_graphics_api->GL_API.bind_buffer, "glBindBuffer"},
        {(void**)&lotus_graphics_api->GL_API.buffer_data, "glBufferData"},
        {(void**)&lotus_graphics_api->GL_API.map_buffer, "glMapBuffer"},
        {(void**)&lotus_graphics_api->GL_API.unmap_buffer, "glUnmapBuffer"},
        {(void**)&lotus_graphics_api->GL_API.buffer_sub_data, "glBufferSubData"},
        {(void**)&lotus_graphics_api->GL_API.get_buffer_parameteriv, "glGetBufferParameteriv"},
        {(void**)&lotus_graphics_api->GL_API.delete_buffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {(void**)&lotus_graphics_api->GL_API.disable_vertex_attrib_array, "glDisableVertexAttribArray"},
        {(void**)&lotus_graphics_api->GL_API.vertex_attrib_pointer, "glVertexAttribPointer"},
        {(void**)&lotus_graphics_api->GL_API.bind_vertex_array, "glBindVertexArray"},
        {(void**)&lotus_graphics_api->GL_API.gen_vertex_arrays, "glGenVertexArrays"},
        {(void**)&lotus_graphics_api->GL_API.delete_vertex_arrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.create_program, "glCreateProgram"},
        {(void**)&lotus_graphics_api->GL_API.use_program, "glUseProgram"},
        {(void**)&lotus_graphics_api->GL_API.link_program, "glLinkProgram"},
        {(void**)&lotus_graphics_api->GL_API.delete_program, "glDeleteProgram"},
        {(void**)&lotus_graphics_api->GL_API.create_shader, "glCreateShader"},
        {(void**)&lotus_graphics_api->GL_API.compile_shader, "glCompileShader"},
        {(void**)&lotus_graphics_api->GL_API.attach_shader, "glAttachShader"},
        {(void**)&lotus_graphics_api->GL_API.detach_shader, "glDetachShader"},
        {(void**)&lotus_graphics_api->GL_API.delete_shader, "glDeleteShader"},
        {(void**)&lotus_graphics_api->GL_API.shader_source, "glShaderSource"},
        {(void**)&lotus_graphics_api->GL_API.get_shaderiv, "glGetShaderiv"},
        {(void**)&lotus_graphics_api->GL_API.get_shader_info_log, "glGetShaderInfoLog"},
        {(void**)&lotus_graphics_api->GL_API.get_programiv, "glGetProgramiv"},
        {(void**)&lotus_graphics_api->GL_API.get_program_info_log, "glGetProgramInfoLog"},
        {(void**)&lotus_graphics_api->GL_API.get_uniform_location, "glGetUniformLocation"},
        {(void**)&lotus_graphics_api->GL_API.uniform2fv, "glUniform2fv"},
        {(void**)&lotus_graphics_api->GL_API.uniform3fv, "glUniform3fv"},
        {(void**)&lotus_graphics_api->GL_API.uniform4fv, "glUniform4fv"},
        {(void**)&lotus_graphics_api->GL_API.uniform_matrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.gen_textures, "glGenTextures"},
        {(void**)&lotus_graphics_api->GL_API.bind_texture, "glBindTexture"},
        {(void**)&lotus_graphics_api->GL_API.tex_parameteri, "glTexParameteri"},
        {(void**)&lotus_graphics_api->GL_API.tex_image2D, "glTexImage2D"},
        {(void**)&lotus_graphics_api->GL_API.active_texture, "glActiveTexture"},
        {(void**)&lotus_graphics_api->GL_API.delete_textures, "glDeleteTextures"},
        {(void**)&lotus_graphics_api->GL_API.generate_mipmap, "glGenerateMipmap"},
        {(void**)&lotus_graphics_api->GL_API.generate_texture_mipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.gen_framebuffers, "glGenFramebuffers"},
        {(void**)&lotus_graphics_api->GL_API.bind_framebuffer, "glBindFramebuffer"},
        {(void**)&lotus_graphics_api->GL_API.framebuffer_texture2d, "glFramebufferTexture2D"},
        {(void**)&lotus_graphics_api->GL_API.delete_framebuffers, "glDeleteFramebuffers"},
        {(void**)&lotus_graphics_api->GL_API.check_framebuffer_status, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.clear, "glClear"},
        {(void**)&lotus_graphics_api->GL_API.clear_color, "glClearColor"},
        {(void**)&lotus_graphics_api->GL_API.draw_arrays, "glDrawArrays"},
        {(void**)&lotus_graphics_api->GL_API.draw_elements, "glDrawElements"},
        {(void**)&lotus_graphics_api->GL_API.draw_elements_base_vertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.enable, "glEnable"},
        {(void**)&lotus_graphics_api->GL_API.disable, "glDisable"},
        {(void**)&lotus_graphics_api->GL_API.blend_func, "glBlendFunc"},
        {(void**)&lotus_graphics_api->GL_API.cull_face, "glCullFace"},
        {(void**)&lotus_graphics_api->GL_API.depth_func, "glDepthFunc"},
        {(void**)&lotus_graphics_api->GL_API.viewport, "glViewport"},
        {(void**)&lotus_graphics_api->GL_API.polygon_mode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&lotus_graphics_api->GL_API.get_error, "glGetError"},
        {(void**)&lotus_graphics_api->GL_API.get_string, "glGetString"}
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = _get_gl_fn(functions[i].name);
        if (!*functions[i].function) {
            printf("Failed to load GL function: %s\n", functions[i].name);
        }
    }
}
#endif

ubyte lotus_init_graphics(void) {
    lotus_graphics_api = (Lotus_Graphics_API*)lotus_memory_api->alloc(sizeof(Lotus_Graphics_API), 16);
    if (!lotus_graphics_api) return LOTUS_FALSE;    // error: out of memory!
    
    _load_gl_functions();

    lotus_graphics_api->get_state = _graphics_get_state_impl;
    lotus_graphics_api->cleanup = _graphics_cleanup_impl;
    
    lotus_graphics_api->create_vertex_data = _graphics_make_vertex_data_impl;
    lotus_graphics_api->destroy_vertex_data = _graphics_destroy_vertex_data_impl;

    lotus_graphics_api->create_shader = _graphics_create_shader_impl;
    lotus_graphics_api->destroy_shader = _graphics_destroy_shader_impl;
    
    lotus_graphics_api->set_uniform = _graphics_set_uniform_impl;
    lotus_graphics_api->get_uniform = _graphics_get_uniform_impl;
    lotus_graphics_api->send_uniform = _graphics_send_uniform_impl;

    lotus_graphics_api->create_texture2D = _create_texture2D_impl;
    lotus_graphics_api->destroy_texture2D = _destroy_texture2D_impl;

    lotus_graphics_api->set_color = _graphics_set_color_impl;
    lotus_graphics_api->set_mode = _graphics_set_mode_impl;
    lotus_graphics_api->set_shader = _graphics_set_shader_impl;
    
    lotus_graphics_api->draw_begin = _graphics_draw_begin_impl;
    lotus_graphics_api->draw_clear = _graphics_draw_clear_impl;
    lotus_graphics_api->draw_data = _graphics_draw_data_impl;

    lotus_graphics_api->wireframe_mode = _graphics_wireframe_mode_impl;

    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = LOTUS_TRIANGLE_MODE;
    internal_graphics_state.shader = NULL;
    internal_graphics_state.projection = lotus_identity();
    internal_graphics_state.clearColor = LOTUS_COLOR4(133, 161, 172, 255);

    // OpenGL State Config
    lotus_graphics_api->GL_API.enable(GL_DEPTH_TEST);
    lotus_graphics_api->GL_API.enable(GL_BLEND);
    lotus_graphics_api->GL_API.blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return LOTUS_TRUE;
}

void lotus_shutdown_graphics(void) {
    lotus_graphics_api->cleanup();
    lotus_memory_api->free(lotus_graphics_api);
}
