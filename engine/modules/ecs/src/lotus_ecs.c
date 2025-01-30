#include "../include/lotus_ecs.h"

Lotus_ECS_API* lotus_ecs_api = NULL;
static struct internal_ecs_state {
    Lotus_Entity_Manager em;
    Lotus_Component_Manager cm;
} internal_ecs_state = {0};

ubyte2 _create_entity_impl(void) {
    ubyte2 entity_id = internal_ecs_state.em.count++;
    
    internal_ecs_state.em.entities[entity_id].id = entity_id;
    internal_ecs_state.em.entities[entity_id].mask = 0;
    
    return entity_id;
}

void _destroy_entity_impl(ubyte2 entity_id) {
    if (entity_id < 0 || entity_id >= internal_ecs_state.em.count) return; // error: invalid entity!

    for (ubyte component_id = 0; component_id < LOTUS_COMPONENT_MAX; component_id++) {
        if (internal_ecs_state.em.entities[entity_id].mask & (1U << component_id)) {
            lotus_ecs_api->rem_component(entity_id, component_id);
        }
    }

    internal_ecs_state.em.entities[entity_id].id = LOTUS_ENTITY_MAX;
    internal_ecs_state.em.entities[entity_id].mask = 0;
}


void _register_component_impl(ubyte4 stride, ubyte component_id) {
    internal_ecs_state.cm.components[component_id].count = 0;
    internal_ecs_state.cm.components[component_id].stride = stride;
    internal_ecs_state.cm.components[component_id].data = lotus_memory_api->alloc(LOTUS_ENTITY_MAX * stride, 16);
    internal_ecs_state.cm.count++; 
}

void _unregister_component_impl(ubyte component_id) {
    if (component_id < 0 || component_id >= LOTUS_COMPONENT_MAX) return; // error: invalid component!

    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return; // error: component not registered!

    for (ubyte2 i = 0; i < internal_ecs_state.em.count; i++) {
        lotus_ecs_api->rem_component(i, component_id);
    }

    lotus_memory_api->free(component->data);
    component->data = NULL;
    component->stride = 0;
    component->count = 0;

    internal_ecs_state.cm.count--;
}


void _register_system_impl(ubyte component_id, Lotus_Component_System system) {
    if (component_id < 0 || component_id >= LOTUS_COMPONENT_MAX) return; // error: invalid component!
    internal_ecs_state.cm.systems[component_id] = system;
}


void _unregister_system_impl(ubyte component_id) {
    if (component_id < 0 || component_id >= LOTUS_COMPONENT_MAX) return; // error: invalid component!
    internal_ecs_state.cm.systems[component_id] = NULL;
}

void _run_system_impl(ubyte component_id) {
    Lotus_Component_System system = internal_ecs_state.cm.systems[component_id];
    if (!system) return;

    for (ubyte2 i = 0; i < internal_ecs_state.em.count; i++) {
        if (!(internal_ecs_state.em.entities[i].mask & (1U << component_id))) continue;
        system(i);
    }
}


void* _add_component_impl(ubyte2 entity_id, ubyte component_id) {
    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return NULL;  // error: component not registered!

    internal_ecs_state.em.entities[entity_id].mask |= (1U << component_id);

    return (void*)((ubyte*)component->data + (entity_id * component->stride));
}

void* _get_component_impl(ubyte2 entity_id, ubyte component_id) {
    if (!(internal_ecs_state.em.entities[entity_id].mask & (1U << component_id))) {
        return NULL;
    }
    
    Lotus_Component* component = &internal_ecs_state.cm.components[component_id];
    if (!component->data) return NULL;  // error: component not registered!

    return (void*)((ubyte*)component->data + (entity_id * component->stride));
}

void _rem_component_impl(ubyte2 entity_id, ubyte component_id) {
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
    if (entity_id >= internal_ecs_state.em.count) return 0; // error: invalid entity!
    return (internal_ecs_state.em.entities[entity_id].mask & (1U << component_id)) ? LOTUS_TRUE : LOTUS_FALSE;
}


ubyte lotus_init_ecs(void) {
    lotus_ecs_api = (Lotus_ECS_API*)lotus_memory_api->alloc(sizeof(Lotus_ECS_API), 16);
    if (!lotus_ecs_api) return LOTUS_FALSE;

    lotus_ecs_api->create_entity = _create_entity_impl;
    lotus_ecs_api->destroy_entity = _destroy_entity_impl;
    
    lotus_ecs_api->register_component = _register_component_impl;
    lotus_ecs_api->unregister_component = _unregister_component_impl;
    
    lotus_ecs_api->register_system = _register_system_impl;
    lotus_ecs_api->unregister_system = _unregister_system_impl;
    lotus_ecs_api->run_system = _run_system_impl;
    
    lotus_ecs_api->add_component = _add_component_impl;
    lotus_ecs_api->get_component = _get_component_impl;
    lotus_ecs_api->rem_component = _rem_component_impl;
    lotus_ecs_api->has_component = _has_component_impl;

    return LOTUS_TRUE;
}

void lotus_shutdown_ecs(void) {
    if (!lotus_ecs_api) return;

    for (ubyte i = 0; i < LOTUS_COMPONENT_MAX; i++) {
        _unregister_component_impl(i);
    }

    memset(&internal_ecs_state.em, 0, sizeof(Lotus_Entity_Manager));
    memset(&internal_ecs_state.cm, 0, sizeof(Lotus_Component_Manager));

    lotus_memory_api->free(lotus_ecs_api);
}


