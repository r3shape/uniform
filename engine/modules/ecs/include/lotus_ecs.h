#pragma once

#include "../../core/include/lotus_core_defines.h"
#include "../../core/include/memory/lotus_memory.h"
#include "../../core/include/utility/lotus_array.h"

#define LOTUS_ENTITY_MAX        ((1U << 16) - 1)
#define LOTUS_COMPONENT_MAX     (1U << 5)

typedef struct Lotus_Entity {
    ubyte2 id;
    ubyte4 mask;    // bitmask for attached components
} Lotus_Entity;

typedef struct Lotus_Entity_Manager {
    ubyte2 count;
    Lotus_Entity entities[LOTUS_ENTITY_MAX];
} Lotus_Entity_Manager;

typedef struct Lotus_Component {
    ubyte4 stride;
    ubyte2 count;
    void* data;
} Lotus_Component;

// TODO: consider a component system structure to hook into core event system (e.g. on damage event, run health systems)
typedef void (*Lotus_Component_System)(ubyte2 entity_id);
typedef struct Lotus_Component_Manager {
    ubyte2 count;
    Lotus_Component components[LOTUS_COMPONENT_MAX];
    Lotus_Component_System systems[LOTUS_COMPONENT_MAX];
} Lotus_Component_Manager;

typedef struct Lotus_ECS_API {
    ubyte2 (*create_entity)(void);
    void (*destroy_entity)(ubyte2 entity_id);

    void (*register_component)(ubyte4 stride, ubyte component_id);
    void (*unregister_component)(ubyte component_id);
    
    void (*register_system)(ubyte component_id, Lotus_Component_System system);
    void (*unregister_system)(ubyte component_id);
    void (*run_system)(ubyte component_id);
    
    void* (*add_component)(ubyte2 entity_id, ubyte component_id);
    void* (*get_component)(ubyte2 entity_id, ubyte component_id);
    void (*rem_component)(ubyte2 entity_id, ubyte component_id);
    ubyte (*has_component)(ubyte2 entity_id, ubyte component_id);
} Lotus_ECS_API;

extern Lotus_ECS_API* lotus_ecs_api;

LOTUS_API ubyte lotus_init_ecs(void);
LOTUS_API void lotus_shutdown_ecs(void);