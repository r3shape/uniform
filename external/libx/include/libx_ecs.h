#pragma once

#include "libx_def.h"
#include "libx_memory.h"
#include "libx_structs.h"

#define COMPONENT_MAX   (1 << 5)
#define SYSTEM_MAX      (1 << 3)
#define ENTITY_MAX      ((1 << 16) - 1)

typedef u8 (*COMPONENT_ADD_FPTR)(u32 entity);
typedef u8 (*COMPONENT_REM_FPTR)(u32 entity);
typedef u8 (*COMPONENT_GET_FPTR)(u32 entity, void* component);
typedef u8 (*COMPONENT_SYSTEM_FPTR)(u32 entity);

typedef struct _libx_ecs_api {
    struct entity_manager {
        u32* queue;
        u32* mask;
        u32 count;
    } entity_manager;

    struct component_manager {
        Hash_Array** component_system;
        void** component_storage;
        void** add_component_fptr;
        void** rem_component_fptr;
        void** get_component_fptr;
        u32 count;
    } component_manager;

    u32 (*create_entity)(void);
    void (*destroy_entity)(u32 entity);
    
    u8 (*register_component)(
        u8 id, void* storage,
        COMPONENT_ADD_FPTR add_func,
        COMPONENT_REM_FPTR rem_func,
        COMPONENT_GET_FPTR get_func);
    u8 (*unregister_component)(u8 id);

    u8 (*register_system)(u8 id, cstr name, void* system);
    u8 (*unregister_system)(u8 id, cstr name);
    u8 (*unregister_systems)(u8 id);
    
    /**
     * The `get_entities` function returns an array of all entities that have the specified component attached.
     * `get_entities` iterates over all active entities (n), thus the size of the returned array is: `n * sizeof(u32)`.
     * Any entities that do not have this component will be pushed onto the returned array as the value expanded from the `ENTITY_MAX` directive.
     * Note: The caller of `get_entities` must call `structx->destroy_array()` on the return value to ensure no memory is leaked.
     */
    u32* (*get_entities)(u8 id);
    u8 (*run_systems)(u8 id);
    u8 (*run_system)(u8 id, cstr name);
    u8 (*add_component)(u8 id, u32 entity);
    u8 (*rem_component)(u8 id, u32 entity);
    u8 (*has_component)(u8 id, u32 entity);
    u8 (*get_component)(u8 id, u32 entity, void* component);
    
} _libx_ecs_api;
extern _libx_ecs_api* ecsx;

LIBX_API u8 libx_init_ecs(void);
LIBX_API void libx_cleanup_ecs(void);
