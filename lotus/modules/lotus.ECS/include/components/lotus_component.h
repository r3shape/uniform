#pragma once

#include "../lotus_ecs_types.h"

#include "lotus_mesh.h"
#include "lotus_texture.h"
#include "lotus_transform.h"

typedef union Lotus_Component_Data {
    Lotus_Mesh mesh;
    Lotus_Transform transform;
} Lotus_Component_Data;

// tagged component union
struct Lotus_Component {
    Lotus_Component_Data data;
    Lotus_Component_Type type;
};

typedef void (*_add_component_ptr)(void* component_data, Lotus_Entity entity);
typedef void (*_rem_component_ptr)(void* component_data, Lotus_Entity entity);
typedef void (*_set_component_ptr)(void* component_data, Lotus_Component component, Lotus_Entity entity);
typedef Lotus_Component (*_get_component_ptr)(void* component_data, Lotus_Entity entity);

typedef ubyte (*_sys_main_ptr)(void* manager, Lotus_Entity entity);
typedef void (*_sys_fallback_ptr)(void);

typedef struct Lotus_Component_Manager {
    struct component_handler {
        _add_component_ptr add_component;
        _rem_component_ptr rem_component;
        _set_component_ptr set_component;
        _get_component_ptr get_component;
    } component_handler[LOTUS_COMPONENT_TYPES];
    
    struct component_register {
        void* data;
        ubyte2 count;
        ubyte active[LOTUS_ENTITY_MAX];
    } component_register[LOTUS_COMPONENT_TYPES];

    struct component_System {
        ubyte4 id;
        const char* tag;
        _sys_main_ptr system_main;
        _sys_fallback_ptr system_fallback;
    } component_system[LOTUS_COMPONENT_TYPES];
} Lotus_Component_Manager;

LOTUS_API_ENTRY ubyte lotus_register_component(
    Lotus_Component_Manager* manager,
    Lotus_Component_Type type,
    void* data,
    _add_component_ptr add_component,
    _rem_component_ptr rem_component,
    _set_component_ptr set_component,
    _get_component_ptr get_component
);
LOTUS_API_ENTRY ubyte lotus_unregister_component(Lotus_Component_Manager* manager, Lotus_Component_Type type);

LOTUS_API_ENTRY void lotus_add_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity);
LOTUS_API_ENTRY ubyte lotus_has_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity);
LOTUS_API_ENTRY void lotus_rem_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity);
LOTUS_API_ENTRY void lotus_set_component(Lotus_Component_Manager* manager, Lotus_Component component, Lotus_Entity entity);
LOTUS_API_ENTRY Lotus_Component lotus_get_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity);

LOTUS_API_ENTRY ubyte lotus_register_component_system(
    Lotus_Component_Manager* manager,
    Lotus_Component_Type type,
    const char* tag,
    _sys_main_ptr system_main,
    _sys_fallback_ptr system_fallback
);
LOTUS_API_ENTRY ubyte lotus_run_component_system(Lotus_Component_Manager* manager, Lotus_Component_Type type);
LOTUS_API_ENTRY ubyte lotus_unregister_component_system(Lotus_Component_Manager* manager, Lotus_Component_Type type);