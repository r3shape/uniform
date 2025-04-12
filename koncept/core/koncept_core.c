#include <koncept/core/koncept_core.h>
#include <koncept/modules/pack/include/koncept_pack.h>
#include <koncept/modules/2D/include/koncept_2D.h>
#include <koncept/modules/3D/include/koncept_3D.h>

_koncept_core_api* koncept_core = NULL;

u8 koncept_init_core(u8 modules) {
    if (koncept_core != NULL) return COREX_TRUE;  // redundant call: core api initialized!

    if (!corex_init(COREX_ALL)) return COREX_FALSE;   // error: failed to init Libx!
    
    koncept_core = corex->memx.alloc(sizeof(_koncept_core_api), 8);
    if (!koncept_core) return COREX_FALSE;    // error: out of memory!
    
    if ((modules & KC_CORE) == KC_CORE) {
        if (!_koncept_init_events(&koncept_core->events)) return COREX_FALSE;   // error: failed to init events api!
        if (!_koncept_init_input(&koncept_core->events, &koncept_core->input)) return COREX_FALSE;   // error: failed to init input api!
        if (!_koncept_init_platform(&koncept_core->events, &koncept_core->input, &koncept_core->platform)) return COREX_FALSE;   // error: failed to init platform api!
        if (!_koncept_init_graphics(&koncept_core->graphics)) return COREX_FALSE;
    }
    
    if ((modules & KC_PACK) == KC_PACK) {
        if (!koncept_init_pack(koncept_core)) return COREX_FALSE;  // error: failed to init pack api!
    }

    if ((modules & KC_2D) == KC_2D) {
        if (!koncept_init_2D()) return COREX_FALSE;    // error: failed to init 2D api!
    }

    if ((modules & KC_3D) == KC_3D) {
        if (!koncept_init_3D()) return COREX_FALSE;    // error: failed to init 3D api!
    }

    koncept_core->modules = modules;
    return COREX_TRUE;
}

u8 koncept_cleanup_core(void) {
    if (!koncept_core) return COREX_TRUE; // redundant call: core api initialized!
    
    u8 result = COREX_FALSE;

    if ((koncept_core->modules & KC_CORE) == KC_CORE) {
        result = _koncept_cleanup_platform(&koncept_core->platform);
        result = _koncept_cleanup_graphics(&koncept_core->graphics);
        result = _koncept_cleanup_input(&koncept_core->input);
        result = _koncept_cleanup_events(&koncept_core->events);
    }

    if ((koncept_core->modules & KC_PACK) == KC_PACK) {
        result = koncept_cleanup_pack();
    }

    if ((koncept_core->modules & KC_2D) == KC_2D) {
        result = koncept_cleanup_2D();
    }
    
    if ((koncept_core->modules & KC_3D) == KC_3D) {
        result = koncept_cleanup_3D();
    }
    
    corex->memx.dealloc(koncept_core);

    corex_cleanup();
    return result;
}
