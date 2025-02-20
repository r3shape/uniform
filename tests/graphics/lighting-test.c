#define R3_3D
#include "../../r3engine/include/r3engine.h"

#include <stdio.h>

ubyte resize_callback(R3_Event data, ubyte2 code) {
    if (code != R3_EVENT_RESIZE) return R3_FALSE;
    r3_graphics_api->GL_API.viewport(0, 0, data.ubyte2[0], data.ubyte2[1]);
    return R3_TRUE;
}

void configure_state(R3_Window* window) {
    r3_platform_api->set_clock(144.0);
    r3_platform_api->hide_cursor(window);
    r3_platform_api->center_cursor(window);

    r3_graphics_api->toggle_vsync(0);
    r3_graphics_api->draw_begin(
        R3_TRIANGLE_MODE,
        R3_COLOR4(123, 161, 172, 255),
        r3_perspective(r3_to_radians(45.0), window->size[0]/window->size[1], 0.1, 1000.0)
    );
}

ubyte2 entities[10][10];
ubyte4 entity_per_dimension = 10;
void cleanup_entities(void) {
    for (ubyte4 z = 0; z < entity_per_dimension; z++) {
        for (ubyte4 x = 0; x < entity_per_dimension; x++) {
            r3_ecs_api->destroy_entity(entities[z][x]);
        }
    }
}

void configure_entities(void) {
    for (sbyte4 z = 0; z < entity_per_dimension; z++) {
        for (sbyte4 x = 0; x < entity_per_dimension; x++) {
            entities[z][x] = r3_ecs_api->create_entity();
            R3_Mesh3D* mesh = r3_ecs_api->add_component(entities[z][x], R3_MESH3D);
            R3_Texture2D* texture = r3_ecs_api->add_component(entities[z][x], R3_TEXTURE2D);
            R3_Transform3D* transform = r3_ecs_api->add_component(entities[z][x], R3_TRANSFORM3D);
        
            if (x < 5) mesh->color = R3_COLOR3(255, 0, 0);
            else mesh->color = R3_COLOR3(0, 0, 255);

            mesh->size = r3_new_vec3(100, 100, 100);
            mesh->vertexData = r3_3D->create_cube3D(mesh->size, mesh->color);
        
            transform->speed = 1.0;
            transform->scale = r3_new_vec3(1.0, 1.0, 1.0);
            transform->rotation = r3_new_vec3(0.0, 0.0, 0.0);
            transform->location = r3_new_vec3(x, 0.0, -z);
        
            *texture = r3_graphics_api->create_texture2D("r3engine/assets/textures/crate.png", R3_RGBA);
        }
    }

    ubyte2 light = r3_ecs_api->create_entity();
    R3_Mesh3D* mesh = r3_ecs_api->add_component(light, R3_MESH3D);
    R3_Texture2D* texture = r3_ecs_api->add_component(light, R3_TEXTURE2D);
    R3_Transform3D* transform = r3_ecs_api->add_component(light, R3_TRANSFORM3D);

    mesh->color = R3_COLOR3(255, 255, 255);
    mesh->size = r3_new_vec3(32, 32, 32);
    mesh->vertexData = r3_3D->create_cube3D(mesh->size, mesh->color);

    transform->speed = 1.0;
    transform->scale = r3_new_vec3(1.0, 1.0, 1.0);
    transform->rotation = r3_new_vec3(0.0, 0.0, 0.0);
    transform->location = r3_new_vec3(4.5, 1.0, -4.5);

    *texture = r3_graphics_api->create_texture2D("r3engine/assets/textures/light.png", R3_RGBA);
}

int main() {
    r3_init_3D();
    
    R3_Window* window = r3_platform_api->create_window("Phong Lighting", 1280, 720);
    r3_platform_api->create_gl_context(window);

    r3_init_graphics();
    r3_event_api->register_callback(R3_EVENT_RESIZE, resize_callback);

    R3_Camera camera = r3_init_camera(r3_new_vec3(0, 0, 3.0), R3_FLY_CAMERA);

    R3_Shader shader = r3_graphics_api->create_shader(
        r3_read_file("r3engine/assets/shaders/default001/vertex.glsl"),
        r3_read_file("r3engine/assets/shaders/default001/fragment.glsl")
    );
    R3_Shader lightshader = r3_graphics_api->create_shader(
        r3_read_file("r3engine/assets/shaders/light/vert.glsl"),
        r3_read_file("r3engine/assets/shaders/light/frag.glsl")
    );
    r3_graphics_api->set_shader(&shader);

    configure_entities();
    configure_state(window);

    ubyte running = 1;
    while (running) {
        r3_platform_api->poll_inputs();
        r3_platform_api->poll_events();
        r3_graphics_api->draw_clear();

        if (r3_key_is_down(R3_KEY_ESCAPE)) running = 0;

        if (r3_key_is_down(R3_KEY_F1)) r3_graphics_api->toggle_wireframe(1);
        else r3_graphics_api->toggle_wireframe(0);

        if (r3_key_is_down(R3_KEY_L)) {
            r3_graphics_api->set_shader(&lightshader);
        } else r3_graphics_api->set_shader(&shader);

        if (r3_key_is_down(R3_KEY_W)) r3_translate_camera(&camera, 0, 0, 1);
        if (r3_key_is_down(R3_KEY_S)) r3_translate_camera(&camera, 0, 0, -1);

        if (r3_key_is_down(R3_KEY_D)) r3_translate_camera(&camera, 1, 0, 0);
        if (r3_key_is_down(R3_KEY_A)) r3_translate_camera(&camera, -1, 0, 0);

        if (r3_key_is_down(R3_KEY_SPACE)) r3_translate_camera(&camera, 0, 1, 0);
        if (r3_key_is_down(R3_KEY_SHIFT)) r3_translate_camera(&camera, 0, -1, 0);

        r3_ecs_api->run_system(R3_TRANSFORM3D);
        r3_ecs_api->run_system(R3_MESH3D);

        r3_update_camera(&camera);

        r3_platform_api->swap_buffers(window);
        r3_platform_api->update_clock();
    }

    r3_graphics_api->destroy_shader(&shader);
    cleanup_entities();

    r3_shutdown_graphics();
    r3_shutdown_3D();
    return 0;
}
