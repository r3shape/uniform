#include "../include/r3_3D.h"
#include "../include/r3_shapes3D.h"
#include "../../core/include/memory/r3_memory.h"

#include <stdio.h>

R3_3D_API* r3_3D = NULL;

/* R33D COMPONENT UTILITIES */
void _set_color3D_impl(ubyte2 entity_id, R3_Vec3 color) {
    R3_Mesh3D* mesh = r3_ecs_api->get_component(entity_id, R3_MESH3D);
    if (mesh) mesh->color = color;
}


void _set_speed3D_impl(ubyte2 entity_id, f32 speed) {
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (transform) transform->speed = speed;
}

void _set_scale3D_impl(ubyte2 entity_id, R3_Vec3 scale) {
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (transform) {
        if (scale.x != 0.0)
            transform->scale.x = scale.x;
        if (scale.y != 0.0)
            transform->scale.y = scale.y;
        if (scale.z != 0.0)
            transform->scale.z = scale.z;
    }
}

void _set_velocity3D_impl(ubyte2 entity_id, R3_Vec3 velocity) {
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (transform) {
        if (velocity.x != 0.0)
            transform->velocity.x = velocity.x;
        if (velocity.y != 0.0)
            transform->velocity.y = velocity.y;
        if (velocity.z != 0.0)
            transform->velocity.z = velocity.z;
    }
}

void _set_location3D_impl(ubyte2 entity_id, R3_Vec3 location) {
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (transform) transform->location = location;
}

void _set_rotation3D_impl(ubyte2 entity_id, R3_Vec3 rotation) {
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (transform) transform->rotation = rotation;
}


/* R33D COMPONENT SYSTEMS */
void _transform_system_3D(ubyte2 entity_id) {
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (!transform) return;

    f64 delta_time = r3_platform_api->get_state()->clock.delta_time;
    transform->location = r3_add_vec3(transform->location, r3_scale_vec3(transform->velocity, delta_time));
    transform->model = r3_mul_mat4(r3_identity(), r3_rotx_mat4(transform->rotation.x));
    transform->model = r3_mul_mat4(transform->model, r3_roty_mat4(transform->rotation.y));
    transform->model = r3_mul_mat4(transform->model, r3_rotz_mat4(transform->rotation.z));
    transform->model = r3_mul_mat4(transform->model, r3_scale_mat4(transform->scale.x, transform->scale.y, transform->scale.z));
    transform->model = r3_mul_mat4(transform->model, r3_trans_mat4(transform->location.x, transform->location.y, transform->location.z));
}

void _render_system_3D(ubyte2 entity_id) {
    R3_Mesh3D* mesh = r3_ecs_api->get_component(entity_id, R3_MESH3D);
    R3_Transform3D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM3D);
    if (!mesh || !transform) return;
    
    R3_Texture2D* texture = r3_ecs_api->get_component(entity_id, R3_TEXTURE2D);
    if (texture) {
        r3_graphics_api->GL_API.bind_texture(GL_TEXTURE_2D, texture->id);
    }
    
    R3_Material* material = r3_ecs_api->get_component(entity_id, R3_MATERIAL);
    if (material && material->shader) {
        r3_graphics_api->set_shader(material->shader);  // u_projection is set to hashmap
    }
    
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_view", &r3_get_camera()->view);   // u_view is set to hashmap
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_model", &transform->model);   // u_model is set to hashmap
    r3_graphics_api->draw_data(mesh->vertexData);   // u_model, u_view, u_projection are sent to GPU
}


ubyte r3_init_3D(void) {
    r3_init_core();

    if (!r3_init_ecs()) return R3_FALSE;  // error: failed to init ECS!

    r3_3D = r3_memory_api->alloc(sizeof(R3_3D_API), 16);
    if (!r3_3D) return R3_FALSE;

    r3_3D->set_color3D = _set_color3D_impl;
    
    r3_3D->set_speed3D = _set_speed3D_impl;
    r3_3D->set_scale3D = _set_scale3D_impl;
    r3_3D->set_velocity3D = _set_velocity3D_impl;
    r3_3D->set_rotation3D = _set_rotation3D_impl;
    r3_3D->set_location3D = _set_location3D_impl;
    
    r3_3D->create_quad3D = _create_quad3D_impl;
    r3_3D->create_cube3D = _create_cube3D_impl;

    r3_ecs_api->register_component(sizeof(R3_Mesh3D), R3_MESH3D);
    r3_ecs_api->register_system(R3_MESH3D, _render_system_3D);
    
    r3_ecs_api->register_component(sizeof(R3_Material), R3_MATERIAL);
    
    r3_ecs_api->register_component(sizeof(R3_Texture2D), R3_TEXTURE2D);

    r3_ecs_api->register_component(sizeof(R3_Transform3D), R3_TRANSFORM3D);
    r3_ecs_api->register_system(R3_TRANSFORM3D, _transform_system_3D);

    return R3_TRUE;
}

void r3_shutdown_3D(void) {
    for (ubyte i = 0; i < R3_3D_COMPONENTS; i++) {
        r3_ecs_api->unregister_component(i);
        r3_ecs_api->unregister_system(i);
    }
    
    r3_shutdown_ecs();
    r3_memory_api->free(r3_3D);
    r3_shutdown_core();
}
