#include "../include/lotus_ecs.h"

static Lotus_Mesh_Data internal_mesh_data = {0};
static Lotus_Transform_Data internal_transform_data = {0};

ubyte lotus_init_ecs(void* graphics_api, Lotus_Entity_Manager* e_manager, Lotus_Component_Manager* c_manager) {
    e_manager->next = 0;
    e_manager->count = 0;

    if (_lotus_init_mesh_data(&internal_mesh_data, graphics_api)) {
        lotus_register_component(
            c_manager, LOTUS_MESH_COMPONENT, &internal_mesh_data,
            _lotus_add_mesh,
            _lotus_rem_mesh,
            _lotus_set_mesh,
            _lotus_get_mesh
        );
    }
    
    if (_lotus_init_transform_data(&internal_transform_data)) {
        lotus_register_component(
            c_manager, LOTUS_TRANSFORM_COMPONENT, &internal_transform_data,
            _lotus_add_transform,
            _lotus_rem_transform,
            _lotus_set_transform,
            _lotus_get_transform
        );
        lotus_register_component_system(
            c_manager, LOTUS_TRANSFORM_COMPONENT, "Lotus Transform System",
            _lotus_transform_system,
            NULL
        );
    }

    return LOTUS_TRUE;
}

void lotus_shutdown_ecs(Lotus_Entity_Manager* e_manager, Lotus_Component_Manager* c_manager) {
    e_manager->next = 0;
    e_manager->count = 0;

    _lotus_destroy_mesh_data(&internal_mesh_data);
    _lotus_destroy_transform_data(&internal_transform_data);
    lotus_unregister_component(c_manager, LOTUS_MESH_COMPONENT);
    lotus_unregister_component(c_manager, LOTUS_TRANSFORM_COMPONENT);
}

Lotus_Entity lotus_make_entity(Lotus_Entity_Manager* manager) {
    if (manager->count + 1 > LOTUS_ENTITY_MAX) return LOTUS_ENTITY_MAX + 1;
    Lotus_Entity entity = manager->next++;
    manager->count++;
    manager->alive[entity] = 1;
    return entity;
}

ubyte lotus_kill_entity(Lotus_Entity_Manager* manager, Lotus_Entity entity) {
    if (manager->count - 1 < 0) return 0;
    manager->next--;
    manager->count--;
    manager->alive[entity] = 0;
    return 1;
}
