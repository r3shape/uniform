#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include "r3.defines.h"

// GL equivalent defines
#define R3_STATIC_DRAW                0x88E4
#define R3_ARRAY_BUFFER               0x8892
#define R3_ELEMENT_ARRAY_BUFFER       0x8893

#define R3_VERTEX_SHADER              0x8B31
#define R3_FRAGMENT_SHADER            0x8B30
#define R3_LINK_STATUS                0x8B82
#define R3_COMPILE_STATUS             0x8B81
#define R3_ACTIVE_ATTRIBUTES          0x8B89
#define R3_SHADING_LANGUAGE_VERSION   0x8B8C

#define R3_TEXT_UNIT0                 0x84C0
#define R3_TEXT_UNIT1                 0x84C1
#define R3_TEXT_UNIT2                 0x84C2
#define R3_TEXT_UNIT3                 0x84C3
#define R3_ACTIVE_TEXTURE             0x84E0
#define R3_MAX_TEXTURE_UNITS          0x84E2

#define R3_LINE                       0x1B01
#define R3_FRONT_AND_BACK             0x0408

typedef enum R3_Vertex_Attribute {
    R3_LOCATION_ATTR = 1 << 0, // 1 (0b0001)
    R3_COLOR_ATTR    = 1 << 1, // 2 (0b0010)
    R3_TCOORD_ATTR   = 1 << 2, // 4 (0b0100)
    R3_NORMAL_ATTR   = 1 << 3, // 8 (0b1000)
    R3_VERTEX_ATTRIBS  = 1 << 4 
} R3_Vertex_Attribute;

typedef struct R3_Vertex_Data {
    u8 attrs;
    u32 vbo;
    u32 ebo;
    u32 vao;
    u32* indices;
    f32* vertices;
    u32 indexCount;
    u32 vertexCount;
} R3_Vertex_Data;

typedef enum R3_Render_Mode {
    R3_POINT_MODE = 0x0000,
    R3_LINE_MODE = 0x0001,
    R3_TRIANGLE_MODE = 0x0004,
    R3_QUAD_MODE = 0x0007,
    R3_DRAW_MODES
} R3_Render_Mode;

typedef struct R3_Render_Call {
    u32 mode;
    u32 shader;
    u32 indices;
    u32 vertices;
    union bind {
        u32 vao;
        u32 ebo;
    } bind;
} R3_Render_Call;

typedef struct _r3_graphics_api {
    struct renderer {
        u32 mode;
        u32 shader;
        void* projection;
        Vector clear_color;
        R3_Render_Call* calls;
    } renderer;

    void (*set_color)(Vector vec4);
    void (*set_mode)(R3_Render_Mode mode);
    
    R3_Vertex_Data (*create_vertex_data)(f32 *vertices, u32 vertexCount, u32 *indices, u32 indexCount, u8 attrs);
    void (*destroy_vertex_data)(R3_Vertex_Data *vertexData);

    void (*render_begin)(u32 mode, Vector clear_color, void* projection);
    void (*render_call)(u32 mode, u32 shader, u32 vertices, u32 indices, u32 buffer_object);
    void (*render_clear)(void);
    void (*render_end)(void);

    u8 (*init_gl)(struct _r3_graphics_api* api);
    struct gl {
        u8 init;

        // BUFFER FUNCTIONS
        void (*gen_buffers)(i32 count, u32 *buffer);
        void (*bind_buffer)(u32 target, u32 buffer);
        void (*buffer_data)(u32 target, i32 size, const void *data, u32 usage);
        
        // VERTEX ATTRIBUTE FUNCTIONS
        void (*delete_buffers)(i32 count, const u32 *buffer);
        void (*map_buffer)(u32 target, u32 access);
        void (*unmap_buffer)(u32 target);
        void (*buffer_sub_data)(u32 target, u32* offset, i32 size, const void *data);
        void (*get_buffer_parameteriv)(u32 target, u32 pname, u32 *params);
        
        // SHADER FUNCTIONS        
        void (*enable_vertex_attrib_array)(u32 index);
        void (*disable_vertex_attrib_array)(u32 index);
        void (*vertex_attrib_pointer)(u32 index, u32 size, u32 type, u8 normalized, i32 stride, const void *pointer);
        void (*bind_vertex_array)(u32 array);
        void (*gen_vertex_arrays)(i32 count, u32 *arrays);
        void (*delete_vertex_arrays)(i32 count, const u32 *arrays);
        
        // TEXTURE FUNCTIONS        
        u32 (*create_program)(void);
        void (*use_program)(u32 program);
        void (*link_program)(u32 program);
        void (*delete_program)(u32 program);
        
        u32 (*create_shader)(u32 type);
        void (*compile_shader)(u32 shader);
        void (*attach_shader)(u32 program, u32 shader);
        void (*detach_shader)(u32 program, u32 shader);
        void (*delete_shader)(u32 shader);
        void (*shader_source)(u32 shader, i32 count, const char **strings, const int *lengths);
        
        void (*get_shaderiv)(u32 shader, u32 pname, u32 *params);
        void (*get_shader_info_log)(u32 shader, i32 maxLength, i32 *length, char *infoLog);
        void (*get_programiv)(u32 program, u32 pname, u32 *params);
        void (*get_program_info_log)(u32 program, i32 maxLength, i32 *length, char *infoLog);
        u32 (*get_uniform_location)(u32 program, const char* name);
        
        void (*uniform1f)(u32 location, const f32* value);
        void (*uniform2fv)(u32 location, u32 count, const f32* value);
        void (*uniform3fv)(u32 location, u32 count, const f32* value);
        void (*uniform4fv)(u32 location, u32 count, const f32* value);
        void (*uniform_matrix4fv)(u32 location, u32 count, u32 transpose, const f32* value);
        
        void (*gen_textures)(i32 count, u32 *textures);
        void (*bind_texture)(u32 target, u32 texture);
        void (*tex_parameteri)(u32 target, u32 pname, u32 param);
        void (*tex_image2D)(u32 target, u32 level, u32 internalFormat, i32 width, i32 height, u32 border, u32 format, u32 type, const void *data);
        void (*active_texture)(u32 texture);
        void (*delete_textures)(i32 count, const u32 *textures);
        void (*generate_mipmap)(u32 target);
        void (*generate_texture_mipmap)(u32 texture);
        
        void (*gen_framebuffers)(i32 count, u32 *framebuffers);
        void (*bind_framebuffer)(u32 target, u32 framebuffer);
        void (*framebuffer_texture2d)(u32 target, u32 attachment, u32 textarget, u32 texture, u32 level);
        void (*delete_framebuffers)(i32 count, const u32 *framebuffers);
        u32 (*check_framebuffer_status)(u32 target);
        
        void (*clear)(i32 mask);
        void (*draw_arrays)(u32 mode, u32 first, i32 count);
        void (*clear_color)(f32 r, f32 g, f32 b, f32 a);
        void (*draw_elements)(u32 mode, i32 count, u32 type, const void *indices);
        void (*draw_elements_base_vertex)(u32 mode, i32 count, u32 type, const void *indices, u32 baseVertex);
        
        // STATE MANAGEMENT FUNCTIONS        
        void (*enable)(u32 cap);
        void (*disable)(u32 cap);
        void (*blend_func)(u32 sfactor, u32 dfactor);
        void (*cull_face)(u32 mode);
        void (*depth_func)(u32 func);
        void (*viewport)(u32 x, u32 y, i32 width, i32 height);
        void (*polygon_mode)(u32 face, u32 mode);
        
        // UTILITY FUNCTIONS        
        u32 (*get_error)(void);
        const u8 *(*get_string)(const u32 name);
    } gl;
} _r3_graphics_api;

u8 _r3_init_graphics(_r3_graphics_api* api);
u8 _r3_cleanup_graphics(_r3_graphics_api* api);