#define LOTUS_2D
#define LOTUS_APPLICATION
#include "../../lotus/include/lotus.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 a_Location;\n"
    "layout(location = 1) in vec3 a_Color;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "out vec3 vertex_color;\n"
    "void main() {\n"
    "   gl_Position = u_projection * u_view * u_model * vec4(a_Location, 1.0f);\n"
    "   vertex_color = a_Color;\n"
    "}\0"
};

char fShader[] = {
    "#version 460 core\n"
    "in vec3 vertex_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "   frag_color = vec4(vertex_color, 1.0f);\n"
    "}\0"
};
Lotus_Shader my_shader;

Lotus_Application_API* app_api = NULL;
Lotus_Application* app = NULL;

Lotus_Mat4 m_model;
Lotus_Primitive triangle_prim;

// camera-data handled manually until implentation :)
Lotus_Mat4 m_view;
Lotus_Vec3 up = {0.0f, 1.0f, 0.0f};
Lotus_Vec3 eye = {0.0f, 0.0f, 1.0f};
Lotus_Vec3 center = {0.0f, 0.0f, 0.0f};

ubyte hello_triangle_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_APPLICATION_MIDFRAME_EVENT) {
        app->resource.graphics_api->draw_clear();
    
        app->resource.graphics_api->send_uniform(&my_shader, LOTUS_UNIFORM_MAT4, "u_view");

        app->resource.graphics_api->draw_primitive(triangle_prim);
        
        return LOTUS_TRUE;
    }; return LOTUS_FALSE;
}

int main() {
    app_api = lotus_init_application();
    app = app_api->initialize("Hello Triangle", LOTUS_VEC2(ubyte4, 1280, 720));

    triangle_prim = app->resource.graphics_api->lotus_2D.make_triangle(lotus_new_vec2(64, 64), LOTUS_COLOR3(25, 80, 150));
    
    my_shader = app->resource.graphics_api->make_shader(vShader, fShader);
    app->resource.graphics_api->set_shader(&my_shader);
    
    // set model matrix
    m_model = lotus_identity();
    m_model = lotus_mul_mat4(lotus_identity(), lotus_trans_mat4(1280/2, 720/2, 0));
    app->resource.graphics_api->set_uniform(&my_shader, "u_model", &m_model);

    // set view matrix
    m_view = lotus_look_at(eye, center, up);
    app->resource.graphics_api->set_uniform(&my_shader, "u_view", &m_view);
    
    // 2D camera
    app->resource.graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(133, 161, 172, 255),
        lotus_ortho(0, app->resource.window.size[0], 0, app->resource.window.size[1], 0.1, 10)
    );
    
    app_api->set_callback(LOTUS_APPLICATION_MIDFRAME_EVENT, hello_triangle_callback);
    app_api->run();

    return 0;
}