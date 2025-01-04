#include "../include/lotus_plug.h"

#include "../../lotus.CORE/include/platform/lotus_logger.h"

static Lotus_Plug_API internal_plug_api = {0};
static Lotus_Plug_Manager internal_plug_manager = {0};

// plug API implementation
Lotus_Plug_Manager* _plug_initialize_impl(void) {
    lotus_set_log_level(LOTUS_LOG_ERROR);
    internal_plug_manager.plug_count = 0;
    
    return &internal_plug_manager;
}

void _plug_shutdown_impl(Lotus_Platform_API* platform_api) {
    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        Lotus_Plug* plug = &internal_plug_manager.plugs[i];
        plug->shutdown();

        plug->name = NULL;
        plug->shutdown = NULL;
        plug->initialize = NULL;
        
        platform_api->unload_library(&plug->library);
    }; internal_plug_manager.plug_count = 0;
}

Lotus_Plug* _plug_load_impl(Lotus_Platform_API* platform_api, const char* path, const char* name) {
    lotus_set_log_level(LOTUS_LOG_ERROR);

    if (internal_plug_manager.plug_count >= LOTUS_ENGINE_PLUG_MAX) {
        lotus_log_error("Maximum loaded plugs reached");
        return NULL;
    }

    ubyte plug_id = internal_plug_manager.plug_count;
    Lotus_Plug* plug = &internal_plug_manager.plugs[plug_id];
    
    plug->id = plug_id;
    plug->name = name;
    plug->library = platform_api->load_library(path, name);
    if (!plug->library.handle) {
        lotus_log_error("Failed to load plug library: %s", name);
        return NULL;
    }

    lotus_set_log_level(LOTUS_LOG_INFO);
    lotus_log_info("Plug loaded: %s", name);
    lotus_set_log_level(LOTUS_LOG_ERROR);
    
    plug->initialize = platform_api->get_library_symbol(&plug->library, "plug_init");
    if (!plug->initialize) {
        lotus_log_error("Failed to load plug initialization function: %s", name);
        return NULL;
    };

    plug->export_api = platform_api->get_library_symbol(&plug->library, "plug_export_api");
    if (!plug->export_api) {
        lotus_log_error("Failed to load plug API export function: %s", name);
        return NULL;
    }
    
    plug->shutdown = platform_api->get_library_symbol(&plug->library, "plug_shutdown");
    if (!plug->shutdown) {
        lotus_log_error("Failed to load plug shutdown function: %s", name);
        return NULL;
    }

    if (!plug->initialize()) {
        lotus_log_error("Plug initialization failed: %s", name);
        platform_api->unload_library(&plug->library);
        return NULL;
    }

    internal_plug_manager.plug_count++;

    return plug;
}

Lotus_Plug* _plug_get_impl(const char* name) {
    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        if (strcmp(internal_plug_manager.plugs[i].name, name) == 0) {
            return &internal_plug_manager.plugs[i];
        } else return NULL;
    }
    lotus_log_warning("Plug not found: %s", name);
    return NULL;
}

ubyte _plug_unload_impl(Lotus_Platform_API* platform_api, const char* name) {
    lotus_set_log_level(LOTUS_LOG_WARNING);

    Lotus_Plug* plug = internal_plug_api.get_plug(name);
    if (plug) {
        plug->shutdown();
        platform_api->unload_library(&plug->library);

        plug->name = NULL;
        plug->shutdown = NULL;
        plug->export_api = NULL;
        plug->initialize = NULL;
        
        internal_plug_manager.plug_count--;
        
        lotus_set_log_level(LOTUS_LOG_INFO);
        lotus_log_info("Plug unloaded: %s", name);
        return LOTUS_TRUE;
    }
    
    lotus_log_warning("Plug not found for unloading: %s", name);
    return LOTUS_FALSE;
}


Lotus_Plug_API* lotus_init_plug(void) {
    internal_plug_api = (Lotus_Plug_API) {
        .initialize = _plug_initialize_impl,
        .shutdown = _plug_shutdown_impl,
        .load_plug = _plug_load_impl,
        .get_plug = _plug_get_impl,
        .unload_plug = _plug_unload_impl
    };

    return &internal_plug_api;
}
