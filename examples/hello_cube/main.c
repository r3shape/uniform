#define LOTUS_3D
#include "../../engine/include/lotus.h"

#include <stdio.h>

int main() {
    lotus_init_core();

    Lotus_Window window = lotus_platform_api->create_window("Hello Cube 3D", 1280, 720);
    lotus_platform_api->create_gl_context(&window);

    lotus_init_graphics();
    lotus_graphics_api->GL_API.viewport(
        0,
        0,
        window.size[0],
        window.size[1]
    );

    Lotus_Shader shader = lotus_graphics_api->make_shader(
        lotus_read_file("engine/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("engine/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);

    // camera data handled manually for now :)

    Lotus_Mat4 model = lotus_identity();
    Lotus3D_Primitive cube = lotus3d_create_cube(lotus_new_vec3(100, 100, 100), LOTUS_COLOR3(25, 80, 150));

    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(133, 161, 172, 255),
        lotus_perspective(lotus_to_radians(45.0), window.size[0]/window.size[1], 0.1, 1000.0)
    );    

    Lotus_Camera cam = lotus_init_camera(
        lotus_new_vec3(0, 0, 1.0),
        lotus_new_vec3(0, 0, 0)
    );

    lotus_graphics_api->set_uniform(&shader, "u_view", &cam.view);
    
    // lotus_platform_api->hide_cursor(&window);
    lotus_platform_api->bound_cursor(&window);

    f32 angle = 0.0;
    ubyte running = 1;
    while (running) {
        lotus_platform_api->poll_events();
        lotus_graphics_api->draw_clear();

        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) running = 0;

        if (lotus_key_is_down(LOTUS_KEY_W)) lotus_translate_camera(&cam, 0, 0, 1);
        if (lotus_key_is_down(LOTUS_KEY_S)) lotus_translate_camera(&cam, 0, 0, -1);
        
        if (lotus_key_is_down(LOTUS_KEY_D)) lotus_translate_camera(&cam, 1, 0, 0);
        if (lotus_key_is_down(LOTUS_KEY_A)) lotus_translate_camera(&cam, -1, 0, 0);

        if (lotus_key_is_down(LOTUS_KEY_SPACE)) lotus_translate_camera(&cam, 0, 1, 0);
        if (lotus_key_is_down(LOTUS_KEY_SHIFT)) lotus_translate_camera(&cam, 0, -1, 0);
    
        // camera data handled manually for now :)
        lotus_graphics_api->send_uniform(&shader, LOTUS_UNIFORM_MAT4, "u_view");
        
        angle += 1;
        // model = lotus_mul_mat4(lotus_identity(), lotus_rot_mat4(1, 0, 0, angle));

        lotus_graphics_api->set_uniform(&shader, "u_model", &model);
        lotus_graphics_api->draw_data(cube.vertexData);
        
        lotus_freelook_camera(&cam);
        lotus_update_camera(&cam);

        lotus_platform_api->poll_inputs();
        lotus_platform_api->swap_buffers(&window);
    }

    lotus_graphics_api->destroy_vertex_data(&cube.vertexData);
    lotus_platform_api->destroy_gl_context(&window);
    lotus_platform_api->destroy_window(&window);

    lotus_shutdown_graphics();

    lotus_shutdown_core();
    return 0;
}