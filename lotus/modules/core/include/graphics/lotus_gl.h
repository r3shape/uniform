#pragma once

#include <GL/gl.h>
#include <GL/glu.h>

#include "../lotus_core_defines.h"

#include "../utility/lotus_math.h"
#include "../utility/lotus_hashmap.h"

#include "lotus_camera.h"

// GL equivalent defines
#define LOTUS_STATIC_DRAW                0x88E4
#define LOTUS_ARRAY_BUFFER               0x8892
#define LOTUS_ELEMENT_ARRAY_BUFFER       0x8893

#define LOTUS_VERTEX_SHADER              0x8B31
#define LOTUS_FRAGMENT_SHADER            0x8B30
#define LOTUS_LINK_STATUS                0x8B82
#define LOTUS_COMPILE_STATUS             0x8B81
#define LOTUS_ACTIVE_ATTRIBUTES          0x8B89
#define LOTUS_SHADING_LANGUAGE_VERSION   0x8B8C

#define LOTUS_TEXT_UNIT0                 0x84C0
#define LOTUS_TEXT_UNIT1                 0x84C1
#define LOTUS_TEXT_UNIT2                 0x84C2
#define LOTUS_TEXT_UNIT3                 0x84C3
#define LOTUS_ACTIVE_TEXTURE             0x84E0
#define LOTUS_MAX_TEXTURE_UNITS          0x84E2

#define LOTUS_LINE                       0x1B01
#define LOTUS_FRONT_AND_BACK             0x0408

#define LOTUS_COLOR3(r, g, b) lotus_new_vec3(r/255.0, g/255.0, b/255.0)
#define LOTUS_COLOR4(r, g, b, a) lotus_new_vec4(r/255.0, g/255.0, b/255.0, a/255.0)

typedef enum Lotus_Uniform_Type {
    LOTUS_UNIFORM_NONE=0,
    LOTUS_UNIFORM_VEC2,
    LOTUS_UNIFORM_VEC3,
    LOTUS_UNIFORM_VEC4,
    LOTUS_UNIFORM_MAT4,
    LOTUS_UNIFORM_TYPES
} Lotus_Uniform_Type;

typedef struct Lotus_Shader {
    ubyte4 program;
    Lotus_Hashmap* uniforms;
} Lotus_Shader;

typedef struct Lotus_Uniform {
    ubyte4 location;
    void* value;
    const char* name;
} Lotus_Uniform;

typedef enum Lotus_Texture_Format {
    LOTUS_RGB = 0x1907,
    LOTUS_RGBA = 0x1908
} Lotus_Texture_Format;

typedef struct Lotus_Texture2D {
    ubyte4 id;
    ubyte4 width;
    ubyte4 height;
    ubyte4 channels;
    char* path;
    ubyte* raw;
} Lotus_Texture2D;

// GL equivalents
typedef enum Lotus_Draw_Mode {
    LOTUS_POINT_MODE = 0x0000,
    LOTUS_LINE_MODE = 0x0001,
    LOTUS_TRIANGLE_MODE = 0x0004,
    LOTUS_QUAD_MODE = 0x0007,
    LOTUS_DRAW_MODES
} Lotus_Draw_Mode;

typedef enum Lotus_Vertex_Attribute {
    LOTUS_LOCATION_ATTR = 1 << 0, // 1 (0b0001)
    LOTUS_COLOR_ATTR    = 1 << 1, // 2 (0b0010)
    LOTUS_TCOORD_ATTR   = 1 << 2, // 4 (0b0100)
    LOTUS_NORMAL_ATTR   = 1 << 3, // 8 (0b1000)
    LOTUS_VERTEX_ATTRIBS  = 1 << 4 
} Lotus_Vertex_Attribute;

typedef struct Lotus_Vertex_Data {
    ubyte attrs;
    ubyte4 vbo;
    ubyte4 ebo;
    ubyte4 vao;
    f32* vertices;
    ubyte4 vertexCount;
    ubyte4* indices;
    ubyte4 indexCount;
} Lotus_Vertex_Data;

typedef struct Lotus_Graphics_State {
    ubyte8 draws;
    Lotus_Shader* shader;
    Lotus_Draw_Mode mode;
    Lotus_Mat4 projection;
    Lotus_Vec4 clearColor;
} Lotus_Graphics_State;

typedef struct Lotus_Graphics_API {
    Lotus_Graphics_State* (*get_state)(void);
    void (*cleanup)(void);

    Lotus_Vertex_Data (*create_vertex_data)(f32* vertices, ubyte4 vertexCount, ubyte4* indices, ubyte4 indexCount, ubyte attrs);
    void (*destroy_vertex_data)(Lotus_Vertex_Data* vertexData);

    Lotus_Shader (*create_shader)(const char* vertex_shader, const char* fragment_shader);
    void (*destroy_shader)(Lotus_Shader* shader);

    ubyte (*set_uniform)(Lotus_Shader* shader, const char* name, void* value);
    Lotus_Uniform (*get_uniform)(Lotus_Shader* shader, const char* name);
    void (*send_uniform)(Lotus_Shader* shader, Lotus_Uniform_Type type, const char* name);

    Lotus_Texture2D (*create_texture2D)(char* path, Lotus_Texture_Format format);
    void (*destroy_texture2D)(Lotus_Texture2D* texture);

    void (*set_color)(Lotus_Vec4 color4);
    void (*set_mode)(Lotus_Draw_Mode mode);
    void (*set_shader)(Lotus_Shader* shader);

    void (*draw_begin)(Lotus_Draw_Mode mode, Lotus_Vec4 color4, Lotus_Mat4 projection);
    void (*draw_clear)(void);
    void (*draw_data)(Lotus_Vertex_Data vertexData);

    // graphics state setters/toggles
    void (*wireframe_mode)(ubyte toggle);

    struct GL_API {
        // BUFFER FUNCTIONS
        void (*gen_buffers)(sbyte4 count, ubyte4 *buffer);
        void (*bind_buffer)(ubyte4 target, ubyte4 buffer);
        void (*buffer_data)(ubyte4 target, sbyte4 size, const void *data, ubyte4 usage);
        
        // VERTEX ATTRIBUTE FUNCTIONS
        void (*delete_buffers)(sbyte4 count, const ubyte4 *buffer);
        void (*map_buffer)(ubyte4 target, ubyte4 access);
        void (*unmap_buffer)(ubyte4 target);
        void (*buffer_sub_data)(ubyte4 target, ubyte4* offset, sbyte4 size, const void *data);
        void (*get_buffer_parameteriv)(ubyte4 target, ubyte4 pname, ubyte4 *params);
        
        // SHADER FUNCTIONS        
        void (*enable_vertex_attrib_array)(ubyte4 index);
        void (*disable_vertex_attrib_array)(ubyte4 index);
        void (*vertex_attrib_pointer)(ubyte4 index, ubyte4 size, ubyte4 type, ubyte normalized, sbyte4 stride, const void *pointer);
        void (*bind_vertex_array)(ubyte4 array);
        void (*gen_vertex_arrays)(sbyte4 count, ubyte4 *arrays);
        void (*delete_vertex_arrays)(sbyte4 count, const ubyte4 *arrays);
        
        // TEXTURE FUNCTIONS        
        ubyte4 (*create_program)(void);
        void (*use_program)(ubyte4 program);
        void (*link_program)(ubyte4 program);
        void (*delete_program)(ubyte4 program);
        ubyte4 (*create_shader)(ubyte4 type);
        void (*compile_shader)(ubyte4 shader);
        void (*attach_shader)(ubyte4 program, ubyte4 shader);
        void (*detach_shader)(ubyte4 program, ubyte4 shader);
        void (*delete_shader)(ubyte4 shader);
        void (*shader_source)(ubyte4 shader, sbyte4 count, const char **strings, const int *lengths);
        void (*get_shaderiv)(ubyte4 shader, ubyte4 pname, ubyte4 *params);
        void (*get_shader_info_log)(ubyte4 shader, sbyte4 maxLength, sbyte4 *length, char *infoLog);
        void (*get_programiv)(ubyte4 program, ubyte4 pname, ubyte4 *params);
        void (*get_program_info_log)(ubyte4 program, sbyte4 maxLength, sbyte4 *length, char *infoLog);
        ubyte4 (*get_uniform_location)(ubyte4 program, const char* name);
        void (*uniform2fv)(ubyte4 location, ubyte4 count, const f32* value);
        void (*uniform3fv)(ubyte4 location, ubyte4 count, const f32* value);
        void (*uniform4fv)(ubyte4 location, ubyte4 count, const f32* value);
        void (*uniform_matrix4fv)(ubyte4 location, ubyte4 count, ubyte4 transpose, const f32* value);
        
        void (*gen_textures)(sbyte4 count, ubyte4 *textures);
        void (*bind_texture)(ubyte4 target, ubyte4 texture);
        void (*tex_parameteri)(ubyte4 target, ubyte4 pname, ubyte4 param);
        void (*tex_image2D)(ubyte4 target, ubyte4 level, ubyte4 internalFormat, sbyte4 width, sbyte4 height, ubyte4 border, ubyte4 format, ubyte4 type, const void *data);
        void (*active_texture)(ubyte4 texture);
        void (*delete_textures)(sbyte4 count, const ubyte4 *textures);
        void (*generate_mipmap)(ubyte4 target);
        void (*generate_texture_mipmap)(ubyte4 texture);
        
        void (*gen_framebuffers)(sbyte4 count, ubyte4 *framebuffers);
        void (*bind_framebuffer)(ubyte4 target, ubyte4 framebuffer);
        void (*framebuffer_texture2d)(ubyte4 target, ubyte4 attachment, ubyte4 textarget, ubyte4 texture, ubyte4 level);
        void (*delete_framebuffers)(sbyte4 count, const ubyte4 *framebuffers);
        ubyte4 (*check_framebuffer_status)(ubyte4 target);
        
        void (*clear)(sbyte4 mask);
        void (*draw_arrays)(ubyte4 mode, ubyte4 first, sbyte4 count);
        void (*clear_color)(f32 r, f32 g, f32 b, f32 a);
        void (*draw_elements)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices);
        void (*draw_elements_base_vertex)(ubyte4 mode, sbyte4 count, ubyte4 type, const void *indices, ubyte4 baseVertex);
        
        // STATE MANAGEMENT FUNCTIONS        
        void (*enable)(ubyte4 cap);
        void (*disable)(ubyte4 cap);
        void (*blend_func)(ubyte4 sfactor, ubyte4 dfactor);
        void (*cull_face)(ubyte4 mode);
        void (*depth_func)(ubyte4 func);
        void (*viewport)(ubyte4 x, ubyte4 y, sbyte4 width, sbyte4 height);
        void (*polygon_mode)(ubyte4 face, ubyte4 mode);
        
        // UTILITY FUNCTIONS        
        ubyte4 (*get_error)(void);
        const ubyte *(*get_string)(const ubyte4 name);
    } GL_API;
} Lotus_Graphics_API;
extern Lotus_Graphics_API* lotus_graphics_api;

LOTUS_API ubyte lotus_init_graphics(void);
LOTUS_API void lotus_shutdown_graphics(void);
