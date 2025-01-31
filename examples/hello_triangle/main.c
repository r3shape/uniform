#define LOTUS_2D
#include "../../engine/include/lotus.h"

int main() {
    lotus_init_core();

    Lotus_Window* window = lotus_platform_api->create_window("Hello Triangle 2D", 1280, 720);
    lotus_platform_api->create_gl_context(window);

    lotus_init_graphics();

    Lotus_Shader shader = lotus_graphics_api->make_shader(
        lotus_read_file("engine/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("engine/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);

    // 2D camera data handled manually for now :)
    Lotus_Mat4 view = lotus_identity();
    view = lotus_look_at(
        lotus_new_vec3(0.0f, 0.0f, 1.0f),
        lotus_new_vec3(0.0f, 0.0f, 0.0f),
        lotus_new_vec3(0.0f, 1.0f, 0.0f)
    );
    lotus_graphics_api->set_uniform(&shader, "u_view", &view);

    Lotus_Mat4 tModel = lotus_identity();
    tModel = lotus_mul_mat4(lotus_identity(), lotus_trans_mat4(1280/2, 720/2, 0));
    Lotus2D_Primitive triangle = lotus2d_create_triangle(lotus_new_vec2(32, 32), LOTUS_COLOR3(25, 80, 150));

    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(133, 161, 172, 255),
        lotus_ortho(0, window->size[0], 0, window->size[1], 0.1, 10.0)
    );
    
    ubyte running = 1;
    while (running) {
        lotus_platform_api->poll_events();
        lotus_graphics_api->draw_clear();

        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) running = 0;

        // camera uniform handled manually for now :)
        lotus_graphics_api->send_uniform(&shader, LOTUS_UNIFORM_MAT4, "u_view");
        
        lotus_graphics_api->set_uniform(&shader, "u_model", &tModel);
        lotus_graphics_api->draw_data(triangle.vertexData);
        
        lotus_platform_api->poll_inputs();
        lotus_platform_api->swap_buffers(window);
    }

    lotus_graphics_api->destroy_vertex_data(&triangle.vertexData);
    lotus_platform_api->destroy_gl_context(window);
    lotus_platform_api->destroy_window(window);

    lotus_shutdown_graphics();
    lotus_shutdown_core();
    return 0;
}