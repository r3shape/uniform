#include "../include/components/lotus_component.h"

ubyte lotus_register_component(
    Lotus_Component_Manager* manager,
    Lotus_Component_Type type,
    void* data,
    _add_component_ptr add_component,
    _rem_component_ptr rem_component,
    _set_component_ptr set_component,
    _get_component_ptr get_component
){
    manager->component_register[type].data = data;
    manager->component_handler[type].add_component = add_component;
    manager->component_handler[type].rem_component = rem_component;
    manager->component_handler[type].set_component = set_component;
    manager->component_handler[type].get_component = get_component;

    return 1;
}

ubyte lotus_unregister_component(Lotus_Component_Manager* manager, Lotus_Component_Type type) {
    manager->component_register[type].data = (void*)0;
    manager->component_handler[type].add_component = (void*)0;
    manager->component_handler[type].rem_component = (void*)0;
    manager->component_handler[type].get_component = (void*)0;

    return 1;
}

void lotus_add_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity) {
    if (type > LOTUS_COMPONENT_MAX || entity > LOTUS_ENTITY_MAX || entity < 0) return;
    manager->component_register[type].active[entity] = 1;
    manager->component_handler[type].add_component(
        manager->component_register[type].data,
        entity
    );
    manager->component_register[type].count++;
}

ubyte lotus_has_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity) {
    if (type > LOTUS_COMPONENT_MAX || entity > LOTUS_ENTITY_MAX || entity < 0) return 0;
    return manager->component_register[type].active[entity];
}

void lotus_rem_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity) {
    if (!lotus_has_component(manager, type, entity)) return;
    manager->component_register[type].active[entity] = 0;
    manager->component_handler[type].rem_component(
        manager->component_register[type].data,
        entity
    );
    manager->component_register[type].count--;
}

void lotus_set_component(Lotus_Component_Manager* manager, Lotus_Component component, Lotus_Entity entity) {
    if (!lotus_has_component(manager, component.type, entity)) return;
    manager->component_handler[component.type].set_component(
        manager->component_register[component.type].data,
        component,
        entity
    );
}

Lotus_Component lotus_get_component(Lotus_Component_Manager* manager, Lotus_Component_Type type, Lotus_Entity entity) {
    if (!lotus_has_component(manager, type, entity)) return (Lotus_Component){0};
    return manager->component_handler[type].get_component(
        manager->component_register[type].data,
        entity
    );
}

ubyte lotus_register_component_system(
    Lotus_Component_Manager* manager,
    Lotus_Component_Type type,
    const char* tag,
    _sys_main_ptr system_main,
    _sys_fallback_ptr system_fallback
) {
    manager->component_system[type].id = type;
    manager->component_system[type].tag = tag;
    manager->component_system[type].system_main = system_main;
    manager->component_system[type].system_fallback = system_fallback;
    
    return LOTUS_TRUE;
}

ubyte lotus_unregister_component_system(Lotus_Component_Manager* manager, Lotus_Component_Type type) {
    manager->component_system[type].id = LOTUS_COMPONENT_MAX;
    manager->component_system[type].tag = NULL;
    manager->component_system[type].system_main = NULL;
    manager->component_system[type].system_fallback = NULL;

    return LOTUS_TRUE;
}

ubyte lotus_run_component_system(Lotus_Component_Manager* manager, Lotus_Component_Type type) {
    if (!manager->component_system[type].system_main) return LOTUS_FALSE;
    for (ubyte2 i = 0; i < LOTUS_ENTITY_MAX; i++) {
        if (!manager->component_register[type].active[i]) continue;
        if (!manager->component_system[type].system_main(manager, i)) {
            if (!manager->component_system[type].system_fallback) continue;
            else manager->component_system[type].system_fallback();
        }
    }
    manager->component_system[type].system_fallback = NULL;
    return LOTUS_TRUE;  // ran system for all active entities
}

