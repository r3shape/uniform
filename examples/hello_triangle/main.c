/* Now fully ECS-driven! */

#define LOTUS_2D
#include "../../lotus/include/lotus.h"

#include <stdio.h>

int main() {
    lotus_init_2D();

    Lotus_Window* window = lotus_platform_api->create_window("Hello Triangle", 1280, 720);
    lotus_platform_api->create_gl_context(window);

    lotus_init_graphics();
    lotus_graphics_api->GL_API.viewport(0,0, window->size[0], window->size[1]);

    Lotus_Camera cam = lotus_init_camera(
        lotus_new_vec3(0, 0, 1.0),
        lotus_new_vec3(0, 0, 0)
    );

    Lotus_Shader shader = lotus_graphics_api->create_shader(
        lotus_read_file("lotus/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("lotus/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);

    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(123, 161, 172, 255),
        lotus_ortho(0, window->size[0], 0, window->size[1], 0.1, 10.0)
    );

    ubyte2 entity = lotus_ecs_api->create_entity();
    Lotus_Mesh2D* mesh = lotus_ecs_api->add_component(entity, LOTUS_MESH2D);
    Lotus_Transform2D* transform = lotus_ecs_api->add_component(entity, LOTUS_TRANSFORM2D);
    Lotus_Texture2D* texture = lotus_ecs_api->add_component(entity, LOTUS_TEXTURE2D);

    mesh->size = lotus_new_vec2(320, 320);
    mesh->color = LOTUS_COLOR3(85, 42, 115);
    mesh->vertexData = lotus_2D->create_triangle2D(mesh->size, mesh->color);

    transform->speed = 5.0;
    transform->scale = lotus_new_vec2(1.0, 1.0);
    transform->location = lotus_new_vec2(640, 360);

    *texture = lotus_graphics_api->create_texture2D("lotus/assets/textures/chip.png", LOTUS_RGBA);

    ubyte running = 1;
    while (running) {
        lotus_platform_api->poll_events();
        lotus_platform_api->poll_inputs();
        lotus_graphics_api->draw_clear();

        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (lotus_key_is_down(LOTUS_KEY_A)) transform->velocity.x = -transform->speed;
        else if (lotus_key_is_down(LOTUS_KEY_D)) transform->velocity.x = transform->speed;
        else transform->velocity.x = 0.0;
        
        if (lotus_key_is_down(LOTUS_KEY_W)) transform->velocity.y = transform->speed;
        else if (lotus_key_is_down(LOTUS_KEY_S)) transform->velocity.y = -transform->speed;
        else transform->velocity.y = 0.0;

        lotus_ecs_api->run_system(LOTUS_TRANSFORM2D);
        lotus_ecs_api->run_system(LOTUS_MESH2D);

        lotus_update_camera(&cam);
        lotus_platform_api->swap_buffers(window);
    }

    lotus_graphics_api->destroy_shader(&shader);
    lotus_ecs_api->destroy_entity(entity);

    lotus_shutdown_graphics();
    lotus_shutdown_2D();
    return 0;
}
