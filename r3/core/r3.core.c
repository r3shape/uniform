#include <r3/core/r3.core.h>
#include <r3/modules/pack/include/r3.pack.h>
#include <r3/modules/2D/include/r3.2D.h>
#include <r3/modules/3D/include/r3.3D.h>

_r3_core_api* r3_core = NULL;

u8 r3_init_core(u8 modules) {
    if (r3_core != NULL) return LIBX_TRUE;  // redundant call: core api initialized!

    if (!libx_init(LIBX_ALL)) return LIBX_FALSE;   // error: failed to init Libx!
    
    r3_core = libx->memx.alloc(sizeof(_r3_core_api), 8);
    if (!r3_core) return LIBX_FALSE;    // error: out of memory!
    
    if ((modules & R3_CORE) == R3_CORE) {
        if (!_r3_init_events(&r3_core->events)) return LIBX_FALSE;   // error: failed to init events api!
        if (!_r3_init_input(&r3_core->events, &r3_core->input)) return LIBX_FALSE;   // error: failed to init input api!
        if (!_r3_init_platform(&r3_core->events, &r3_core->input, &r3_core->platform)) return LIBX_FALSE;   // error: failed to init platform api!
        if (!_r3_init_graphics(&r3_core->graphics)) return LIBX_FALSE;
    }
    
    if ((modules & R3_PACK) == R3_PACK) {
        if (!r3_init_pack(r3_core)) return LIBX_FALSE;  // error: failed to init pack api!
    }

    if ((modules & R3_2D) == R3_2D) {
        if (!r3_init_2D()) return LIBX_FALSE;    // error: failed to init 2D api!
    }

    if ((modules & R3_3D) == R3_3D) {
        if (!r3_init_3D()) return LIBX_FALSE;    // error: failed to init 3D api!
    }

    r3_core->modules = modules;
    return LIBX_TRUE;
}

u8 r3_cleanup_core(void) {
    if (!r3_core) return LIBX_TRUE; // redundant call: core api initialized!
    
    u8 result = LIBX_FALSE;

    if ((r3_core->modules & R3_CORE) == R3_CORE) {
        result = _r3_cleanup_platform(&r3_core->platform);
        result = _r3_cleanup_graphics(&r3_core->graphics);
        result = _r3_cleanup_input(&r3_core->input);
        result = _r3_cleanup_events(&r3_core->events);
    }

    if ((r3_core->modules & R3_PACK) == R3_PACK) {
        result = r3_cleanup_pack();
    }

    if ((r3_core->modules & R3_2D) == R3_2D) {
        result = r3_cleanup_2D();
    }
    
    if ((r3_core->modules & R3_3D) == R3_3D) {
        result = r3_cleanup_3D();
    }
    
    libx->memx.dealloc(r3_core);

    libx_cleanup();
    return result;
}
