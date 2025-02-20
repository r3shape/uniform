#include "../include/r3_plug.h"

#include "../../core/include/memory/r3_memory.h"
#include "../../core/include/platform/r3_platform.h"

typedef struct R3_Plug {
    ubyte id;
    char* name;
    R3_DyLib dylib;
    ubyte (*init)(void);
    void (*shutdown)(void);
    void* (*export_api)(void);
} R3_Plug;

R3_Plug_API* r3_plug_api = NULL;
static struct R3_Plug_Manager {
    ubyte plug_count;
    R3_Plug plugs[R3_ENGINE_PLUG_MAX];    // [ could be hashmap... ]--- (o_o)
} internal_plug_manager = {0};

void* _load_plug_impl(char* name, char* path) {
    if (internal_plug_manager.plug_count >= R3_ENGINE_PLUG_MAX) return NULL; // error: plugin max reached!
    
    ubyte plug_id = internal_plug_manager.plug_count++;
    R3_Plug* plug = &internal_plug_manager.plugs[plug_id];
    R3_DyLib* plug_lib = &internal_plug_manager.plugs[plug_id].dylib;
    
    *plug_lib = r3_platform_api->load_library(path, name);
    if (!plug_lib->handle) return NULL;   // error: failed to load plugin library!


    plug->init = r3_platform_api->get_library_symbol(plug_lib, "_plug_init_impl");
    plug->shutdown = r3_platform_api->get_library_symbol(plug_lib, "_plug_shutdown_impl");
    plug->export_api = r3_platform_api->get_library_symbol(plug_lib, "_plug_export_api_impl");
    if (!plug->init || !plug->shutdown || !plug->export_api) {
        r3_platform_api->unload_library(plug_lib);
        internal_plug_manager.plug_count--;
        return NULL;    // error: failed to aquire default plugin symbols!
    }

    if (!plug->init()) {
        r3_platform_api->unload_library(plug_lib);
        internal_plug_manager.plug_count--;
        plug->export_api = NULL;
        plug->shutdown = NULL;
        plug->init = NULL;
        return NULL;    // error: failed to init plugin!
    }

    void* plug_api = plug->export_api();
    if (!plug_api) {
        r3_platform_api->unload_library(plug_lib);
        internal_plug_manager.plug_count--;
        plug->export_api = NULL;
        plug->shutdown = NULL;
        plug->init = NULL;
        return NULL;    // error: failed to export plugin API!
    }
    
    plug->id = plug_id;
    plug->name = name;

    return plug_api;
}

void* _get_plug_impl(char* name) {
    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        if (!strcmp(internal_plug_manager.plugs[i].name, name)) {
            R3_Plug* plug = &internal_plug_manager.plugs[i];
            return (!plug->export_api) ? NULL : plug->export_api();    // error: NULL if plugin not loaded!
        }
    }
    return NULL;    // error: plugin not found!
}

ubyte _unload_plug_impl(char* name) {
    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        if (!strcmp(internal_plug_manager.plugs[i].name, name)) {
            R3_Plug* plug = &internal_plug_manager.plugs[i];
            if (!plug->shutdown) return R3_FALSE;    // error: plugin not loaded!
            plug->shutdown();
            r3_platform_api->unload_library(&plug->dylib);
        
            plug->id = R3_ENGINE_PLUG_MAX;
            plug->name = NULL;
            plug->init = NULL;
            plug->shutdown = NULL;
            plug->export_api = NULL;

            internal_plug_manager.plug_count--;
            return R3_TRUE;
        }
    }
    return R3_FALSE; // error: plugin not found/loaded!
}


ubyte r3_init_plug(void) {
    r3_plug_api = (R3_Plug_API*)r3_memory_api->alloc(sizeof(R3_Plug_API), 16);
    if (!r3_plug_api) return R3_FALSE;

    r3_plug_api->load_plug = _load_plug_impl;
    r3_plug_api->get_plug = _get_plug_impl;
    r3_plug_api->unload_plug = _unload_plug_impl;

    internal_plug_manager.plug_count = 0;
    return R3_TRUE;
}

void r3_shutdown_plug(void) {
    for (ubyte i = 0; i < internal_plug_manager.plug_count; i++) {
        R3_Plug* plug = &internal_plug_manager.plugs[i];
        plug->shutdown();
        r3_platform_api->unload_library(&plug->dylib);
        
        plug->id = R3_ENGINE_PLUG_MAX;
        plug->name = NULL;
        plug->init = NULL;
        plug->shutdown = NULL;
        plug->export_api = NULL;
    }
    internal_plug_manager.plug_count = 0;
    r3_memory_api->free(r3_plug_api);
}
