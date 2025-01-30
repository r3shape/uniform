#include "../include/lotus_ecs.h"

#include <stdio.h>

Lotus_ECS_API* lotus_ecs_api = NULL;
static struct internal_ecs_state {
    Lotus_Component_Manager cm;
    Lotus_Entity_Manager em;
    Lotus_Prefab_Manager pm;
} internal_ecs_state = {0};

ubyte2 _create_entity_impl(void) {
    ubyte2 entity_id;
    
    if (internal_ecs_state.em.free_count > 0) {
        entity_id = internal_ecs_state.em.free_pool[--internal_ecs_state.em.free_count];
    } else {
        entity_id = internal_ecs_state.em.entity_count++;
    }
    
    internal_ecs_state.em.entities[entity_id].id = entity_id;
    internal_ecs_state.em.entities[entity_id].mask = 0;
    
    return entity_id;
}

void _destroy_entity_impl(ubyte2 entity_id) {
    if (entity_id >= internal_ecs_state.em.entity_count) return; // error: invalid entity!

    internal_ecs_state.em.free_pool[internal_ecs_state.em.free_count++] = entity_id;

    for (ubyte component_id = 0; component_id < LOTUS_COMPONENT_MAX; component_id++) {
        if (internal_ecs_state.em.entities[entity_id].mask & (1U << component_id)) {
            lotus_ecs_api->rem_component(entity_id, component_id);
        }
    }

    internal_ecs_state.em.entities[entity_id].id = LOTUS_ENTITY_MAX;
    internal_ecs_state.em.entities[entity_id].mask = 0;
}


void _register_component_impl(ubyte4 stride, ubyte component_id) {
    if (!stride ||
    component_id >= LOTUS_COMPONENT_MAX  ||
    (internal_ecs_state.cm.count + 1) >= LOTUS_COMPONENT_MAX
    ) return;
    internal_ecs_state.cm.components[component_id].count = 0;
    internal_ecs_state.cm.components[component_id].stride = stride;
    internal_ecs_state.cm.components[component_id].data = lotus_memory_api->alloc(LOTUS_ENTITY_MAX * stride, 16);
    internal_ecs_state.cm.count++; 
}

void _unregister_component_impl(ubyte component_id) {
    if (component_id >= LOTUS_COMPONENT_MAX) return; // error: invalid component!

    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return; // error: component not registered!

    for (ubyte2 i = 0; i < internal_ecs_state.em.entity_count; i++) {
        lotus_ecs_api->rem_component(i, component_id);
    }

    lotus_memory_api->free(component->data);
    component->data = NULL;
    component->stride = 0;
    component->count = 0;

    internal_ecs_state.cm.count--;
}

void* _add_component_impl(ubyte2 entity_id, ubyte component_id) {
    if (entity_id >= internal_ecs_state.em.entity_count ||
        component_id >= internal_ecs_state.cm.count ||
        component_id >= LOTUS_COMPONENT_MAX ||
        entity_id >= LOTUS_ENTITY_MAX
        ) return NULL; // error: invalid entity/component!
    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return NULL;  // error: component not registered!

    internal_ecs_state.em.entities[entity_id].mask |= (1U << component_id);

    return (void*)((ubyte*)component->data + (entity_id * component->stride));
}

void* _get_component_impl(ubyte2 entity_id, ubyte component_id) {
    if (entity_id >= internal_ecs_state.em.entity_count ||
        component_id >= internal_ecs_state.cm.count ||
        component_id >= LOTUS_COMPONENT_MAX ||
        entity_id >= LOTUS_ENTITY_MAX
        ) return NULL; // error: invalid entity/component!
    if (!(internal_ecs_state.em.entities[entity_id].mask & (1U << component_id))) {
        return NULL;
    }
    
    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return NULL;  // error: component not registered!

    return (void*)((ubyte*)component->data + (entity_id * component->stride));
}

void _rem_component_impl(ubyte2 entity_id, ubyte component_id) {
    if (entity_id >= internal_ecs_state.em.entity_count ||
        component_id >= internal_ecs_state.cm.count ||
        component_id >= LOTUS_COMPONENT_MAX ||
        entity_id >= LOTUS_ENTITY_MAX
        ) return; // error: invalid entity/component!
    if (!(internal_ecs_state.em.entities[entity_id].mask & (1U << component_id))) {
        return;
    }

    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return;

    internal_ecs_state.em.entities[entity_id].mask &= ~(1U << component_id);
    
    void* component_data = (void*)((ubyte*)component->data + (entity_id * component->stride));
    memset(component_data, 0, component->stride);
}

ubyte _has_component_impl(ubyte2 entity_id, ubyte component_id) {
    if (entity_id >= internal_ecs_state.em.entity_count ||
        component_id >= internal_ecs_state.cm.count ||
        component_id >= LOTUS_COMPONENT_MAX ||
        entity_id >= LOTUS_ENTITY_MAX
        ) return LOTUS_FALSE; // error: invalid entity/component!
    return (internal_ecs_state.em.entities[entity_id].mask & (1U << component_id)) ? LOTUS_TRUE : LOTUS_FALSE;
}


void _register_system_impl(ubyte component_id, Lotus_Component_System system) {
    if (!system || component_id >= LOTUS_COMPONENT_MAX || component_id >= internal_ecs_state.cm.count) return; // error: invalid component!
    internal_ecs_state.cm.systems[component_id] = system;
}

void _unregister_system_impl(ubyte component_id) {
    if (component_id >= LOTUS_COMPONENT_MAX || component_id >= internal_ecs_state.cm.count) return; // error: invalid component!
    internal_ecs_state.cm.systems[component_id] = NULL;
}

void _run_system_impl(ubyte component_id) {
    if (!system || component_id >= LOTUS_COMPONENT_MAX || component_id >= internal_ecs_state.cm.count) return; // error: invalid component!
    Lotus_Component_System system = internal_ecs_state.cm.systems[component_id];

    for (ubyte2 i = 0; i < internal_ecs_state.em.entity_count; i++) {
        if (!(internal_ecs_state.em.entities[i].mask & (1U << component_id))) continue;
        system(i);
    }
}


ubyte2 _create_prefab_impl(const ubyte* component_ids, ubyte component_count) {
    if (!component_ids || component_count > LOTUS_COMPONENT_MAX) return LOTUS_PREFAB_MAX;   // error: value error!
    if (internal_ecs_state.pm.count >= LOTUS_PREFAB_MAX) return LOTUS_PREFAB_MAX;     // error: max prefabs reached!

    ubyte2 prefab_id = internal_ecs_state.pm.count++;
    Lotus_Prefab* prefab = &internal_ecs_state.pm.prefabs[prefab_id];

    prefab->count = component_count;
    for (ubyte i = 0; i < component_count; i++) {
        prefab->ids[i] = component_ids[i];
    }

    return prefab_id;
}

ubyte2 _create_prefab_ex_impl(const ubyte* component_ids, ubyte component_count, ubyte2 parent_id) {
    if (!component_ids || component_count > LOTUS_COMPONENT_MAX) return LOTUS_PREFAB_MAX;   // error: value error!
    if (internal_ecs_state.pm.count >= LOTUS_PREFAB_MAX) return LOTUS_PREFAB_MAX;     // error: max prefabs reached!
    ubyte2 prefab_id = internal_ecs_state.pm.count++;
    Lotus_Prefab* prefab = &internal_ecs_state.pm.prefabs[prefab_id];

    prefab->count = component_count;
    prefab->extends = parent_id;

    if (parent_id < internal_ecs_state.pm.count) {
        Lotus_Prefab* parent = &internal_ecs_state.pm.prefabs[parent_id];
        if (!parent) return LOTUS_PREFAB_MAX;
        
        for (ubyte i = 0; i < parent->count; i++) {
            prefab->ids[i] = parent->ids[i];
        }
        prefab->count += parent->count;
    }

    for (ubyte i = 0; i < component_count; i++) {
        prefab->ids[i] = component_ids[i];
    }

    return prefab_id;
}

void _destroy_prefab_impl(ubyte2 prefab_id) {
    if (prefab_id >= internal_ecs_state.pm.count) return;   // error: invalid prefab!

    for (ubyte2 i = prefab_id; i < internal_ecs_state.pm.count - 1; i++) {
        internal_ecs_state.pm.prefabs[i] = internal_ecs_state.pm.prefabs[i + 1];
    }

    internal_ecs_state.pm.count--;
}

ubyte2 _instance_prefab_impl(ubyte2 prefab_id) {
    if (prefab_id >= internal_ecs_state.pm.count) return LOTUS_PREFAB_MAX;  // error: invalid prefab!

    ubyte2 entity_id = lotus_ecs_api->create_entity();
    Lotus_Prefab* prefab = &internal_ecs_state.pm.prefabs[prefab_id];
    if (!prefab) return LOTUS_PREFAB_MAX;

    for (ubyte i = 0; i < prefab->count; i++) {
        ubyte component_id = prefab->ids[i];
        lotus_ecs_api->add_component(entity_id, component_id);
    }

    return entity_id;
}


ubyte lotus_init_ecs(void) {
    lotus_ecs_api = (Lotus_ECS_API*)lotus_memory_api->alloc(sizeof(Lotus_ECS_API), 16);
    if (!lotus_ecs_api) return LOTUS_FALSE;

    lotus_ecs_api->create_entity = _create_entity_impl;
    lotus_ecs_api->destroy_entity = _destroy_entity_impl;
    
    lotus_ecs_api->register_component = _register_component_impl;
    lotus_ecs_api->unregister_component = _unregister_component_impl;
    lotus_ecs_api->add_component = _add_component_impl;
    lotus_ecs_api->get_component = _get_component_impl;
    lotus_ecs_api->rem_component = _rem_component_impl;
    lotus_ecs_api->has_component = _has_component_impl;
    
    lotus_ecs_api->register_system = _register_system_impl;
    lotus_ecs_api->unregister_system = _unregister_system_impl;
    lotus_ecs_api->run_system = _run_system_impl;
    
    lotus_ecs_api->create_prefab = _create_prefab_impl;
    lotus_ecs_api->create_prefab_ex = _create_prefab_ex_impl;
    lotus_ecs_api->destroy_prefab = _destroy_prefab_impl;
    lotus_ecs_api->instance_prefab = _instance_prefab_impl;

    return LOTUS_TRUE;
}

void lotus_shutdown_ecs(void) {
    if (!lotus_ecs_api) return;

    for (ubyte i = 0; i < LOTUS_COMPONENT_MAX; i++) {
        lotus_ecs_api->unregister_component(i);
    }

    memset(&internal_ecs_state.em, 0, sizeof(Lotus_Entity_Manager));
    memset(&internal_ecs_state.cm, 0, sizeof(Lotus_Component_Manager));

    lotus_memory_api->free(lotus_ecs_api);
}

