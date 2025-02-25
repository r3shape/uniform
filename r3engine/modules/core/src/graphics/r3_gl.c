#include "../../include/graphics/r3_gl.h"
#include "../../include/memory/r3_memory.h"

#include <stdio.h>

R3_Graphics_API *r3_graphics_api = NULL;
static R3_Graphics_State internal_graphics_state = {0};

/* GRAPHICS API IMPLEMENTATION */
R3_Graphics_State* _get_state_impl(void) { return &internal_graphics_state; }

void _cleanup_impl(void) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = R3_DRAW_MODES;
    internal_graphics_state.shader = NULL;
    internal_graphics_state.projection = r3_identity();
    internal_graphics_state.clearColor = R3_COLOR4(0, 0, 0, 0);

    r3_graphics_api->GL_API.disable(GL_DEPTH_TEST);
    r3_graphics_api->GL_API.disable(GL_BLEND);
}


R3_Vertex_Data _make_vertex_data_impl(f32 *vertices, ubyte4 vertexCount, ubyte4 *indices, ubyte4 indexCount, ubyte attrs) {
    if ((attrs & ~((1 << R3_VERTEX_ATTRIBS) - 1)) != 0 || !vertices) {
        return (R3_Vertex_Data){0};
    }

    R3_Vertex_Data vertexData;

    // calculate stride and offsets dynamically
    ubyte4 stride = 0;
    ubyte4 offsets[R3_VERTEX_ATTRIBS] = {0};
    const ubyte4 attr_sizes[R3_VERTEX_ATTRIBS] = {
        3, // R3_LOCATION_ATTR:  vec3
        3, // R3_COLOR_ATTR:     vec3
        2, // R3_TCOORD_ATTR:    vec2
        3  // R3_NORMAL_ATTR:    vec3
    };

    for (int i = 0; i < R3_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            // accumulate stride for enabled vertex attributes
            offsets[i] = stride;
            stride += attr_sizes[i];
        }
    }

    r3_graphics_api->GL_API.gen_vertex_arrays(1, &vertexData.vao);
    r3_graphics_api->GL_API.gen_buffers(1, &vertexData.vbo);

    r3_graphics_api->GL_API.bind_vertex_array(vertexData.vao);
    r3_graphics_api->GL_API.bind_buffer(R3_ARRAY_BUFFER, vertexData.vbo);
    
    size_t vertex_data_size = vertexCount * (stride * sizeof(f32));
    r3_graphics_api->GL_API.buffer_data(R3_ARRAY_BUFFER, vertex_data_size, vertices, R3_STATIC_DRAW);

    // generate EBO if indices are provided
    if (indexCount > 0 && indices) {
        r3_graphics_api->GL_API.gen_buffers(1, &vertexData.ebo);

        r3_graphics_api->GL_API.bind_buffer(R3_ELEMENT_ARRAY_BUFFER, vertexData.ebo);
        
        size_t index_data_size = indexCount * sizeof(ubyte4);
        r3_graphics_api->GL_API.buffer_data(R3_ELEMENT_ARRAY_BUFFER, index_data_size, indices, R3_STATIC_DRAW);
        
        vertexData.indices = indices;
        vertexData.indexCount = indexCount;
    } else {
        vertexData.ebo = 0;
        vertexData.indices = NULL;
        vertexData.indexCount = 0;
    }

    // configure vertex attributes dynamically
    for (int i = 0; i < R3_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            r3_graphics_api->GL_API.vertex_attrib_pointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            r3_graphics_api->GL_API.enable_vertex_attrib_array(i);
        }
    }

    r3_graphics_api->GL_API.bind_buffer(R3_ARRAY_BUFFER, 0);
    r3_graphics_api->GL_API.bind_vertex_array(0);

    vertexData.attrs = attrs;
    vertexData.vertices = vertices;
    vertexData.vertexCount = vertexCount;

    return vertexData;
}

void _destroy_vertex_data_impl(R3_Vertex_Data *vertexData) {
    vertexData->attrs = 0;
    
    r3_graphics_api->GL_API.delete_buffers(1, &vertexData->vbo);
    vertexData->vbo = 0;
    
    r3_graphics_api->GL_API.delete_buffers(1, &vertexData->ebo);
    vertexData->ebo = 0;
    
    r3_graphics_api->GL_API.delete_vertex_arrays(1, &vertexData->vao);
    vertexData->vao = 0;
    
    vertexData->vertices = NULL;
    vertexData->vertexCount = 0;
    vertexData->indices = NULL;
    vertexData->indexCount = 0;
}


R3_Shader _create_shader_impl(const char *vertex_shader, const char *fragment_shader) {
    ubyte4 link = 0;
    ubyte4 compile = 0;

    R3_Shader program = {.uniforms=r3_create_hashmap(16)};
    if (!program.uniforms) {
        // error: failed to allocate uniform hashmap!
        return (R3_Shader){0};
    }

    program.program = r3_graphics_api->GL_API.create_program();
    sbyte4 v_Shader = r3_graphics_api->GL_API.create_shader(R3_VERTEX_SHADER);
    sbyte4 f_Shader = r3_graphics_api->GL_API.create_shader(R3_FRAGMENT_SHADER);

    r3_graphics_api->GL_API.shader_source(v_Shader, 1, &vertex_shader, NULL);
    r3_graphics_api->GL_API.compile_shader(v_Shader);
    r3_graphics_api->GL_API.get_shaderiv(v_Shader, R3_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile vertex-shader!
        r3_destroy_hashmap(program.uniforms);
        return (R3_Shader){0};
    }
    
    r3_graphics_api->GL_API.shader_source(f_Shader, 1, &fragment_shader, NULL);
    r3_graphics_api->GL_API.compile_shader(f_Shader);
    r3_graphics_api->GL_API.get_shaderiv(f_Shader, R3_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile fragment-shader!
        r3_destroy_hashmap(program.uniforms);
        return (R3_Shader){0};
    }

    r3_graphics_api->GL_API.attach_shader(program.program, v_Shader);
    r3_graphics_api->GL_API.attach_shader(program.program, f_Shader);
    r3_graphics_api->GL_API.link_program(program.program);
    r3_graphics_api->GL_API.get_programiv(program.program, R3_LINK_STATUS, &link);
    if (!link) {
        // error: failed to link shader program!
        r3_destroy_hashmap(program.uniforms);
        return (R3_Shader){0};
    }

    r3_graphics_api->GL_API.delete_shader(v_Shader);
    r3_graphics_api->GL_API.delete_shader(f_Shader);
    return program;

}

void _destroy_shader_impl(R3_Shader *shader) {
    r3_destroy_hashmap(shader->uniforms);
    r3_graphics_api->GL_API.delete_program(shader->program);
}


ubyte _set_uniform_impl(R3_Shader *shader, const char *name, void *value) {
    if (!shader || !name) return R3_FALSE;   // error: value error!
    return r3_set_hashmap(shader->uniforms, name, value);
}

R3_Uniform _get_uniform_impl(R3_Shader *shader, const char *name) {
    ubyte4 location = r3_graphics_api->GL_API.get_uniform_location(shader->program, name);
    if (location < 0) {
        return (R3_Uniform){ .location = 0, .value = NULL, .name = NULL };
    }
    
    void *value = r3_get_hashmap(shader->uniforms, name);
    if (!value) {
        return (R3_Uniform){ .location = 0, .value = NULL, .name = NULL };
    }
    
    return (R3_Uniform){.location = location, .value = value, .name = name};
}

void _send_uniform_impl(R3_Shader *shader, R3_Uniform_Type type, const char *name) {
    R3_Uniform uniform = r3_graphics_api->get_uniform(shader, name);
    if (uniform.location < 0 || !uniform.value) {
        return;
    }
    
    r3_graphics_api->GL_API.use_program(shader->program);
    switch (type) {
        case R3_UNIFORM_NONE: break;
        case R3_UNIFORM_TYPES: break;
        case R3_UNIFORM_VEC2: r3_graphics_api->GL_API.uniform2fv(uniform.location, 1, uniform.value); break;
        case R3_UNIFORM_VEC3: r3_graphics_api->GL_API.uniform3fv(uniform.location, 1, uniform.value); break;
        case R3_UNIFORM_VEC4: r3_graphics_api->GL_API.uniform4fv(uniform.location, 1, uniform.value); break;
        case R3_UNIFORM_MAT4: r3_graphics_api->GL_API.uniform_matrix4fv(uniform.location, 1, 0, uniform.value); break;
        default: break;
    }
}

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/graphics/stb_image.h"

R3_Texture2D _create_texture2D_impl(char* path, R3_Texture_Format format) {
    R3_Texture2D texture = { .id = 0, .width = 0, .height = 0, .channels = 0, .path = NULL, .raw = NULL };
    if (!path) return texture;

    texture.path = path;
    stbi_set_flip_vertically_on_load(R3_TRUE);
    texture.raw = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
    if (!texture.raw) return texture;    // error: failed to allocate raw data buffer!

    r3_graphics_api->GL_API.gen_textures(1, &texture.id);
    r3_graphics_api->GL_API.bind_texture(GL_TEXTURE_2D, texture.id);

    // set texture wrapping options
    r3_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x axis
    r3_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y axis

    // set texture filtering options (scaling up/down)
    r3_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    r3_graphics_api->GL_API.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generate the texture
    r3_graphics_api->GL_API.tex_image2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.raw);
    r3_graphics_api->GL_API.generate_mipmap(GL_TEXTURE_2D);

    return texture;
}

void _destroy_texture2D_impl(R3_Texture2D* texture) {
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;
    texture ->path = NULL;
    r3_graphics_api->GL_API.delete_textures(1, &texture->id);
    stbi_image_free(texture->raw);
    texture->id = 0;
}


R3_Material _create_material_impl(R3_Shader* shader) {
    R3_Material mat = {NULL};
    if (!shader || !shader->program) return mat;

    mat.uniforms = r3_create_hashmap(16);
    if (!mat.uniforms) return mat;  // error: failed to create material uniform hashmap!

    mat.shader = shader;
    return mat;
}

void _destroy_material_impl(R3_Material* material) {
    r3_destroy_hashmap(material->uniforms);
    material->uniforms = NULL;
    material->shader = NULL;
}

ubyte _set_material_uniform_impl(R3_Material* material, const char *name, void *value) {
    if (!material || !material->shader || !material->shader->program) return R3_FALSE;
    return r3_graphics_api->set_uniform(material->shader, name, value);
}

R3_Uniform _get_material_uniform_impl(R3_Material* material, const char *name) {
    if (!material || !material->shader || !material->shader->program) return (R3_Uniform){ .location = 0, .value = NULL, .name = NULL };
    return r3_graphics_api->get_uniform(material->shader, name);
}

void _send_material_uniform_impl(R3_Material* material, R3_Uniform_Type type, const char *name) {
    if (!material || !material->shader || !material->shader->program) return;
    r3_graphics_api->send_uniform(material->shader, type, name);
}


void _set_color_impl(R3_Vec4 color4) {
    internal_graphics_state.clearColor = color4;
}

void _set_mode_impl(R3_Draw_Mode mode) {
    if (internal_graphics_state.mode != mode) {
        internal_graphics_state.mode = mode;
    }
}

void _set_shader_impl(R3_Shader *shader) {
    internal_graphics_state.shader = (shader->program == 0) ? NULL : shader;
    r3_graphics_api->set_uniform(internal_graphics_state.shader, "u_projection", &internal_graphics_state.projection);
}


void _draw_begin_impl(R3_Draw_Mode mode, R3_Vec4 color4, R3_Mat4 projection) {
    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = mode;
    internal_graphics_state.projection = projection;
    internal_graphics_state.clearColor = color4;
}

void _draw_data_impl(R3_Vertex_Data vertexData) {
    if (vertexData.vertexCount <= 0 || !vertexData.vertices || vertexData.vao < 0) {
        return; // error: invalid vertex data for draw call
    };

    // error: cannot draw without an active shader!
    if (!internal_graphics_state.shader) return;

    if (internal_graphics_state.shader->program > 0) {
        r3_graphics_api->GL_API.use_program(internal_graphics_state.shader->program);
        r3_graphics_api->send_uniform(internal_graphics_state.shader, R3_UNIFORM_MAT4, "u_model");
        r3_graphics_api->send_uniform(internal_graphics_state.shader, R3_UNIFORM_MAT4, "u_view");
        r3_graphics_api->send_uniform(internal_graphics_state.shader, R3_UNIFORM_MAT4, "u_projection");
    }

    r3_graphics_api->GL_API.bind_vertex_array(vertexData.vao);
    if (vertexData.indices && vertexData.indexCount > 0) r3_graphics_api->GL_API.draw_elements(internal_graphics_state.mode, vertexData.indexCount, GL_UNSIGNED_INT, NULL);
    else r3_graphics_api->GL_API.draw_arrays(internal_graphics_state.mode, 0, vertexData.vertexCount);

    r3_graphics_api->GL_API.bind_vertex_array(0);
}

void _draw_clear_impl(void) {
    r3_graphics_api->GL_API.clear_color(
        internal_graphics_state.clearColor.x,
        internal_graphics_state.clearColor.y,
        internal_graphics_state.clearColor.z,
        internal_graphics_state.clearColor.w
    ); r3_graphics_api->GL_API.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/* Graphics State Setters/Toggles */
void _toggle_wireframe_impl(ubyte toggle) {
    if (toggle) {
        r3_graphics_api->GL_API.polygon_mode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        r3_graphics_api->GL_API.polygon_mode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

#if defined(R3_PLATFORM_WINDOWS)
void _toggle_vsync_impl(ubyte toggle) {
    typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALPROC)(int);
    PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

    const ubyte* extensions = r3_graphics_api->GL_API.get_string(GL_EXTENSIONS);
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

    if (wglSwapIntervalEXT) wglSwapIntervalEXT(toggle);
}
#endif

/* GRAPHICS API IMPLEMENTATION */

#if defined(R3_PLATFORM_WINDOWS)
#include <Windows.h>
#include <windowsx.h>
static void *_get_gl_fn(const char *name) {
    void *proc = (void*)wglGetProcAddress(name);
    if (proc == NULL || (proc == (void*)0x1) || (proc == (void*)0x2) || (proc == (void*)0x3) || (proc == (void*)-1)) {
        proc = (void*)GetProcAddress(LoadLibrary("opengl32.dll"), name);
    }; return proc;
}
#endif

void _load_gl_functions(void) {
    struct gl_func {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.gen_buffers, "glGenBuffers"},
        {(void**)&r3_graphics_api->GL_API.bind_buffer, "glBindBuffer"},
        {(void**)&r3_graphics_api->GL_API.buffer_data, "glBufferData"},
        {(void**)&r3_graphics_api->GL_API.map_buffer, "glMapBuffer"},
        {(void**)&r3_graphics_api->GL_API.unmap_buffer, "glUnmapBuffer"},
        {(void**)&r3_graphics_api->GL_API.buffer_sub_data, "glBufferSubData"},
        {(void**)&r3_graphics_api->GL_API.get_buffer_parameteriv, "glGetBufferParameteriv"},
        {(void**)&r3_graphics_api->GL_API.delete_buffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {(void**)&r3_graphics_api->GL_API.disable_vertex_attrib_array, "glDisableVertexAttribArray"},
        {(void**)&r3_graphics_api->GL_API.vertex_attrib_pointer, "glVertexAttribPointer"},
        {(void**)&r3_graphics_api->GL_API.bind_vertex_array, "glBindVertexArray"},
        {(void**)&r3_graphics_api->GL_API.gen_vertex_arrays, "glGenVertexArrays"},
        {(void**)&r3_graphics_api->GL_API.delete_vertex_arrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.create_program, "glCreateProgram"},
        {(void**)&r3_graphics_api->GL_API.use_program, "glUseProgram"},
        {(void**)&r3_graphics_api->GL_API.link_program, "glLinkProgram"},
        {(void**)&r3_graphics_api->GL_API.delete_program, "glDeleteProgram"},
        {(void**)&r3_graphics_api->GL_API.create_shader, "glCreateShader"},
        {(void**)&r3_graphics_api->GL_API.compile_shader, "glCompileShader"},
        {(void**)&r3_graphics_api->GL_API.attach_shader, "glAttachShader"},
        {(void**)&r3_graphics_api->GL_API.detach_shader, "glDetachShader"},
        {(void**)&r3_graphics_api->GL_API.delete_shader, "glDeleteShader"},
        {(void**)&r3_graphics_api->GL_API.shader_source, "glShaderSource"},
        {(void**)&r3_graphics_api->GL_API.get_shaderiv, "glGetShaderiv"},
        {(void**)&r3_graphics_api->GL_API.get_shader_info_log, "glGetShaderInfoLog"},
        {(void**)&r3_graphics_api->GL_API.get_programiv, "glGetProgramiv"},
        {(void**)&r3_graphics_api->GL_API.get_program_info_log, "glGetProgramInfoLog"},
        {(void**)&r3_graphics_api->GL_API.get_uniform_location, "glGetUniformLocation"},
        {(void**)&r3_graphics_api->GL_API.uniform2fv, "glUniform2fv"},
        {(void**)&r3_graphics_api->GL_API.uniform3fv, "glUniform3fv"},
        {(void**)&r3_graphics_api->GL_API.uniform4fv, "glUniform4fv"},
        {(void**)&r3_graphics_api->GL_API.uniform_matrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.gen_textures, "glGenTextures"},
        {(void**)&r3_graphics_api->GL_API.bind_texture, "glBindTexture"},
        {(void**)&r3_graphics_api->GL_API.tex_parameteri, "glTexParameteri"},
        {(void**)&r3_graphics_api->GL_API.tex_image2D, "glTexImage2D"},
        {(void**)&r3_graphics_api->GL_API.active_texture, "glActiveTexture"},
        {(void**)&r3_graphics_api->GL_API.delete_textures, "glDeleteTextures"},
        {(void**)&r3_graphics_api->GL_API.generate_mipmap, "glGenerateMipmap"},
        {(void**)&r3_graphics_api->GL_API.generate_texture_mipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.gen_framebuffers, "glGenFramebuffers"},
        {(void**)&r3_graphics_api->GL_API.bind_framebuffer, "glBindFramebuffer"},
        {(void**)&r3_graphics_api->GL_API.framebuffer_texture2d, "glFramebufferTexture2D"},
        {(void**)&r3_graphics_api->GL_API.delete_framebuffers, "glDeleteFramebuffers"},
        {(void**)&r3_graphics_api->GL_API.check_framebuffer_status, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.clear, "glClear"},
        {(void**)&r3_graphics_api->GL_API.clear_color, "glClearColor"},
        {(void**)&r3_graphics_api->GL_API.draw_arrays, "glDrawArrays"},
        {(void**)&r3_graphics_api->GL_API.draw_elements, "glDrawElements"},
        {(void**)&r3_graphics_api->GL_API.draw_elements_base_vertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.enable, "glEnable"},
        {(void**)&r3_graphics_api->GL_API.disable, "glDisable"},
        {(void**)&r3_graphics_api->GL_API.blend_func, "glBlendFunc"},
        {(void**)&r3_graphics_api->GL_API.cull_face, "glCullFace"},
        {(void**)&r3_graphics_api->GL_API.depth_func, "glDepthFunc"},
        {(void**)&r3_graphics_api->GL_API.viewport, "glViewport"},
        {(void**)&r3_graphics_api->GL_API.polygon_mode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&r3_graphics_api->GL_API.get_error, "glGetError"},
        {(void**)&r3_graphics_api->GL_API.get_string, "glGetString"}
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = _get_gl_fn(functions[i].name);
        if (!*functions[i].function) {
            printf("Failed to load GL function: %s\n", functions[i].name);
        }
    }
}

ubyte r3_init_graphics(void) {
    r3_graphics_api = (R3_Graphics_API*)r3_memory_api->alloc(sizeof(R3_Graphics_API), 16);
    if (!r3_graphics_api) return R3_FALSE;    // error: out of memory!
    
    _load_gl_functions();

    r3_graphics_api->get_state = _get_state_impl;
    r3_graphics_api->cleanup = _cleanup_impl;
    
    r3_graphics_api->create_vertex_data = _make_vertex_data_impl;
    r3_graphics_api->destroy_vertex_data = _destroy_vertex_data_impl;

    r3_graphics_api->create_shader = _create_shader_impl;
    r3_graphics_api->destroy_shader = _destroy_shader_impl;
    
    r3_graphics_api->set_uniform = _set_uniform_impl;
    r3_graphics_api->get_uniform = _get_uniform_impl;
    r3_graphics_api->send_uniform = _send_uniform_impl;

    r3_graphics_api->create_texture2D = _create_texture2D_impl;
    r3_graphics_api->destroy_texture2D = _destroy_texture2D_impl;
    
    r3_graphics_api->create_material = _create_material_impl;
    r3_graphics_api->destroy_material = _destroy_material_impl;
    r3_graphics_api->set_material_uniform = _set_material_uniform_impl;
    r3_graphics_api->get_material_uniform = _get_material_uniform_impl;
    r3_graphics_api->send_material_uniform = _send_material_uniform_impl;

    r3_graphics_api->set_color = _set_color_impl;
    r3_graphics_api->set_mode = _set_mode_impl;
    r3_graphics_api->set_shader = _set_shader_impl;
    
    r3_graphics_api->draw_begin = _draw_begin_impl;
    r3_graphics_api->draw_clear = _draw_clear_impl;
    r3_graphics_api->draw_data = _draw_data_impl;

    r3_graphics_api->toggle_vsync = _toggle_vsync_impl;
    r3_graphics_api->toggle_wireframe = _toggle_wireframe_impl;

    internal_graphics_state.draws = 0;
    internal_graphics_state.mode = R3_TRIANGLE_MODE;
    internal_graphics_state.shader = NULL;
    internal_graphics_state.projection = r3_identity();
    internal_graphics_state.clearColor = R3_COLOR4(133, 161, 172, 255);

    // Initial Graphics State Config
    r3_graphics_api->toggle_vsync(R3_TRUE);
    
    r3_graphics_api->GL_API.enable(GL_DEPTH_TEST);
    r3_graphics_api->GL_API.enable(GL_BLEND);
    r3_graphics_api->GL_API.blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return R3_TRUE;
}

void r3_shutdown_graphics(void) {
    r3_graphics_api->cleanup();
    r3_memory_api->free(r3_graphics_api);
}
