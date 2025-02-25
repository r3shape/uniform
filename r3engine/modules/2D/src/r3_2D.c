#include "../include/r3_2D.h"
#include "../include/r3_shapes2D.h"
#include "../../core/include/memory/r3_memory.h"

#include <stdio.h>

R3_2D_API* r3_2D = NULL;

/* R32D COMPONENT UTILITIES */
void _set_color2D_impl(ubyte2 entity_id, R3_Vec3 color) {
    R3_Mesh2D* mesh = r3_ecs_api->get_component(entity_id, R3_MESH2D);
    if (mesh) mesh->color = color;
}


void _set_speed2D_impl(ubyte2 entity_id, f32 speed) {
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (transform) transform->speed = speed;
}

void _set_scale2D_impl(ubyte2 entity_id, R3_Vec2 scale) {
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (transform) {
        if (scale.x != 0.0)
            transform->scale.x = scale.x;
        if (scale.y != 0.0)
            transform->scale.y = scale.y;
    }
}

void _set_velocity2D_impl(ubyte2 entity_id, R3_Vec2 velocity) {
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (transform) {
        if (velocity.x != 0.0)
            transform->velocity.x = velocity.x;
        if (velocity.y != 0.0)
            transform->velocity.y = velocity.y;
    }
}

void _set_location2D_impl(ubyte2 entity_id, R3_Vec2 location) {
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (transform) transform->location = location;
}

void _set_rotation2D_impl(ubyte2 entity_id, f32 rotation) {
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (transform) transform->rotation = rotation;
}


/* R32D COMPONENT SYSTEMS */
void _transform_system_2D(ubyte2 entity_id) {
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (!transform) return;

    f64 delta_time = r3_platform_api->get_state()->clock.delta_time;
    transform->location = r3_add_vec2(transform->location, r3_scale_vec2(transform->velocity, delta_time));
    transform->model = r3_mul_mat4(r3_identity(), r3_rot_mat4(0, 0, 1, transform->rotation));
    transform->model = r3_mul_mat4(transform->model, r3_scale_mat4(transform->scale.x, transform->scale.y, 1.0));
    transform->model = r3_mul_mat4(transform->model, r3_trans_mat4(transform->location.x, transform->location.y, 0.0));
}

void _render_system_2D(ubyte2 entity_id) {
    R3_Mesh2D* mesh = r3_ecs_api->get_component(entity_id, R3_MESH2D);
    R3_Transform2D* transform = r3_ecs_api->get_component(entity_id, R3_TRANSFORM2D);
    if (!mesh || !transform) return;

    R3_Texture2D* texture = r3_ecs_api->get_component(entity_id, R3_TEXTURE2D);
    if (texture) {
        r3_graphics_api->GL_API.bind_texture(GL_TEXTURE_2D, texture->id);
    }

    R3_Material* material = r3_ecs_api->get_component(entity_id, R3_MATERIAL);
    if (material && material->shader) {
        r3_graphics_api->set_shader(material->shader);
        r3_graphics_api->set_uniform(material->shader, "u_material.shine", &material->shine);
        r3_graphics_api->set_uniform(material->shader, "u_material.ambient", &material->ambient);
        r3_graphics_api->set_uniform(material->shader, "u_material.diffuse", &material->diffuse);
        r3_graphics_api->set_uniform(material->shader, "u_material.specular", &material->specular);
    }
    
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_view", &r3_get_camera()->view);
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_model", &transform->model);
    r3_graphics_api->draw_data(mesh->vertexData);
}


ubyte r3_init_2D(void) {
    r3_init_core();

    if (!r3_init_ecs()) return R3_FALSE;  // error: failed to init ECS!

    r3_2D = r3_memory_api->alloc(sizeof(R3_2D_API), 16);
    if (!r3_2D) return R3_FALSE;

    r3_2D->set_color2D = _set_color2D_impl;

    r3_2D->set_speed2D = _set_speed2D_impl;
    r3_2D->set_scale2D = _set_scale2D_impl;
    r3_2D->set_velocity2D = _set_velocity2D_impl;
    r3_2D->set_rotation2D = _set_rotation2D_impl;
    r3_2D->set_location2D = _set_location2D_impl;

    r3_2D->create_quad2D = _create_quad2D_impl;
    r3_2D->create_circle2D = _create_circle2D_impl;
    r3_2D->create_triangle2D = _create_triangle2D_impl;

    r3_ecs_api->register_component(sizeof(R3_Mesh2D), R3_MESH2D);
    r3_ecs_api->register_system(R3_MESH2D, _render_system_2D);
    
    r3_ecs_api->register_component(sizeof(R3_Material), R3_MATERIAL);
    
    r3_ecs_api->register_component(sizeof(R3_Texture2D), R3_TEXTURE2D);

    r3_ecs_api->register_component(sizeof(R3_Transform2D), R3_TRANSFORM2D);
    r3_ecs_api->register_system(R3_TRANSFORM2D, _transform_system_2D);

    return R3_TRUE;
}

void r3_shutdown_2D(void) {
    for (ubyte i = 0; i < R3_2D_COMPONENTS; i++) {
        r3_ecs_api->unregister_component(i);
        r3_ecs_api->unregister_system(i);
    }
    
    r3_shutdown_ecs();
    r3_memory_api->free(r3_2D);
    r3_shutdown_core();
}
