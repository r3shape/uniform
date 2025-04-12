#include <koncept/modules/pack/include/koncept_pack.h>

typedef struct KC_Package {
    u8 id;
    char* name;
    KC_DLL dll;
    u8 (*init)(void);
    void (*cleanup)(void);
    void* (*export)(void);
} KC_Package;

_koncept_pack_api* koncept_pack = NULL;
_koncept_core_api* core_pack = NULL;
static struct KC_Pack_Manager {
    u8 pack_count;
    KC_Package packs[KC_PACK_MAX];    // [ could be hashmap... ]--- (o_o)
} internal_pack_manager = {0};

void* _load_pack_impl(char* name, char* path) {
    if (internal_pack_manager.pack_count >= KC_PACK_MAX) return NULL; // error: package max reached!
    
    u8 pack_id = internal_pack_manager.pack_count++;
    KC_Package* pack = &internal_pack_manager.packs[pack_id];
    KC_DLL* pack_lib = &internal_pack_manager.packs[pack_id].dll;
    
    *pack_lib = core_pack->platform.load_library(path, name);
    if (!pack_lib->handle) return NULL;   // error: failed to load package library!


    pack->init = core_pack->platform.get_symbol(pack_lib, "_pack_init_impl");
    pack->cleanup = core_pack->platform.get_symbol(pack_lib, "_pack_cleanup_impl");
    pack->export = core_pack->platform.get_symbol(pack_lib, "_pack_export_impl");
    if (!pack->init || !pack->cleanup || !pack->export) {
        core_pack->platform.unload_library(pack_lib);
        internal_pack_manager.pack_count--;
        return NULL;    // error: failed to aquire default package symbols!
    }

    if (!pack->init()) {
        core_pack->platform.unload_library(pack_lib);
        internal_pack_manager.pack_count--;
        pack->export = NULL;
        pack->cleanup = NULL;
        pack->init = NULL;
        return NULL;    // error: failed to init package!
    }

    void* pack_api = pack->export();
    if (!pack_api) {
        core_pack->platform.unload_library(pack_lib);
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
            KC_Package* pack = &internal_pack_manager.packs[i];
            return (!pack->export) ? NULL : pack->export();    // error: NULL if package not loaded!
        }
    }
    return NULL;    // error: package not found!
}

u8 _unload_pack_impl(char* name) {
    for (u8 i = 0; i < internal_pack_manager.pack_count; i++) {
        if (!strcmp(internal_pack_manager.packs[i].name, name)) {
            KC_Package* pack = &internal_pack_manager.packs[i];
            if (!pack->cleanup) return COREX_FALSE;    // error: package not loaded!
            pack->cleanup();
            core_pack->platform.unload_library(&pack->dll);
        
            pack->id = KC_PACK_MAX;
            pack->name = NULL;
            pack->init = NULL;
            pack->cleanup = NULL;
            pack->export = NULL;

            internal_pack_manager.pack_count--;
            return COREX_TRUE;
        }
    }
    return COREX_FALSE; // error: package not found/loaded!
}


u8 koncept_init_pack(void* koncept_core) {
    if (!koncept_core) return COREX_FALSE;   // error: null ptr!
    if (koncept_pack != NULL) return COREX_TRUE;  // redundant call: pack api initialized!

    core_pack = koncept_core;
    koncept_pack = (_koncept_pack_api*)corex->memx.alloc(sizeof(_koncept_pack_api), 8);
    if (!koncept_pack) return COREX_FALSE;

    koncept_pack->load_pack = _load_pack_impl;
    koncept_pack->get_pack = _get_pack_impl;
    koncept_pack->unload_pack = _unload_pack_impl;

    internal_pack_manager.pack_count = 0;
    return COREX_TRUE;
}

u8 koncept_cleanup_pack(void) {
    if (!koncept_pack || !core_pack) return COREX_TRUE;    // reduntant call: pack api not initialized!

    for (u8 i = 0; i < internal_pack_manager.pack_count; i++) {
        KC_Package* pack = &internal_pack_manager.packs[i];
        pack->cleanup();
        core_pack->platform.unload_library(&pack->dll);
        
        pack->id = KC_PACK_MAX;
        pack->name = NULL;
        pack->init = NULL;
        pack->cleanup = NULL;
        pack->export = NULL;
    }
    internal_pack_manager.pack_count = 0;
    corex->memx.dealloc(koncept_pack);
    core_pack = NULL;
    return COREX_TRUE;
}