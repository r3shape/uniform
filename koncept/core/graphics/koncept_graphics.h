#pragma once

#include <GL/gl.h>
#include <GL/glu.h>
#include <koncept/core/koncept_defines.h>

// GL equivalent defines
#define KC_STATIC_DRAW                0x88E4
#define KC_ARRAY_BUFFER               0x8892
#define KC_ELEMENT_ARRAY_BUFFER       0x8893

#define KC_VERTEX_SHADER              0x8B31
#define KC_FRAGMENT_SHADER            0x8B30
#define KC_LINK_STATUS                0x8B82
#define KC_COMPILE_STATUS             0x8B81
#define KC_ACTIVE_ATTRIBUTES          0x8B89
#define KC_SHADING_LANGUAGE_VERSION   0x8B8C

#define KC_TEXT_UNIT0                 0x84C0
#define KC_TEXT_UNIT1                 0x84C1
#define KC_TEXT_UNIT2                 0x84C2
#define KC_TEXT_UNIT3                 0x84C3
#define KC_ACTIVE_TEXTURE             0x84E0
#define KC_MAX_TEXTURE_UNITS          0x84E2

#define KC_LINE                       0x1B01
#define KC_FRONT_AND_BACK             0x0408

typedef enum KC_Uniform_Type {
    KC_UNIFORM_NONE=0,
    KC_UNIFORM_FLOAT,
    KC_UNIFORM_VEC2,
    KC_UNIFORM_VEC3,
    KC_UNIFORM_VEC4,
    KC_UNIFORM_MAT4,
    KC_UNIFORM_TYPES
} KC_Uniform_Type;

typedef struct KC_Uniform {
    void* value;
    cstr name;
    u32 type;
    u32 location;
} KC_Uniform;

typedef struct KC_Shader {
    Hash_Array* uniforms;
    u32 program;
} KC_Shader;

typedef enum KC_Vertex_Attribute {
    KC_LOCATION_ATTR = 1 << 0, // 1 (0b0001)
    KC_COLOR_ATTR    = 1 << 1, // 2 (0b0010)
    KC_TCOORD_ATTR   = 1 << 2, // 4 (0b0100)
    KC_NORMAL_ATTR   = 1 << 3, // 8 (0b1000)
    KC_VERTEX_ATTRIBS  = 1 << 4 
} KC_Vertex_Attribute;

typedef struct KC_Vertex_Data {
    u8 attrs;
    u32 vbo;
    u32 ebo;
    u32 vao;
    u32* indices;
    f32* vertices;
    u32 indexCount;
    u32 vertexCount;
} KC_Vertex_Data;

typedef enum KC_Texture_Format {
    KC_RGB_FORMAT = 0x1907,
    KC_RGBA_FORMAT = 0x1908
} KC_Texture_Format;

typedef struct KC_Texture {
    u8* raw;
    str path;
    u32 id;
    u32 width;
    u32 height;
    u32 channels;
} KC_Texture;

typedef enum KC_Render_Mode {
    KC_POINT_MODE = 0x0000,
    KC_LINE_MODE = 0x0001,
    KC_TRIANGLE_MODE = 0x0004,
    KC_QUAD_MODE = 0x0007,
    KC_DRAW_MODES
} KC_Render_Mode;

typedef enum KC_Render_Call_Type {
    KC_RENDER_ARRAYS,
    KC_RENDER_ELEMENTS,
    KC_DRAW_CALL_TYPES
} KC_Render_Call_Type;

typedef struct KC_Render_Call {
    KC_Vertex_Data* vertex;
    KC_Texture* texture;
    KC_Shader* shader;
    Mat4* model;
    u8 uniform_count;
    KC_Uniform uniforms[12];    // 12 per-call uniforms permitted as 4 uniforms are set by default in `flush_pipeline()` (u_model, u_view, u_proj, u_view_locaiton)
    KC_Render_Mode mode;
    KC_Render_Call_Type type;
} KC_Render_Call;

typedef struct _koncept_graphics_api {
    struct KC_Pipeline {
        u8 init;
        Mat4 proj;
        Vec4 clear_color;
        KC_Shader* shader;
        KC_Render_Mode mode;
        KC_Render_Call* calls;
    } pipeline;
    
    struct KC_Camera {
        u8 init;
        Mat4 view;
        f32 sensitivity;
        f32 speed;
        f32 yaw;
        f32 roll;
        f32 pitch;
        Vec3 up;
        Vec3 eye;
        Vec3 right;
        Vec3 center;
        Vec3 direction;
    } camera;

    u8 (*init_pipeline)(KC_Render_Mode mode, KC_Shader* shader, Mat4 proj);
    void (*push_pipeline)(KC_Render_Call* call);
    void (*flush_pipeline)(void);

    u8 (*init_camera)(Vec3 eye, Vec3 center, Vec3 up);
    void (*rotate_camera)( f32 dx, f32 dy);
    void (*translate_camera)(i8 x, i8 y, i8 z);
    void (*update_camera)(void);

    KC_Shader (*create_shader)(cstr vertex, cstr fragment);
    void (*destroy_shader)(KC_Shader* shader);

    u8 (*set_uniform)(KC_Shader* shader, KC_Uniform* uniform);
    u8 (*send_uniform)(KC_Shader* shader, str name);

    KC_Vertex_Data (*create_vertex_data)(f32 *vertices, u32 vertexCount, u32 *indices, u32 indexCount, u8 attrs);
    void (*destroy_vertex_data)(KC_Vertex_Data *vertexData);
    
    KC_Texture (*create_texture2D)(str path, KC_Texture_Format format);
    void (*destroy_texture2D)(KC_Texture* texture);

    u8 (*init_gl)(struct _koncept_graphics_api* api);

    void (*toggle_wireframe)(u8 toggle);

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
        
        void (*uniform1f)(u32 location, f32 value);
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
} _koncept_graphics_api;

u8 _koncept_init_graphics(_koncept_graphics_api* api);
u8 _koncept_cleanup_graphics(_koncept_graphics_api* api);