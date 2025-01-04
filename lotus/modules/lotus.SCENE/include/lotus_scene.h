#pragma once

#include "../../lotus.CORE/include/platform/lotus_memory.h"
#include "../../lotus.CORE/include/platform/lotus_logger.h"

#include "../../lotus.ECS/include/lotus_ecs.h"

#define LOTUS_DEFAULT_SCENE_HEAP_SIZE 1024 * 1024   // 1kb

typedef struct Lotus_Scene_Info {
    ubyte id;
    const char* name;
} Lotus_Scene_Info;

typedef struct Lotus_Scene {
    Lotus_Scene_Info info;
    Lotus_Allocator allocator;
    Lotus_Entity_Manager entity_namager;
    Lotus_Component_Manager component_manager;
} Lotus_Scene;

LOTUS_API_ENTRY Lotus_Scene* lotus_init_scene(ubyte id, const char* name);
LOTUS_API_ENTRY void lotus_destroy_scene(Lotus_Scene* scene);
