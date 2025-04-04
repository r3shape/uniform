#include <r3/r3.pack/include/r3.pack.h>
#include <r3/r3.core/include/r3.core.h>

typedef struct R3_Package {
    u8 id;
    char* name;
    R3_DLL dll;
    u8 (*init)(void);
    void (*cleanup)(void);
    void* (*export)(void);
} R3_Package;

_r3_pack_api* r3_pack = NULL;
_r3_core_api* core_api = NULL;
static struct R3_Pack_Manager {
    u8 pack_count;
    R3_Package packs[R3_PACK_MAX];    // [ could be hashmap... ]--- (o_o)
} internal_pack_manager = {0};

void* _load_pack_impl(char* name, char* path) {
    if (internal_pack_manager.pack_count >= R3_PACK_MAX) return NULL; // error: package max reached!
    
    u8 pack_id = internal_pack_manager.pack_count++;
    R3_Package* pack = &internal_pack_manager.packs[pack_id];
    R3_DLL* pack_lib = &internal_pack_manager.packs[pack_id].dll;
    
    *pack_lib = core_api->platform.load_library(path, name);
    if (!pack_lib->handle) return NULL;   // error: failed to load package library!


    pack->init = core_api->platform.get_symbol(pack_lib, "_pack_init_impl");
    pack->cleanup = core_api->platform.get_symbol(pack_lib, "_pack_cleanup_impl");
    pack->export = core_api->platform.get_symbol(pack_lib, "_pack_export_impl");
    if (!pack->init || !pack->cleanup || !pack->export) {
        core_api->platform.unload_library(pack_lib);
        internal_pack_manager.pack_count--;
        return NULL;    // error: failed to aquire default package symbols!
    }

    if (!pack->init()) {
        core_api->platform.unload_library(pack_lib);
        internal_pack_manager.pack_count--;
        pack->export = NULL;
        pack->cleanup = NULL;
        pack->init = NULL;
        return NULL;    // error: failed to init package!
    }

    void* pack_api = pack->export();
    if (!pack_api) {
        core_api->platform.unload_library(pack_lib);
        internal_pack_manager.pack_count--;
        pack->export = NULL;
        pack->cleanup = NULL;
        pack->init = NULL;
        return NULL;    // error: failed to export package API!
    }
    
    pack->id = pack_id;
    pack->name = name;

    return pack_api;
}

void* _get_pack_impl(char* name) {
    for (u8 i = 0; i < internal_pack_manager.pack_count; i++) {
        if (!strcmp(internal_pack_manager.packs[i].name, name)) {
            R3_Package* pack = &internal_pack_manager.packs[i];
            return (!pack->export) ? NULL : pack->export();    // error: NULL if package not loaded!
        }
    }
    return NULL;    // error: package not found!
}

u8 _unload_pack_impl(char* name) {
    for (u8 i = 0; i < internal_pack_manager.pack_count; i++) {
        if (!strcmp(internal_pack_manager.packs[i].name, name)) {
            R3_Package* pack = &internal_pack_manager.packs[i];
            if (!pack->cleanup) return LIBX_FALSE;    // error: package not loaded!
            pack->cleanup();
            core_api->platform.unload_library(&pack->dll);
        
            pack->id = R3_PACK_MAX;
            pack->name = NULL;
            pack->init = NULL;
            pack->cleanup = NULL;
            pack->export = NULL;

            internal_pack_manager.pack_count--;
            return LIBX_TRUE;
        }
    }
    return LIBX_FALSE; // error: package not found/loaded!
}


u8 r3_init_pack(void* r3_core) {
    if (!r3_core) return LIBX_FALSE;   // error: null ptr!

    core_api = r3_core;
    r3_pack = (_r3_pack_api*)memx->alloc(sizeof(_r3_pack_api), 16);
    if (!r3_pack) return LIBX_FALSE;

    r3_pack->load_pack = _load_pack_impl;
    r3_pack->get_pack = _get_pack_impl;
    r3_pack->unload_pack = _unload_pack_impl;

    internal_pack_manager.pack_count = 0;
    return LIBX_TRUE;
}

void r3_cleanup_pack(void) {
    for (u8 i = 0; i < internal_pack_manager.pack_count; i++) {
        R3_Package* pack = &internal_pack_manager.packs[i];
        pack->cleanup();
        core_api->platform.unload_library(&pack->dll);
        
        pack->id = R3_PACK_MAX;
        pack->name = NULL;
        pack->init = NULL;
        pack->cleanup = NULL;
        pack->export = NULL;
    }
    internal_pack_manager.pack_count = 0;
    memx->dealloc(r3_pack);
    core_api = NULL;
}