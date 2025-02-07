#include "../include/lotus_3D.h"
#include "../include/lotus_shapes3D.h"
#include "../../core/include/memory/lotus_memory.h"

Lotus_3D_API* lotus_3D = NULL;

/* LOTUS3D COMPONENT UTILITIES */
void _set_color3D_impl(ubyte2 entity_id, Lotus_Vec3 color) {
    Lotus_Mesh3D* mesh = lotus_ecs_api->get_component(entity_id, LOTUS_MESH3D);
    if (mesh) mesh->color = color;
}


void _set_speed3D_impl(ubyte2 entity_id, f32 speed) {
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (transform) transform->speed = speed;
}

void _set_scale3D_impl(ubyte2 entity_id, Lotus_Vec3 scale) {
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (transform) {
        if (scale.x != 0.0)
            transform->scale.x = scale.x;
        if (scale.y != 0.0)
            transform->scale.y = scale.y;
        if (scale.z != 0.0)
            transform->scale.z = scale.z;
    }
}

void _set_velocity3D_impl(ubyte2 entity_id, Lotus_Vec3 velocity) {
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (transform) {
        if (velocity.x != 0.0)
            transform->velocity.x = velocity.x;
        if (velocity.y != 0.0)
            transform->velocity.y = velocity.y;
        if (velocity.z != 0.0)
            transform->velocity.z = velocity.z;
    }
}

void _set_location3D_impl(ubyte2 entity_id, Lotus_Vec3 location) {
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (transform) transform->location = location;
}

void _set_rotation3D_impl(ubyte2 entity_id, Lotus_Vec3 rotation) {
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (transform) transform->rotation = rotation;
}


/* LOTUS3D COMPONENT SYSTEMS */
void _transform_system_3D(ubyte2 entity_id) {
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (!transform) return;

    f64 delta_time = lotus_platform_api->get_state()->clock.delta_time;
    transform->location = lotus_add_vec3(transform->location, lotus_scale_vec3(transform->velocity, delta_time));
    transform->model = lotus_mul_mat4(lotus_identity(), lotus_rotx_mat4(transform->rotation.x));
    transform->model = lotus_mul_mat4(transform->model, lotus_roty_mat4(transform->rotation.y));
    transform->model = lotus_mul_mat4(transform->model, lotus_rotz_mat4(transform->rotation.z));
    transform->model = lotus_mul_mat4(transform->model, lotus_scale_mat4(transform->scale.x, transform->scale.y, transform->scale.z));
    transform->model = lotus_mul_mat4(transform->model, lotus_trans_mat4(transform->location.x, transform->location.y, transform->location.z));
}

void _render_system_3D(ubyte2 entity_id) {
    Lotus_Mesh3D* mesh = lotus_ecs_api->get_component(entity_id, LOTUS_MESH3D);
    Lotus_Transform3D* transform = lotus_ecs_api->get_component(entity_id, LOTUS_TRANSFORM3D);
    if (!mesh || !transform) return;

    lotus_graphics_api->set_uniform(lotus_graphics_api->get_state()->shader, "u_model", &transform->model);
    lotus_graphics_api->draw_data(mesh->vertexData);
}

ubyte lotus_init_3D(void) {
    lotus_init_core();

    if (!lotus_init_ecs()) return LOTUS_FALSE;  // error: failed to init ECS!

    lotus_3D = lotus_memory_api->alloc(sizeof(Lotus_3D_API), 16);
    if (!lotus_3D) return LOTUS_FALSE;

    lotus_3D->set_color3D = _set_color3D_impl;
    
    lotus_3D->set_speed3D = _set_speed3D_impl;
    lotus_3D->set_scale3D = _set_scale3D_impl;
    lotus_3D->set_velocity3D = _set_velocity3D_impl;
    lotus_3D->set_rotation3D = _set_rotation3D_impl;
    lotus_3D->set_location3D = _set_location3D_impl;
    
    lotus_3D->create_cube3D = _create_cube3D_impl;

    lotus_ecs_api->register_component(sizeof(Lotus_Mesh3D), LOTUS_MESH3D);
    lotus_ecs_api->register_system(LOTUS_MESH3D, _render_system_3D);

    lotus_ecs_api->register_component(sizeof(Lotus_Transform3D), LOTUS_TRANSFORM3D);
    lotus_ecs_api->register_system(LOTUS_TRANSFORM3D, _transform_system_3D);

    return LOTUS_TRUE;
}

void lotus_shutdown_3D(void) {
    for (ubyte i = 0; i < LOTUS_3D_COMPONENTS; i++) {
        lotus_ecs_api->unregister_component(i);
        lotus_ecs_api->unregister_system(i);
    }
    
    lotus_shutdown_ecs();
    lotus_memory_api->free(lotus_3D);
    lotus_shutdown_core();
}
