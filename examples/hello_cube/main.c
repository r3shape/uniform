/* Now fully ECS-driven! */

#define R3_3D
#include "../../r3engine/include/r3engine.h"

#include <stdio.h>

int main() {
    r3_init_3D();
    
    R3_Window* window = r3_platform_api->create_window("Hello Cube", 1280, 720);
    r3_platform_api->create_gl_context(window);

    r3_init_graphics();
    r3_graphics_api->GL_API.viewport(0,0, window->size[0], window->size[1]);

    R3_Camera* camera = r3_init_camera(
        r3_new_vec3(0, 0, 1.0),
        R3_FLY_CAMERA
    );

    R3_Shader shader = r3_graphics_api->create_shader(
        r3_read_file("r3engine/assets/shaders/default001/vertex.glsl"),
        r3_read_file("r3engine/assets/shaders/default001/fragment.glsl")
    );
    r3_graphics_api->set_shader(&shader);

    r3_graphics_api->draw_begin(
        R3_TRIANGLE_MODE,
        R3_COLOR4(123, 161, 172, 255),
        r3_perspective(r3_to_radians(45.0), window->size[0]/window->size[1], 0.1, 1000.0)
    );

    ubyte2 entity = r3_ecs_api->create_entity();
    R3_Mesh3D* mesh = r3_ecs_api->add_component(entity, R3_MESH3D);
    R3_Transform3D* transform = r3_ecs_api->add_component(entity, R3_TRANSFORM3D);
    R3_Texture2D* texture = r3_ecs_api->add_component(entity, R3_TEXTURE2D);

    mesh->size = r3_new_vec3(64, 64, 64);
    mesh->color = R3_COLOR3(25, 180, 150);
    mesh->vertexData = r3_3D->create_cube3D(mesh->size, mesh->color);

    transform->speed = 1.0;
    transform->scale = r3_new_vec3(1.0, 1.0, 1.0);
    transform->rotation = r3_new_vec3(0.0, 0.0, 0.0);
    transform->location = r3_new_vec3(1.0, 1.0, 0.0);

    *texture = r3_graphics_api->create_texture2D("r3engine/assets/textures/r3-logo.png", R3_RGBA);

    r3_platform_api->hide_cursor(window);
    r3_platform_api->center_cursor(window);

    r3_graphics_api->toggle_vsync(0);
    r3_platform_api->set_clock(60.0);

    ubyte running = 1;
    while (running) {
        r3_platform_api->poll_inputs();
        r3_platform_api->poll_events();
        r3_graphics_api->draw_clear();
        
        if (r3_key_is_down(R3_KEY_ESCAPE)) running = 0;
        
        if (r3_key_is_down(R3_KEY_F1)) r3_graphics_api->toggle_wireframe(1);

        if (r3_key_is_down(R3_KEY_W)) r3_translate_camera(0, 0, 1);
        if (r3_key_is_down(R3_KEY_S)) r3_translate_camera(0, 0, -1);
        
        if (r3_key_is_down(R3_KEY_D)) r3_translate_camera(1, 0, 0);
        if (r3_key_is_down(R3_KEY_A)) r3_translate_camera(-1, 0, 0);
        
        if (r3_key_is_down(R3_KEY_SPACE)) r3_translate_camera(0, 1, 0);
        if (r3_key_is_down(R3_KEY_SHIFT)) r3_translate_camera(0, -1, 0);
        
        r3_update_camera(&camera);
        r3_ecs_api->run_system(R3_TRANSFORM3D);
        r3_ecs_api->run_system(R3_MESH3D);
        
        r3_platform_api->swap_buffers(window);
        r3_platform_api->update_clock();
    }

    r3_graphics_api->destroy_shader(&shader);
    r3_ecs_api->destroy_entity(entity);

    r3_shutdown_graphics();
    r3_shutdown_3D();
    return 0;
}