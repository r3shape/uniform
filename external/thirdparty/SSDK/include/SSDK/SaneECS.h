#ifndef __SANEECS_H__
#define __SANEECS_H__

#include <include/SSDK/globals.h>


#define COMPONENT_MAX   (1 << 5)
#define SYSTEM_MAX      (1 << 3)
#define ENTITY_MAX      ((1 << 16) - 1)

typedef byte (*COMPONENT_ADD_FPTR)(u32 entity);
typedef byte (*COMPONENT_REM_FPTR)(u32 entity);
typedef byte (*COMPONENT_GET_FPTR)(u32 entity, void* component);
typedef byte (*COMPONENT_SYSTEM_FPTR)(u32 entity);

typedef struct SaneECS {
    SaneModule module;

    u32 (*entities)(none);
    u32 (*createEntity)(void);
    u32 (*createEntityWith)(byte count, byte* ids);
    void (*destroyEntity)(u32 entity);
    
    byte (*registerComponent)(
        u8 id, void* storage,
        COMPONENT_ADD_FPTR addFunc,
        COMPONENT_REM_FPTR remFunc,
        COMPONENT_GET_FPTR getFunc);
    byte (*unregisterComponent)(u8 id);
    byte (*unregisterComponents)(none);
    
    byte (*registerSystem)(u8 id, cstr name, void* system);
    byte (*unregisterSystem)(u8 id, cstr name);
    byte (*unregisterSystems)(u8 id);
    
    /**
     * The `get_entities` function returns an array of all entities that have the specified component attached.
     * `get_entities` iterates over all active entities (n), thus the size of the returned array is: `n * sizeof(u32)`.
     * Any entities that do not have this component will be pushed onto the returned array as the value expanded from the `ENTITY_MAX` directive.
     * Note: The caller of `get_entities` must call `structx->destroy_array()` on the return value to ensure no memory is leaked.
     */
    byte (*runSystems)(u8 id);
    u32* (*getEntities)(u8 id);
    byte (*runSystem)(u8 id, cstr name);
    byte (*addComponent)(u8 id, u32 entity);
    byte (*remComponent)(u8 id, u32 entity);
    byte (*hasComponent)(u8 id, u32 entity);
    byte (*getComponent)(u8 id, u32 entity, void* component);
    byte init;
} SaneECS;
extern SaneECS* saneECS;

SSDK_FUNC byte ssdkInitECS(none);
SSDK_FUNC none ssdkExitECS(none);

#endif  // __SANEECS_H__