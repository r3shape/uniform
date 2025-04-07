#include <r3/r3.core/include/r3.core.h>
#include <r3/r3.pack/include/r3.pack.h>
#include <r3/r3.2D/include/r3.2D.h>
#include <r3/r3.3D/include/r3.3D.h>

_r3_core_api* r3_core = NULL;

u8 r3_init_core(u8 modules) {
    if (r3_core != NULL) return LIBX_TRUE;  // redundant call: core api initialized!

    if (!libx_init_memory()) return LIBX_FALSE;   // error: failed to init memory api!
    if (!libx_init_structs()) return LIBX_FALSE;   // error: failed to init structs api!
    if (!libx_init_math()) return LIBX_FALSE;   // error: failed to init math api!
    if (!libx_init_fileio()) return LIBX_FALSE;   // error: failed to init fileio api!
    
    r3_core = memx->alloc(sizeof(_r3_core_api), 8);
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
        if (!libx_init_ecs()) return LIBX_FALSE;        // error: failed to init ecs api!
        if (!r3_init_2D()) return LIBX_FALSE;    // error: failed to init 2D api!
    }

    if ((modules & R3_3D) == R3_3D) {
        if (!libx_init_ecs()) return LIBX_FALSE;        // error: failed to init ecs api!
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
        libx_cleanup_ecs();
    }
    
    if ((r3_core->modules & R3_3D) == R3_3D) {
        result = r3_cleanup_3D();
        libx_cleanup_ecs();
    }
    
    memx->dealloc(r3_core);

    libx_cleanup_fileio();
    libx_cleanup_math();
    libx_cleanup_structs();
    libx_cleanup_memory();
    return result;
}
