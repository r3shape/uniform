/* Now fully ECS-driven! */

#define LOTUS_3D
#include "../../lotus/include/lotus.h"

#include <stdio.h>

int main() {
    lotus_init_3D();
    
    Lotus_Window* window = lotus_platform_api->create_window("Hello Cube", 1280, 720);
    lotus_platform_api->create_gl_context(window);

    lotus_init_graphics();
    lotus_graphics_api->GL_API.viewport(0,0, window->size[0], window->size[1]);

    Lotus_Camera camera = lotus_init_camera(lotus_new_vec3(0, 0, 3.0), LOTUS_FLY_CAMERA);

    Lotus_Shader shader = lotus_graphics_api->create_shader(
        lotus_read_file("lotus/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("lotus/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);

    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(123, 161, 172, 255),
        lotus_perspective(lotus_to_radians(45.0), window->size[0]/window->size[1], 0.1, 1000.0)
    );

    ubyte2 entity = lotus_ecs_api->create_entity();
    Lotus_Mesh3D* mesh = lotus_ecs_api->add_component(entity, LOTUS_MESH3D);
    Lotus_Transform3D* transform = lotus_ecs_api->add_component(entity, LOTUS_TRANSFORM3D);
    Lotus_Texture2D* texture = lotus_ecs_api->add_component(entity, LOTUS_TEXTURE2D);

    mesh->size = lotus_new_vec3(32, 32, 32);
    mesh->color = LOTUS_COLOR3(25, 180, 150);
    mesh->vertexData = lotus_3D->create_cube3D(mesh->size, mesh->color);

    transform->speed = 1.0;
    transform->scale = lotus_new_vec3(1.0, 1.0, 1.0);
    transform->rotation = lotus_new_vec3(0.0, 0.0, 0.0);
    transform->location = lotus_new_vec3(1.0, 1.0, 0.0);

    *texture = lotus_graphics_api->create_texture2D("lotus/assets/textures/crate.png", LOTUS_RGBA);

    lotus_platform_api->hide_cursor(window);
    lotus_platform_api->center_cursor(window);

    lotus_graphics_api->toggle_vsync(0);
    lotus_platform_api->set_clock(144.0);
    ubyte running = 1;
    while (running) {
        lotus_platform_api->poll_inputs();
        lotus_platform_api->poll_events();
        lotus_graphics_api->draw_clear();
        
        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (lotus_key_is_down(LOTUS_KEY_F1)) lotus_graphics_api->toggle_wireframe(1);
        else lotus_graphics_api->toggle_wireframe(0);

        if (lotus_key_is_down(LOTUS_KEY_W)) lotus_translate_camera(&camera, 0, 0, 1);
        if (lotus_key_is_down(LOTUS_KEY_S)) lotus_translate_camera(&camera, 0, 0, -1);
        
        if (lotus_key_is_down(LOTUS_KEY_D)) lotus_translate_camera(&camera, 1, 0, 0);
        if (lotus_key_is_down(LOTUS_KEY_A)) lotus_translate_camera(&camera, -1, 0, 0);
        
        if (lotus_key_is_down(LOTUS_KEY_SPACE)) lotus_translate_camera(&camera, 0, 1, 0);
        if (lotus_key_is_down(LOTUS_KEY_SHIFT)) lotus_translate_camera(&camera, 0, -1, 0);
        
        transform->rotation = lotus_add_vec3(transform->rotation, (Lotus_Vec3){0.5, 0.0, 0.0});

        lotus_ecs_api->run_system(LOTUS_TRANSFORM3D);
        lotus_ecs_api->run_system(LOTUS_MESH3D);
        
        lotus_update_camera(&camera);

        // printf("Location: (%.2f, %.2f, %.2f)\n", camera.location.x, camera.location.y, camera.location.z);
        // printf("Direction: (%.2f, %.2f, %.2f)\n", camera.direction.x, camera.direction.y, camera.direction.z);
        printf("Yaw: %.2f, Pitch: %.2f\n", camera.yaw, camera.pitch);

        
        lotus_platform_api->swap_buffers(window);
        lotus_platform_api->update_clock();
    }

    lotus_graphics_api->destroy_shader(&shader);
    lotus_ecs_api->destroy_entity(entity);

    lotus_shutdown_graphics();
    lotus_shutdown_3D();
    return 0;
}
