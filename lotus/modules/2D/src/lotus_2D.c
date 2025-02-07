#include "../include/lotus_2D.h"
#include "../include/lotus_shapes2D.h"
#include "../../core/include/memory/lotus_memory.h"

#include <stdio.h>

Lotus_2D_API* lotus_2D = NULL;

/* LOTUS2D COMPONENT UTILITIES */
void _set_color2D_impl(ubyte2 entity_id, Lotus_Vec3 color) {
    Lotus_Mesh2D* mesh = lotus_ecs_api->get_component(entity_id, LOTUS_MESH2D);
    if (mesh) mesh->color = color;
}


void _set_speed2D_impl(ubyte2 entity_id, f32 speed) {
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (transform) transform->speed = speed;
}

void _set_scale2D_impl(ubyte2 entity_id, Lotus_Vec2 scale) {
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (transform) {
        if (scale.x != 0.0)
            transform->scale.x = scale.x;
        if (scale.y != 0.0)
            transform->scale.y = scale.y;
    }
}

void _set_velocity2D_impl(ubyte2 entity_id, Lotus_Vec2 velocity) {
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (transform) {
        if (velocity.x != 0.0)
            transform->velocity.x = velocity.x;
        if (velocity.y != 0.0)
            transform->velocity.y = velocity.y;
    }
}

void _set_location2D_impl(ubyte2 entity_id, Lotus_Vec2 location) {
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (transform) transform->location = location;
}

void _set_rotation2D_impl(ubyte2 entity_id, f32 rotation) {
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (transform) transform->rotation = rotation;
}


/* LOTUS2D COMPONENT SYSTEMS */
void _transform_system_2D(ubyte2 entity_id) {
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (!transform) return;

    f64 delta_time = lotus_platform_api->get_state()->clock.delta_time;
    transform->location = lotus_add_vec2(transform->location, lotus_scale_vec2(transform->velocity, delta_time));
    transform->model = lotus_mul_mat4(lotus_identity(), lotus_rot_mat4(0, 0, 1, transform->rotation));
    transform->model = lotus_mul_mat4(transform->model, lotus_scale_mat4(transform->scale.x, transform->scale.y, 1.0));
    transform->model = lotus_mul_mat4(transform->model, lotus_trans_mat4(transform->location.x, transform->location.y, 0.0));
}

void _render_system_2D(ubyte2 entity_id) {
    Lotus_Mesh2D* mesh = lotus_ecs_api->get_component(entity_id, LOTUS_MESH2D);
    Lotus_Transform2D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM2D);
    if (!mesh || !transform) return;

    Lotus_Texture2D* texture = lotus_ecs_api->get_component(entity_id, LOTUS_TEXTURE2D);
    if (texture) {
        lotus_graphics_api->GL_API.bind_texture(GL_TEXTURE_2D, texture->id);
    }

    lotus_graphics_api->set_uniform(lotus_graphics_api->get_state()->shader, "u_model", &transform->model);
    lotus_graphics_api->draw_data(mesh->vertexData);
}


ubyte lotus_init_2D(void) {
    lotus_init_core();

    if (!lotus_init_ecs()) return LOTUS_FALSE;  // error: failed to init ECS!

    lotus_2D = lotus_memory_api->alloc(sizeof(Lotus_2D_API), 16);
    if (!lotus_2D) return LOTUS_FALSE;

    lotus_2D->set_color2D = _set_color2D_impl;

    lotus_2D->set_speed2D = _set_speed2D_impl;
    lotus_2D->set_scale2D = _set_scale2D_impl;
    lotus_2D->set_velocity2D = _set_velocity2D_impl;
    lotus_2D->set_rotation2D = _set_rotation2D_impl;
    lotus_2D->set_location2D = _set_location2D_impl;

    lotus_2D->create_circle2D = _create_circle2D_impl;
    lotus_2D->create_triangle2D = _create_triangle2D_impl;
    lotus_2D->create_rectangle2D = _create_rectangle2D_impl;

    lotus_ecs_api->register_component(sizeof(Lotus_Mesh2D), LOTUS_MESH2D);
    lotus_ecs_api->register_system(LOTUS_MESH2D, _render_system_2D);

    lotus_ecs_api->register_component(sizeof(Lotus_Transform2D), LOTUS_TRANSFORM2D);
    lotus_ecs_api->register_system(LOTUS_TRANSFORM2D, _transform_system_2D);

    lotus_ecs_api->register_component(sizeof(Lotus_Texture2D), LOTUS_TEXTURE2D);

    return LOTUS_TRUE;
}

void lotus_shutdown_2D(void) {
    for (ubyte i = 0; i < LOTUS_2D_COMPONENTS; i++) {
        lotus_ecs_api->unregister_component(i);
        lotus_ecs_api->unregister_system(i);
    }
    
    lotus_shutdown_ecs();
    lotus_memory_api->free(lotus_2D);
    lotus_shutdown_core();
}
