#pragma once

#include "components/lotus_component.h"

#include "systems/lotus_transform_sys.h"

#include "lotus_ecs_types.h"

typedef struct Lotus_Entity_Manager {
    ubyte2 next;
    ubyte2 count;
    ubyte alive[LOTUS_ENTITY_MAX];
} Lotus_Entity_Manager;

LOTUS_API_ENTRY ubyte lotus_init_ecs(void* graphics_api, Lotus_Entity_Manager* e_manager, Lotus_Component_Manager* c_manager);
LOTUS_API_ENTRY void lotus_shutdown_ecs(Lotus_Entity_Manager* e_manager, Lotus_Component_Manager* c_manager);

LOTUS_API_ENTRY Lotus_Entity lotus_make_entity(Lotus_Entity_Manager* manager);
LOTUS_API_ENTRY ubyte lotus_kill_entity(Lotus_Entity_Manager* manager, Lotus_Entity entity);
