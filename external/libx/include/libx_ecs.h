#pragma once

#include "libx_def.h"
#include "libx_memory.h"
#include "libx_structs.h"

#define COMPONENT_MAX   (1 << 5)
#define ENTITY_MAX      ((1 << 16) - 1)

typedef struct _libx_ecs_api {
    struct entity_manager {
        u32* queue;
        u32* mask;
        u32 count;
    } entity_manager;

    struct component_manager {
        void** component_storage;
        void** add_component_fptr;
        void** rem_component_fptr;
        void** get_component_fptr;
        u32 count;
    } component_manager;

    u32 (*create_entity)(void);
    void (*destroy_entity)(u32 entity);
    
    u8 (*register_component)(u8 id,
        void* storage, void* add_func,
        void* rem_func, void* get_func);
    u8 (*unregister_component)(u8 id);
    
    /**
     * The `get_entities` function returns an array of all entities that have the specified component attached.
     * `get_entities` iterates over all active entities (n), thus the size of the returned array is: `n * sizeof(u32)`.
     * Any entities that do not have this component will be pushed onto the returned array as the value expanded from the `ENTITY_MAX` directive.
     * Note: The caller of `get_entities` must call `structx->destroy_array()` on the return value to ensure no memory is leaked.
     */
    u32* (*get_entities)(u8 id);
    u8 (*add_component)(u8 id, u32 entity);
    u8 (*rem_component)(u8 id, u32 entity);
    u8 (*get_component)(u8 id, u32 entity, void* component);
    
} _libx_ecs_api;
extern _libx_ecs_api* ecsx;

LIBX_API u8 libx_init_ecs(void);
LIBX_API void libx_cleanup_ecs(void);
