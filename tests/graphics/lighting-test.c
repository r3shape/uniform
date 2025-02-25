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

void configure_entities(R3_Shader* light_shader, R3_Shader* object_shader) {
    for (sbyte4 z = 0; z < entity_per_dimension; z++) {
        for (sbyte4 x = 0; x < entity_per_dimension; x++) {
            entities[z][x] = r3_ecs_api->create_entity();
            R3_Mesh3D* mesh = r3_ecs_api->add_component(entities[z][x], R3_MESH3D);
            R3_Transform3D* transform = r3_ecs_api->add_component(entities[z][x], R3_TRANSFORM3D);
            R3_Material* object_material = r3_ecs_api->add_component(entities[z][x], R3_MATERIAL);
            
            *object_material = r3_graphics_api->create_material(object_shader);
            
            if (x < 5) mesh->color = R3_COLOR3(255, 0, 0);
            else mesh->color = R3_COLOR3(0, 0, 255);
            
            mesh->size = r3_new_vec3(100, 100, 100);
            mesh->vertexData = r3_3D->create_cube3D(mesh->size, mesh->color);
            
            transform->speed = 1.0;
            transform->scale = r3_new_vec3(1.0, 1.0, 1.0);
            transform->rotation = r3_new_vec3(0.0, 0.0, 0.0);
            transform->location = r3_new_vec3(x, 0.0, -z);
        }
    }
    
    ubyte2 light = r3_ecs_api->create_entity();
    R3_Mesh3D* mesh = r3_ecs_api->add_component(light, R3_MESH3D);
    R3_Transform3D* transform = r3_ecs_api->add_component(light, R3_TRANSFORM3D);
    R3_Material* light_material = r3_ecs_api->add_component(light, R3_MATERIAL);
    
    *light_material = r3_graphics_api->create_material(light_shader);

    mesh->color = R3_COLOR3(255, 255, 255);
    mesh->size = r3_new_vec3(32, 32, 32);
    mesh->vertexData = r3_3D->create_cube3D(mesh->size, mesh->color);

    transform->speed = 1.0;
    transform->scale = r3_new_vec3(1.0, 1.0, 1.0);
    transform->rotation = r3_new_vec3(0.0, 0.0, 0.0);
    transform->location = r3_new_vec3(4.5, 1.0, -4.5);
}

int main() {
    r3_init_3D();
    R3_Window* window = r3_platform_api->create_window("Phong Lighting", 1280, 720);
    r3_platform_api->create_gl_context(window);

    r3_init_graphics();
    r3_event_api->register_callback(R3_EVENT_RESIZE, resize_callback);

    R3_Camera* camera = r3_init_camera(r3_new_vec3(0, 0, 3.0), R3_FLY_CAMERA);

    R3_Shader light_shader = r3_graphics_api->create_shader(
        r3_read_file("r3engine/assets/shaders/light/source/vert.glsl"),
        r3_read_file("r3engine/assets/shaders/light/source/frag.glsl")
    );
    R3_Shader object_shader = r3_graphics_api->create_shader(
        r3_read_file("r3engine/assets/shaders/light/object/vert.glsl"),
        r3_read_file("r3engine/assets/shaders/light/object/frag.glsl")
    );

    configure_entities(&light_shader, &object_shader);
    configure_state(window);

    struct Light {
        R3_Vec3 ambient;
        R3_Vec3 diffuse;
        R3_Vec3 specular;
    } light;

    light.ambient = r3_new_vec3(10.0, 10.0, 10.0);
    light.diffuse = r3_new_vec3(0.5, 0.5, 0.5);
    light.specular = r3_new_vec3(1.0, 1.0, 1.0);
    R3_Transform3D* light_transform = r3_ecs_api->get_component(100, R3_TRANSFORM3D);
    r3_graphics_api->set_uniform(&object_shader, R3_UNIFORM_VEC3, "u_light.ambient", &light.ambient);
    r3_graphics_api->set_uniform(&object_shader, R3_UNIFORM_VEC3, "u_light.diffuse", &light.diffuse);
    r3_graphics_api->set_uniform(&object_shader, R3_UNIFORM_VEC3, "u_light.specular", &light.specular);
    r3_graphics_api->set_uniform(&object_shader, R3_UNIFORM_VEC3, "u_light.location", &light_transform->location);

    r3_graphics_api->set_uniform(&object_shader, R3_UNIFORM_VEC3, "u_cam_location", &camera->location);

    ubyte running = 1;
    while (running) {
        r3_graphics_api->draw_clear();
        r3_platform_api->poll_inputs();
        r3_platform_api->poll_events();

        if (r3_key_is_down(R3_KEY_ESCAPE)) running = 0;

        if (r3_key_is_down(R3_KEY_F1)) r3_graphics_api->toggle_wireframe(1);
        else r3_graphics_api->toggle_wireframe(0);

        if (r3_key_is_down(R3_KEY_W)) r3_translate_camera(0, 0, 1);
        if (r3_key_is_down(R3_KEY_S)) r3_translate_camera(0, 0, -1);

        if (r3_key_is_down(R3_KEY_D)) r3_translate_camera(1, 0, 0);
        if (r3_key_is_down(R3_KEY_A)) r3_translate_camera(-1, 0, 0);

        if (r3_key_is_down(R3_KEY_SPACE)) r3_translate_camera(0, 1, 0);
        if (r3_key_is_down(R3_KEY_SHIFT)) r3_translate_camera(0, -1, 0);

        if (r3_key_is_down(R3_KEY_UP)) light_transform->velocity.y = light_transform->speed;
        if (r3_key_is_down(R3_KEY_DOWN)) light_transform->velocity.y = -light_transform->speed;
        if (r3_key_is_down(R3_KEY_LEFT)) light_transform->velocity.x = -light_transform->speed;
        if (r3_key_is_down(R3_KEY_RIGHT)) light_transform->velocity.x = light_transform->speed;

        r3_graphics_api->send_uniform(&object_shader, R3_UNIFORM_VEC3, "u_light.location");
        r3_graphics_api->send_uniform(&object_shader, R3_UNIFORM_VEC3, "u_cam_location");

        r3_update_camera();
        r3_ecs_api->run_system(R3_TRANSFORM3D);
        r3_ecs_api->run_system(R3_MESH3D);

        r3_platform_api->swap_buffers(window);
        r3_platform_api->update_clock();
    }

    r3_graphics_api->destroy_shader(&object_shader);
    r3_graphics_api->destroy_shader(&light_shader);
    cleanup_entities();

    r3_shutdown_graphics();
    r3_shutdown_3D();
    return 0;
}
