#include "../include/r3.graphics.h"

_r3_graphics_api* internal_api_ptr = NULL;

// GENERAL GRAPHICS API
R3_Shader _create_shader_impl(cstr vertex, cstr fragment) {
    u32 link = 0;
    u32 compile = 0;

    R3_Shader shader = {.uniforms=structx->create_hash_array(16)};
    if (!shader.uniforms) {
        // error: failed to allocate uniform hashmap!
        return (R3_Shader){0};
    }

    shader.program = internal_api_ptr->gl.create_program();
    i32 v_Shader = internal_api_ptr->gl.create_shader(R3_VERTEX_SHADER);
    i32 f_Shader = internal_api_ptr->gl.create_shader(R3_FRAGMENT_SHADER);

    internal_api_ptr->gl.shader_source(v_Shader, 1, &vertex, NULL);
    internal_api_ptr->gl.compile_shader(v_Shader);
    internal_api_ptr->gl.get_shaderiv(v_Shader, R3_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile vertex-shader!
        printf("failed to compile vertex shader!\n");
        structx->destroy_hash_array(shader.uniforms);
        return (R3_Shader){0};
    }
    
    internal_api_ptr->gl.shader_source(f_Shader, 1, &fragment, NULL);
    internal_api_ptr->gl.compile_shader(f_Shader);
    internal_api_ptr->gl.get_shaderiv(f_Shader, R3_COMPILE_STATUS, &compile);
    if (!compile) {
        // error: failed to compile fragment-shader!
        printf("failed to compile fragment shader!\n");
        structx->destroy_hash_array(shader.uniforms);
        return (R3_Shader){0};
    }
    
    internal_api_ptr->gl.attach_shader(shader.program, v_Shader);
    internal_api_ptr->gl.attach_shader(shader.program, f_Shader);
    internal_api_ptr->gl.link_program(shader.program);
    internal_api_ptr->gl.get_programiv(shader.program, R3_LINK_STATUS, &link);
    if (!link) {
        // error: failed to link shader program!
        printf("failed to link shader!\n");
        structx->destroy_hash_array(shader.uniforms);
        return (R3_Shader){0};
    }

    internal_api_ptr->gl.delete_shader(v_Shader);
    internal_api_ptr->gl.delete_shader(f_Shader);
    return shader;
}

void _destroy_shader_impl(R3_Shader* shader) {
    structx->destroy_hash_array(shader->uniforms);
    internal_api_ptr->gl.delete_program(shader->program);
}

u8 _set_uniform_impl(R3_Shader* shader, str name, void* value) {
    if (!shader || !name || !value) return LIBX_FALSE;  // error: null ptr!
    return structx->put_hash_array(shader->uniforms, name, value);
}

u8 _send_uniform_impl(R3_Shader* shader, R3_Uniform_Type type, str name) {
    if (!shader || !name) return LIBX_FALSE;    // error: null ptr!
    
    void* value = structx->get_hash_array(shader->uniforms, name);
    if (!value) return LIBX_FALSE;  // error: failed to find uniform in hashmap!

    u32 location = internal_api_ptr->gl.get_uniform_location(shader->program, (cstr)name);

    internal_api_ptr->gl.use_program(shader->program);
    switch (type) {
        case R3_UNIFORM_NONE: break;
        case R3_UNIFORM_TYPES: break;
        case R3_UNIFORM_FLOAT: internal_api_ptr->gl.uniform1f(location, value); break;
        case R3_UNIFORM_VEC2: internal_api_ptr->gl.uniform2fv(location, 1, value); break;
        case R3_UNIFORM_VEC3: internal_api_ptr->gl.uniform3fv(location, 1, value); break;
        case R3_UNIFORM_VEC4: internal_api_ptr->gl.uniform4fv(location, 1, value); break;
        case R3_UNIFORM_MAT4: internal_api_ptr->gl.uniform_matrix4fv(location, 1, 1, value); break; // libx matrices are row-major
        default: break;
    }
    return LIBX_TRUE;
}


R3_Vertex_Data _create_vertex_data_impl(f32 *vertices, u32 vertexCount, u32 *indices, u32 indexCount, u8 attrs) {
    if (!internal_api_ptr || !internal_api_ptr->gl.init) return (R3_Vertex_Data){0}; // error: null ptr!

    if ((attrs & ~((1 << R3_VERTEX_ATTRIBS) - 1)) != 0 || !vertices) {
        return (R3_Vertex_Data){0};
    }

    R3_Vertex_Data vertexData;

    // calculate stride and offsets dynamically
    u32 stride = 0;
    u32 offsets[R3_VERTEX_ATTRIBS] = {0};
    const u32 attr_sizes[R3_VERTEX_ATTRIBS] = {
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

    internal_api_ptr->gl.gen_vertex_arrays(1, &vertexData.vao);
    internal_api_ptr->gl.gen_buffers(1, &vertexData.vbo);

    internal_api_ptr->gl.bind_vertex_array(vertexData.vao);
    internal_api_ptr->gl.bind_buffer(R3_ARRAY_BUFFER, vertexData.vbo);
    
    size_t vertex_data_size = vertexCount * (stride * sizeof(f32));
    internal_api_ptr->gl.buffer_data(R3_ARRAY_BUFFER, vertex_data_size, vertices, R3_STATIC_DRAW);

    // generate EBO if indices are provided
    if (indexCount > 0 && indices) {
        internal_api_ptr->gl.gen_buffers(1, &vertexData.ebo);

        internal_api_ptr->gl.bind_buffer(R3_ELEMENT_ARRAY_BUFFER, vertexData.ebo);
        
        size_t index_data_size = indexCount * sizeof(u32);
        internal_api_ptr->gl.buffer_data(R3_ELEMENT_ARRAY_BUFFER, index_data_size, indices, R3_STATIC_DRAW);
        
        vertexData.indices = indices;
        vertexData.indexCount = indexCount;
    } else {
        vertexData.ebo = 0;
        vertexData.indices = NULL;
        vertexData.indexCount = 0;
    }

    // configure vertex attributes
    for (int i = 0; i < R3_VERTEX_ATTRIBS; i++) {
        if ((attrs & (1 << i)) != 0) {
            internal_api_ptr->gl.vertex_attrib_pointer(
                i, 
                attr_sizes[i], 
                GL_FLOAT, 
                GL_FALSE, 
                stride * sizeof(f32), 
                (void*)(offsets[i] * sizeof(f32))
            );
            internal_api_ptr->gl.enable_vertex_attrib_array(i);
        }
    }

    internal_api_ptr->gl.bind_buffer(R3_ARRAY_BUFFER, 0);
    internal_api_ptr->gl.bind_vertex_array(0);

    vertexData.attrs = attrs;
    vertexData.vertices = vertices;
    vertexData.vertexCount = vertexCount;

    return vertexData;
}

void _destroy_vertex_data_impl(R3_Vertex_Data *vertexData) {
    if (!internal_api_ptr || !internal_api_ptr->gl.init) return;  // error: null ptr!

    vertexData->attrs = 0;
    
    internal_api_ptr->gl.delete_buffers(1, &vertexData->vbo);
    vertexData->vbo = 0;
    
    internal_api_ptr->gl.delete_buffers(1, &vertexData->ebo);
    vertexData->ebo = 0;
    
    internal_api_ptr->gl.delete_vertex_arrays(1, &vertexData->vao);
    vertexData->vao = 0;
    
    vertexData->vertices = NULL;
    vertexData->vertexCount = 0;
    vertexData->indices = NULL;
    vertexData->indexCount = 0;
}


#define STB_IMAGE_IMPLEMENTATION
#include "../../../external/stb/include/stb_image.h"

R3_Texture _create_texture2D_impl(char* path, R3_Texture_Format format) {
R3_Texture texture = { .id = 0, .width = 0, .height = 0, .channels = 0, .path = NULL, .raw = NULL };
    if (!path) return texture;

    texture.path = path;
    stbi_set_flip_vertically_on_load(LIBX_TRUE);
    texture.raw = stbi_load(path, &texture.width, &texture.height, &texture.channels, 0);
    if (!texture.raw) return texture;    // error: failed to allocate raw data buffer!

    internal_api_ptr->gl.gen_textures(1, &texture.id);
    internal_api_ptr->gl.bind_texture(GL_TEXTURE_2D, texture.id);

    // set texture wrapping options
    internal_api_ptr->gl.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // x axis
    internal_api_ptr->gl.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // y axis

    // set texture filtering options (scaling up/down)
    internal_api_ptr->gl.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    internal_api_ptr->gl.tex_parameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // generate the texture
    internal_api_ptr->gl.tex_image2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.raw);
    internal_api_ptr->gl.generate_mipmap(GL_TEXTURE_2D);

    return texture;
}

void _destroy_texture2D_impl(R3_Texture* texture) {
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;
    texture ->path = NULL;
    internal_api_ptr->gl.delete_textures(1, &texture->id);
    stbi_image_free(texture->raw);
    texture->id = 0;
}


// PIPELINE API
void _push_pipeline_impl(R3_Vertex_Data* vertex, Mat4* model, R3_Shader* shader, R3_Texture* texture, R3_Render_Mode mode, R3_Render_Call_Type type) {
    if (!internal_api_ptr->pipeline.calls) return;  // error: render pipeline not initialized!
    structx->push_array(internal_api_ptr->pipeline.calls, &(R3_Render_Call){
        .vertex = vertex,
        .texture = texture,
        .shader = shader,
        .model = model,
        .mode = mode,
        .type = type
    });
}

void _flush_pipeline_impl(void) {
    if (!internal_api_ptr->pipeline.calls) return;  // error: render pipeline not initialized!

    internal_api_ptr->gl.clear_color(
        internal_api_ptr->pipeline.clear_color.x,
        internal_api_ptr->pipeline.clear_color.y,
        internal_api_ptr->pipeline.clear_color.z,
        internal_api_ptr->pipeline.clear_color.w
    ); internal_api_ptr->gl.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Array_Head head = structx->get_array_head(internal_api_ptr->pipeline.calls);
    R3_Shader* shader = internal_api_ptr->pipeline.shader;
    LIBX_FORI(0, head.count, 1) {
        R3_Render_Call call;
        structx->pull_array(internal_api_ptr->pipeline.calls, 0, &call);
        
        if (call.shader && shader->program != call.shader->program) {
            shader = call.shader;
            internal_api_ptr->set_uniform(shader, "u_proj", &internal_api_ptr->pipeline.proj.m);
            internal_api_ptr->send_uniform(shader, R3_UNIFORM_MAT4, "u_proj");
            if (internal_api_ptr->camera.init) {
                internal_api_ptr->set_uniform(shader, "u_view", &internal_api_ptr->camera.view.m);
                internal_api_ptr->send_uniform(shader, R3_UNIFORM_MAT4, "u_view");
            }
        } else {
            internal_api_ptr->send_uniform(shader, R3_UNIFORM_MAT4, "u_proj");
            internal_api_ptr->send_uniform(shader, R3_UNIFORM_MAT4, "u_view");
        }
        
        if (call.model) {
            internal_api_ptr->set_uniform(shader, "u_model", &call.model->m);
            internal_api_ptr->send_uniform(shader, R3_UNIFORM_MAT4, "u_model");
        }
        
        if (internal_api_ptr->pipeline.mode != call.mode) internal_api_ptr->pipeline.mode = call.mode;
        
        internal_api_ptr->gl.use_program(shader->program);
        if (call.texture->id > 0) internal_api_ptr->gl.bind_texture(GL_TEXTURE_2D, call.texture->id);
        internal_api_ptr->gl.bind_vertex_array(call.vertex->vao);
        if (call.type == R3_RENDER_ARRAYS) {
            internal_api_ptr->gl.draw_arrays(call.mode, 0, call.vertex->vertexCount);
        } else if (call.type == R3_RENDER_ELEMENTS) {
            internal_api_ptr->gl.draw_elements(call.mode, call.vertex->indexCount, GL_UNSIGNED_INT, NULL);
        }
    }
}

u8 _init_pipeline_impl(R3_Render_Mode mode, R3_Shader* shader, Mat4 proj) {
    if (internal_api_ptr->pipeline.calls != NULL) {
        printf("render pipeline already initialized.\n");
        return LIBX_TRUE;
    }

    if (mode >= R3_DRAW_MODES || !shader) return LIBX_FALSE;  // error: value error/null ptr!
    
    internal_api_ptr->pipeline.calls = structx->create_array(sizeof(R3_Render_Call), 1024);
    if (!internal_api_ptr->pipeline.calls) return LIBX_FALSE;    // error: out of memory!

    internal_api_ptr->pipeline.mode = mode;
    internal_api_ptr->pipeline.proj = proj;
    internal_api_ptr->pipeline.shader = shader;
    internal_api_ptr->pipeline.clear_color = mathx->vec.vec4(60/255.0, 120/255.0, 210/255.0, 255/255.0);

    internal_api_ptr->set_uniform(internal_api_ptr->pipeline.shader, "u_proj", &internal_api_ptr->pipeline.proj.m);
    internal_api_ptr->pipeline.init = LIBX_TRUE;

    return LIBX_TRUE;
}


// CAMERA API
u8 _init_camera_impl(Vec3 eye, Vec3 center, Vec3 up) {
    if (!internal_api_ptr->pipeline.calls) return LIBX_FALSE;
    
    internal_api_ptr->camera.sensitivity = 0.1;
    internal_api_ptr->camera.speed = 0.1;
    internal_api_ptr->camera.roll = 0.0;
    internal_api_ptr->camera.yaw = -90.0;
    internal_api_ptr->camera.pitch = 0.0;
    internal_api_ptr->camera.eye = eye;
    internal_api_ptr->camera.center = center;
    internal_api_ptr->camera.direction = mathx->vec.norm3(mathx->vec.sub3(eye, center));
    internal_api_ptr->camera.right = mathx->vec.norm3(mathx->vec.cross3(up, internal_api_ptr->camera.direction));
    internal_api_ptr->camera.up = mathx->vec.cross3(internal_api_ptr->camera.direction, internal_api_ptr->camera.right);
    
    // update the view matrix with camera vectors
    internal_api_ptr->camera.view = mathx->mat.lookat(
        internal_api_ptr->camera.eye,
        mathx->vec.add3(internal_api_ptr->camera.eye, internal_api_ptr->camera.direction),
        internal_api_ptr->camera.up
    );
    
    if (internal_api_ptr->pipeline.init) {
        internal_api_ptr->set_uniform(internal_api_ptr->pipeline.shader, "u_view", &internal_api_ptr->camera.view.m);
    }

    internal_api_ptr->camera.init = LIBX_TRUE;
    return LIBX_TRUE;
}

void _rotate_camera_impl( f32 dx, f32 dy) {
    internal_api_ptr->camera.yaw += dx * internal_api_ptr->camera.sensitivity;
    internal_api_ptr->camera.pitch += dy * internal_api_ptr->camera.sensitivity;
    
    internal_api_ptr->camera.yaw = LIBX_WRAPF(internal_api_ptr->camera.yaw, 360.0);
    internal_api_ptr->camera.pitch = LIBX_CLAMP(internal_api_ptr->camera.pitch, -89.0, 89.0);
}

void _translate_camera_impl(i8 x, i8 y, i8 z) {
    if (x > 0) {
        internal_api_ptr->camera.eye = mathx->vec.add3(internal_api_ptr->camera.eye, mathx->vec.scale3(mathx->vec.norm3(mathx->vec.cross3(
            internal_api_ptr->camera.direction,
            internal_api_ptr->camera.up
        )), internal_api_ptr->camera.speed));
    }
    else if (x < 0) {
        internal_api_ptr->camera.eye = mathx->vec.sub3(internal_api_ptr->camera.eye, mathx->vec.scale3(mathx->vec.norm3(mathx->vec.cross3(
            internal_api_ptr->camera.direction,
            internal_api_ptr->camera.up
        )), internal_api_ptr->camera.speed));
    }

    if (y > 0) {
        internal_api_ptr->camera.eye = mathx->vec.add3(
            internal_api_ptr->camera.eye,
            mathx->vec.scale3(internal_api_ptr->camera.up, internal_api_ptr->camera.speed)
        );
    }
    else if (y < 0) {
        internal_api_ptr->camera.eye = mathx->vec.sub3(
            internal_api_ptr->camera.eye,
            mathx->vec.scale3(internal_api_ptr->camera.up, internal_api_ptr->camera.speed)
        );
    }
    
    if (z > 0) {
        internal_api_ptr->camera.eye = mathx->vec.add3(
            internal_api_ptr->camera.eye,
            mathx->vec.scale3(internal_api_ptr->camera.direction, internal_api_ptr->camera.speed)
        );
    }
    else if (z < 0) {
        internal_api_ptr->camera.eye = mathx->vec.sub3(
            internal_api_ptr->camera.eye,
            mathx->vec.scale3(internal_api_ptr->camera.direction, internal_api_ptr->camera.speed)
        );
    }
}

void _update_camera_impl(void) {
    internal_api_ptr->camera.direction = mathx->vec.norm3(mathx->vec.vec3(
        cosf(mathx->scalar.to_radians(internal_api_ptr->camera.yaw)) * cosf(mathx->scalar.to_radians(internal_api_ptr->camera.pitch)),
        sinf(mathx->scalar.to_radians(internal_api_ptr->camera.pitch)),
        sinf(mathx->scalar.to_radians(internal_api_ptr->camera.yaw)) * cosf(mathx->scalar.to_radians(internal_api_ptr->camera.pitch))
    ));
    internal_api_ptr->camera.right = mathx->vec.norm3(mathx->vec.cross3(internal_api_ptr->camera.direction, mathx->vec.vec3(0, 1.0, 0)));
    internal_api_ptr->camera.up = mathx->vec.norm3(mathx->vec.cross3(internal_api_ptr->camera.right, internal_api_ptr->camera.direction));

    internal_api_ptr->camera.view = mathx->mat.lookat(
        internal_api_ptr->camera.eye,
        mathx->vec.add3(internal_api_ptr->camera.eye, internal_api_ptr->camera.direction),
        internal_api_ptr->camera.up
    );
}


// API SETUP & CONFIGURATION
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

u8 _init_gl_impl(_r3_graphics_api* api) {
    if (!api) return LIBX_FALSE;   // error: null ptr!

    struct gl_func {
        void **function;
        const char *name;
    } functions[] = {
        // BUFFER FUNCTIONS
        {(void**)&api->gl.gen_buffers, "glGenBuffers"},
        {(void**)&api->gl.bind_buffer, "glBindBuffer"},
        {(void**)&api->gl.buffer_data, "glBufferData"},
        {(void**)&api->gl.map_buffer, "glMapBuffer"},
        {(void**)&api->gl.unmap_buffer, "glUnmapBuffer"},
        {(void**)&api->gl.buffer_sub_data, "glBufferSubData"},
        {(void**)&api->gl.get_buffer_parameteriv, "glGetBufferParameteriv"},
        {(void**)&api->gl.delete_buffers, "glDeleteBuffers"},

        // VERTEX ATTRIBUTE FUNCTIONS
        {(void**)&api->gl.enable_vertex_attrib_array, "glEnableVertexAttribArray"},
        {(void**)&api->gl.disable_vertex_attrib_array, "glDisableVertexAttribArray"},
        {(void**)&api->gl.vertex_attrib_pointer, "glVertexAttribPointer"},
        {(void**)&api->gl.bind_vertex_array, "glBindVertexArray"},
        {(void**)&api->gl.gen_vertex_arrays, "glGenVertexArrays"},
        {(void**)&api->gl.delete_vertex_arrays, "glDeleteVertexArrays"},

        // SHADER FUNCTIONS
        {(void**)&api->gl.create_program, "glCreateProgram"},
        {(void**)&api->gl.use_program, "glUseProgram"},
        {(void**)&api->gl.link_program, "glLinkProgram"},
        {(void**)&api->gl.delete_program, "glDeleteProgram"},
        {(void**)&api->gl.create_shader, "glCreateShader"},
        {(void**)&api->gl.compile_shader, "glCompileShader"},
        {(void**)&api->gl.attach_shader, "glAttachShader"},
        {(void**)&api->gl.detach_shader, "glDetachShader"},
        {(void**)&api->gl.delete_shader, "glDeleteShader"},
        {(void**)&api->gl.shader_source, "glShaderSource"},
        {(void**)&api->gl.get_shaderiv, "glGetShaderiv"},
        {(void**)&api->gl.get_shader_info_log, "glGetShaderInfoLog"},
        {(void**)&api->gl.get_programiv, "glGetProgramiv"},
        {(void**)&api->gl.get_program_info_log, "glGetProgramInfoLog"},
        {(void**)&api->gl.get_uniform_location, "glGetUniformLocation"},
        
        {(void**)&api->gl.uniform1f, "glUniform1f"},
        {(void**)&api->gl.uniform2fv, "glUniform2fv"},
        {(void**)&api->gl.uniform3fv, "glUniform3fv"},
        {(void**)&api->gl.uniform4fv, "glUniform4fv"},
        {(void**)&api->gl.uniform_matrix4fv, "glUniformMatrix4fv"},

        // TEXTURE FUNCTIONS
        {(void**)&api->gl.gen_textures, "glGenTextures"},
        {(void**)&api->gl.bind_texture, "glBindTexture"},
        {(void**)&api->gl.tex_parameteri, "glTexParameteri"},
        {(void**)&api->gl.tex_image2D, "glTexImage2D"},
        {(void**)&api->gl.active_texture, "glActiveTexture"},
        {(void**)&api->gl.delete_textures, "glDeleteTextures"},
        {(void**)&api->gl.generate_mipmap, "glGenerateMipmap"},
        {(void**)&api->gl.generate_texture_mipmap, "glGenerateTextureMipmap"},

        // FRAMEBUFFER FUNCTIONS
        {(void**)&api->gl.gen_framebuffers, "glGenFramebuffers"},
        {(void**)&api->gl.bind_framebuffer, "glBindFramebuffer"},
        {(void**)&api->gl.framebuffer_texture2d, "glFramebufferTexture2D"},
        {(void**)&api->gl.delete_framebuffers, "glDeleteFramebuffers"},
        {(void**)&api->gl.check_framebuffer_status, "glCheckFramebufferStatus"},

        // DRAWING FUNCTIONS
        {(void**)&api->gl.clear, "glClear"},
        {(void**)&api->gl.clear_color, "glClearColor"},
        {(void**)&api->gl.draw_arrays, "glDrawArrays"},
        {(void**)&api->gl.draw_elements, "glDrawElements"},
        {(void**)&api->gl.draw_elements_base_vertex, "glDrawElementsBaseVertex"},

        // STATE MANAGEMENT FUNCTIONS
        {(void**)&api->gl.enable, "glEnable"},
        {(void**)&api->gl.disable, "glDisable"},
        {(void**)&api->gl.blend_func, "glBlendFunc"},
        {(void**)&api->gl.cull_face, "glCullFace"},
        {(void**)&api->gl.depth_func, "glDepthFunc"},
        {(void**)&api->gl.viewport, "glViewport"},
        {(void**)&api->gl.polygon_mode, "glPolygonMode"},

        // UTILITY FUNCTIONS
        {(void**)&api->gl.get_error, "glGetError"},
        {(void**)&api->gl.get_string, "glGetString"}
    };

    for (size_t i = 0; i < sizeof(functions) / sizeof(functions[0]); ++i) {
        *functions[i].function = _get_gl_fn(functions[i].name);
        if (!*functions[i].function) {
            printf("Failed to load GL function: %s\n", functions[i].name);
        }
    }

    api->gl.init = LIBX_TRUE;
    api->gl.enable(GL_DEPTH_TEST);
    return LIBX_TRUE;
}


u8 _r3_init_graphics(_r3_graphics_api* api) {
    if (!api) return LIBX_FALSE;

    api->init_pipeline = _init_pipeline_impl;
    api->push_pipeline = _push_pipeline_impl;
    api->flush_pipeline = _flush_pipeline_impl;

    api->init_camera = _init_camera_impl;
    api->rotate_camera = _rotate_camera_impl;
    api->translate_camera = _translate_camera_impl;
    api->update_camera = _update_camera_impl;

    api->create_shader = _create_shader_impl;
    api->destroy_shader = _destroy_shader_impl;

    api->set_uniform = _set_uniform_impl;
    api->send_uniform = _send_uniform_impl;
    
    api->create_vertex_data = _create_vertex_data_impl;
    api->destroy_vertex_data = _destroy_vertex_data_impl;

    api->create_texture2D = _create_texture2D_impl;
    api->destroy_texture2D = _destroy_texture2D_impl;

    api->gl.init = LIBX_FALSE;
    api->init_gl = _init_gl_impl;

    internal_api_ptr = api;
    return LIBX_TRUE;
}

u8 _r3_cleanup_graphics(_r3_graphics_api* api) {
    if (!api) return LIBX_FALSE;
    
    if (api->pipeline.calls) structx->destroy_array(api->pipeline.calls);
    
    api->init_pipeline = NULL;
    api->push_pipeline = NULL;
    api->flush_pipeline = NULL;

    api->init_camera = NULL;
    api->rotate_camera = NULL;
    api->translate_camera = NULL;
    api->update_camera = NULL;

    api->create_shader = NULL;
    api->destroy_shader = NULL;

    api->set_uniform = NULL;
    api->send_uniform = NULL;

    api->create_vertex_data = NULL;
    api->destroy_vertex_data = NULL;
    
    api->create_texture2D = NULL;
    api->destroy_texture2D = NULL;

    api->init_gl = NULL;

    internal_api_ptr = NULL;

    return LIBX_TRUE;
}
