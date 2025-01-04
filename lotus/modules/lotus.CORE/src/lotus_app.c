#include "../include/application/lotus_app.h"

#include "../include/platform/lotus_logger.h"

// internal application instance pointer
static Lotus_Application_API internal_application_api;
static Lotus_Application internal_application_instance;

// default engine event callbacks
ubyte quit_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_EVENT_APP_QUIT) {
        internal_application_instance.state.running = LOTUS_FALSE;
        return LOTUS_FALSE;
    }; return LOTUS_TRUE;
}


// application API implementation
ubyte _application_create_window_impl(const char* title, ubyte4 size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!internal_application_instance.state.running) return LOTUS_FALSE;
    internal_application_instance.resource.window = internal_application_instance.resource.platform_api->create_window((title != NULL) ? title : internal_application_instance.info.name, size[0], size[1]);

    if (!internal_application_instance.resource.platform_api->create_gl_context(&internal_application_instance.resource.window)) {
        lotus_log_fatal("Failed to create GL Context!");
        return LOTUS_FALSE;
    }
    
    return LOTUS_TRUE;
}

void _application_destroy_window_impl(void) {
    if (!internal_application_instance.state.running) return;
    internal_application_instance.resource.platform_api->destroy_window(&internal_application_instance.resource.window);
    internal_application_instance.resource.platform_api->destroy_gl_context(&internal_application_instance.resource.window);
}

ubyte _application_run_impl(void) {
    ubyte result = LOTUS_FALSE;
    if (!internal_application_instance.state.running) return result;
    
    lotus_set_log_level(LOTUS_LOG_FATAL);
    while (internal_application_instance.state.running) {
        if (internal_application_instance.callbacks[LOTUS_APPLICATION_MIDFRAME_EVENT] == NULL) {
            lotus_log_fatal("Lotus Application Missing Mid-Frame Callback!");
            return result;
        }
        
        // preframe logic
        internal_application_instance.resource.platform_api->poll_events();
        if (internal_application_instance.callbacks[LOTUS_APPLICATION_PREFRAME_EVENT]) {
            result = lotus_push_event((Lotus_Event){0}, LOTUS_APPLICATION_PREFRAME_EVENT);
        }
        
        if (internal_application_instance.callbacks[LOTUS_APPLICATION_FIXEDFRAME_EVENT]) {
            result = lotus_push_event((Lotus_Event){0}, LOTUS_APPLICATION_FIXEDFRAME_EVENT);
        }
        
        result = lotus_push_event((Lotus_Event){0}, LOTUS_APPLICATION_MIDFRAME_EVENT);
        internal_application_instance.resource.graphics_api->draw_end();
        
        // run component systems
        if (internal_application_instance.state.current_scene) {
            result = lotus_run_component_system(&internal_application_instance.state.current_scene->component_manager, LOTUS_TRANSFORM_COMPONENT);
        }

        // postframe logic
        if (internal_application_instance.callbacks[LOTUS_APPLICATION_POSTFRAME_EVENT]) {
            result = lotus_push_event((Lotus_Event){0}, LOTUS_APPLICATION_POSTFRAME_EVENT);
        }
        internal_application_instance.resource.platform_api->swap_buffers(&internal_application_instance.resource.window);
        lotus_update_input(0);

    }; internal_application_api.shutdown();

    return result;
}

sbyte _application_create_scene_impl(const char* scene_name) {
    if (internal_application_instance.state.scene_count + 1 > LOTUS_ENGINE_SCENE_MAX) { return -1; }

    ubyte scene_id = internal_application_instance.state.scene_count++;
    internal_application_instance.resource.scenes[scene_id] = lotus_init_scene(scene_id, scene_name);
    internal_application_instance.state.current_scene = internal_application_instance.resource.scenes[scene_id];
    internal_application_api.init_ecs(scene_id);

    return scene_id;
}

ubyte _application_set_scene_impl(ubyte scene_id) {
    if (scene_id > LOTUS_ENGINE_SCENE_MAX) return LOTUS_FALSE;
    internal_application_instance.state.current_scene = internal_application_instance.resource.scenes[scene_id];
    return LOTUS_TRUE;
}

Lotus_Scene* _application_get_scene_impl(ubyte scene_id) {
    if (scene_id > LOTUS_ENGINE_SCENE_MAX) return NULL;
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    return scene;
}

void _application_destroy_scene_impl(ubyte scene_id) {
    if (scene_id > LOTUS_ENGINE_SCENE_MAX || internal_application_instance.state.scene_count - 1 < 0) { return; }
    
    internal_application_api.shutdown_ecs(scene_id);
    lotus_destroy_scene(internal_application_instance.resource.scenes[scene_id]);
    internal_application_instance.state.current_scene = NULL;
    internal_application_instance.state.scene_count--;
}

Lotus_Application* _application_initialize_impl(const char* app_name, ubyte4 window_size[2]) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    if (!app_name) app_name = "Lotus Application";

    internal_application_instance.info.name = app_name;
    internal_application_instance.info.window_size[0] = window_size[0];
    internal_application_instance.info.window_size[1] = window_size[1];
    
    internal_application_instance.state.scene_count = 0;
    internal_application_instance.state.callback_count = 0;
    internal_application_instance.state.running = LOTUS_TRUE;
    
    internal_application_instance.resource.allocator = lotus_make_allocator(LOTUS_DEFAULT_APPLICATION_HEAP_SIZE);

    internal_application_instance.resource.platform_api = lotus_init_platform();
    if (!internal_application_instance.resource.platform_api) {
        lotus_log_fatal("Failed to initialize platform api!");
        return NULL;
    }

    // register default event callbacks
    // TODO: resize event
    lotus_register_event_callback(LOTUS_EVENT_APP_QUIT, quit_callback);

    if (!_application_create_window_impl(app_name, window_size)) {
        lotus_log_fatal("Failed to create application window!");
        return NULL;
    }
    
    internal_application_instance.resource.graphics_api = lotus_init_graphics();
    if (!internal_application_instance.resource.graphics_api) {
        lotus_log_fatal("Failed to initialize graphics api!");
        return NULL;
    }

    internal_application_instance.resource.plug_api = lotus_init_plug();
    if (!internal_application_instance.resource.plug_api) {
        lotus_log_fatal("Failed to initialize plug api!");
        return NULL;
    }

    internal_application_instance.resource.plug_manager = internal_application_instance.resource.plug_api->initialize();
    if (!internal_application_instance.resource.plug_manager) {
        lotus_log_fatal("Failed to initialize plug manager!");
        return NULL;
    }

    return &internal_application_instance;
}

void _application_shutdown_impl(void) {
    internal_application_instance.state.running = LOTUS_FALSE;
    internal_application_instance.info.name = "NULL";

    internal_application_instance.resource.plug_api->shutdown(internal_application_instance.resource.platform_api);

    for (ubyte i = 0; i < internal_application_instance.state.scene_count; i++) {
        internal_application_api.destroy_scene(i);
    }; internal_application_instance.state.scene_count = 0;

    for (ubyte i = 0; i < LOTUS_APPLICATION_EVENTS; i++) {
        lotus_unregister_event_callback(i, internal_application_instance.callbacks[i]);
    }; internal_application_instance.state.callback_count = 0;

    internal_application_api.destroy_window();
    
    internal_application_instance.resource.plug_api->shutdown(internal_application_instance.resource.platform_api);
    internal_application_instance.resource.plug_api = NULL;

    internal_application_instance.resource.graphics_api->shutdown();
    internal_application_instance.resource.graphics_api = NULL;

    internal_application_instance.resource.platform_api->shutdown();
    internal_application_instance.resource.platform_api = NULL;
}

ubyte _application_set_callback_impl(Lotus_Application_Event event, Lotus_Event_Callback callback) {
    if (internal_application_instance.callbacks[event]) return LOTUS_FALSE; // event callback exists
    internal_application_instance.callbacks[event] = callback;
    internal_application_instance.state.callback_count++;
    return lotus_register_event_callback(event, callback);
}


// ECS wrapper implementation
ubyte _application_init_ecs_impl(ubyte scene_id) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL || internal_application_instance.resource.graphics_api != NULL) {
        return lotus_init_ecs(internal_application_instance.resource.graphics_api, &scene->entity_namager, &scene->component_manager);
    }
    return LOTUS_FALSE;
}

void _application_shutdown_ecs_impl(ubyte scene_id) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        lotus_shutdown_ecs(&scene->entity_namager, &scene->component_manager);
    }
}

Lotus_Entity _application_make_entity_impl(ubyte scene_id) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_make_entity(&scene->entity_namager);
    }
    return LOTUS_ENTITY_MAX+1;
}

ubyte _application_kill_entity_impl(ubyte scene_id, Lotus_Entity entity) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_kill_entity(&scene->entity_namager, entity);
    }
    return LOTUS_FALSE;
}

ubyte _application_register_component_impl(
    ubyte scene_id,
    Lotus_Component_Type type,
    void* data,
    _add_component_ptr add_component,
    _rem_component_ptr rem_component,
    _set_component_ptr set_component,
    _get_component_ptr get_component
) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_register_component(&scene->component_manager, type, data, add_component, rem_component, set_component, get_component);
    }
    return LOTUS_FALSE;
}

ubyte _application_unregister_component_impl(ubyte scene_id, Lotus_Component_Type type) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_unregister_component(&scene->component_manager, type);
    }
    return LOTUS_FALSE;
}

void _application_add_component_impl(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_add_component(&scene->component_manager, type, entity);
    }
}

ubyte _application_has_component_impl(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_has_component(&scene->component_manager, type, entity);
    }
    return LOTUS_FALSE;
}

void _application_rem_component_impl(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        lotus_rem_component(&scene->component_manager, type, entity);
    }
}

void _application_set_component_impl(ubyte scene_id, Lotus_Component component, Lotus_Entity entity) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        lotus_set_component(&scene->component_manager, component, entity);
    }
}

Lotus_Component _application_get_component_impl(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity) {
    Lotus_Scene* scene = internal_application_instance.resource.scenes[scene_id];
    if (scene != NULL) {
        return lotus_get_component(&scene->component_manager, type, entity);
    }

    return (Lotus_Component){ .type = LOTUS_COMPONENT_MAX+1 };
}


Lotus_Application_API* lotus_init_application(void) {
    internal_application_api = (Lotus_Application_API) {
        .initialize = _application_initialize_impl,
        .shutdown = _application_shutdown_impl,
        
        .create_window = _application_create_window_impl,
        .destroy_window = _application_destroy_window_impl,
        
        .create_scene = _application_create_scene_impl,
        .get_scene = _application_get_scene_impl,
        .set_scene = _application_set_scene_impl,
        .destroy_scene = _application_destroy_scene_impl,
        
        .set_callback = _application_set_callback_impl,
        .run = _application_run_impl,

        .init_ecs = _application_init_ecs_impl,
        .shutdown_ecs = _application_shutdown_ecs_impl,
        .make_entity = _application_make_entity_impl,
        .kill_entity = _application_kill_entity_impl,
        .register_component = _application_register_component_impl,
        .unregister_component = _application_unregister_component_impl,
        .add_component = _application_add_component_impl,
        .has_component = _application_has_component_impl,
        .rem_component = _application_rem_component_impl,
        .set_component = _application_set_component_impl,
        .get_component = _application_get_component_impl
    };

    return &internal_application_api;
}
