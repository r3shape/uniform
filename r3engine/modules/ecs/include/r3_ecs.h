#pragma once

#include "../../core/include/r3_core_defines.h"
#include "../../core/include/memory/r3_memory.h"
#include "../../core/include/utility/r3_array.h"

#define R3_ENTITY_MAX        ((1U << 16) - 1)
#define R3_COMPONENT_MAX     (1U << 5)
#define R3_PREFAB_MAX        (1U << 6)

typedef struct R3_Entity {
    ubyte2 id;
    ubyte4 mask;    // bitmask for attached components
} R3_Entity;

typedef struct R3_Entity_Manager {
    ubyte2 entity_count;
    ubyte2 free_count;
    ubyte2 free_pool[R3_ENTITY_MAX];
    R3_Entity entities[R3_ENTITY_MAX];
} R3_Entity_Manager;

typedef struct R3_Component {
    ubyte4 stride;
    ubyte2 count;
    void* data;
} R3_Component;

// TODO: consider a component system structure to hook into core event system (e.g. on damage event, run health systems)
typedef void (*R3_Component_System)(ubyte2 entity_id);
typedef struct R3_Component_Manager {
    ubyte2 count;
    R3_Component components[R3_COMPONENT_MAX];
    R3_Component_System systems[R3_COMPONENT_MAX];
} R3_Component_Manager;

typedef struct R3_Prefab {
    ubyte2 count;
    ubyte2 extends;
    ubyte ids[R3_PREFAB_MAX];
} R3_Prefab;

typedef struct R3_Prefab_Manager {
    ubyte2 count;
    R3_Prefab prefabs[R3_PREFAB_MAX];
} R3_Prefab_Manager;

typedef struct R3_ECS_API {
    ubyte2 (*create_entity)(void);
    void (*destroy_entity)(ubyte2 entity_id);

    void (*register_component)(ubyte4 stride, ubyte component_id);
    void (*unregister_component)(ubyte component_id);
    void* (*add_component)(ubyte2 entity_id, ubyte component_id);
    void* (*get_component)(ubyte2 entity_id, ubyte component_id);
    void (*rem_component)(ubyte2 entity_id, ubyte component_id);
    ubyte (*has_component)(ubyte2 entity_id, ubyte component_id);

    void (*register_system)(ubyte component_id, R3_Component_System system);
    void (*unregister_system)(ubyte component_id);
    void (*run_system)(ubyte component_id);

    ubyte2 (*create_prefab)(const ubyte* component_ids, ubyte component_count);
    ubyte2 (*create_prefab_ex)(const ubyte* component_ids, ubyte component_count, ubyte2 parent_id);
    void (*destroy_prefab)(ubyte2 prefab_id);
    ubyte2 (*instance_prefab)(ubyte2 prefab_id);
} R3_ECS_API;

extern R3_ECS_API* r3_ecs_api;

R3_API ubyte r3_init_ecs(void);
R3_API void r3_shutdown_ecs(void);