/* Now fully ECS-driven! */

#define R3_2D
#include "../../r3engine/include/r3engine.h"

#include <stdio.h>

int main() {
    r3_init_2D();

    R3_Window* window = r3_platform_api->create_window("Hello Triangle", 1280, 720);
    r3_platform_api->create_gl_context(window);

    r3_init_graphics();
    r3_graphics_api->GL_API.viewport(0,0, window->size[0], window->size[1]);

    R3_Camera camera = r3_init_camera(r3_new_vec3(0, 0, 1.0), -1);

    R3_Shader shader = r3_graphics_api->create_shader(
        r3_read_file("r3engine/assets/shaders/default001/vertex.glsl"),
        r3_read_file("r3engine/assets/shaders/default001/fragment.glsl")
    );
    r3_graphics_api->set_shader(&shader);

    r3_graphics_api->draw_begin(
        R3_TRIANGLE_MODE,
        R3_COLOR4(123, 161, 172, 255),
        r3_ortho(0, window->size[0], 0, window->size[1], 0.1, 10.0)
    );

    ubyte2 entity = r3_ecs_api->create_entity();
    R3_Mesh2D* mesh = r3_ecs_api->add_component(entity, R3_MESH2D);
    R3_Transform2D* transform = r3_ecs_api->add_component(entity, R3_TRANSFORM2D);
    R3_Texture2D* texture = r3_ecs_api->add_component(entity, R3_TEXTURE2D);

    mesh->size = r3_new_vec2(320, 320);
    mesh->color = R3_COLOR3(85, 42, 115);
    mesh->vertexData = r3_2D->create_triangle2D(mesh->size, mesh->color);

    transform->speed = 120.0;
    transform->scale = r3_new_vec2(1.0, 1.0);
    transform->location = r3_new_vec2(640, 360);

    *texture = r3_graphics_api->create_texture2D("r3engine/assets/textures/chip.png", R3_RGBA);

    r3_graphics_api->toggle_vsync(0);
    r3_platform_api->set_clock(60.0);
    ubyte running = 1;
    while (running) {
        r3_platform_api->poll_inputs();
        r3_platform_api->poll_events();
        r3_graphics_api->draw_clear();

        if (r3_key_is_down(R3_KEY_ESCAPE)) running = 0;
        
        if (r3_key_is_down(R3_KEY_A)) transform->velocity.x = -transform->speed;
        else if (r3_key_is_down(R3_KEY_D)) transform->velocity.x = transform->speed;
        else transform->velocity.x = 0.0;
        
        if (r3_key_is_down(R3_KEY_W)) transform->velocity.y = transform->speed;
        else if (r3_key_is_down(R3_KEY_S)) transform->velocity.y = -transform->speed;
        else transform->velocity.y = 0.0;

        r3_ecs_api->run_system(R3_TRANSFORM2D);
        r3_ecs_api->run_system(R3_MESH2D);

        r3_update_camera(&camera);
        r3_platform_api->swap_buffers(window);
        r3_platform_api->update_clock();
    }

    r3_graphics_api->destroy_shader(&shader);
    r3_ecs_api->destroy_entity(entity);

    r3_shutdown_graphics();
    r3_shutdown_2D();
    return 0;
}
