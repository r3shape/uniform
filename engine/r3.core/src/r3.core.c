#include "../include/r3.core.h"

_r3_core_api* r3_core = NULL;

u8 r3_init_core(void) {
    if (!libx_init_memory()) return LIBX_FALSE;   // error: failed to init memory api!
    if (!libx_init_structs()) return LIBX_FALSE;   // error: failed to init structs api!
    if (!libx_init_math()) return LIBX_FALSE;   // error: failed to init math api!
    if (!libx_init_fileio()) return LIBX_FALSE;   // error: failed to init fileio api!
    
    r3_core = memx->alloc(sizeof(_r3_core_api), 16);
    if (!r3_core) return LIBX_FALSE;    // error: out of memory!
    
    if (!_r3_init_events(&r3_core->events)) return LIBX_FALSE;   // error: failed to init events api!
    if (!_r3_init_input(&r3_core->events, &r3_core->input)) return LIBX_FALSE;   // error: failed to init input api!
    if (!_r3_init_platform(&r3_core->events, &r3_core->input, &r3_core->platform)) return LIBX_FALSE;   // error: failed to init platform api!
    if (!_r3_init_graphics(&r3_core->graphics)) return LIBX_FALSE;

    return LIBX_TRUE;
}

u8 r3_cleanup_core(void) {
    u8 result = _r3_cleanup_platform(&r3_core->platform);
    result = _r3_cleanup_graphics(&r3_core->graphics);
    result = _r3_cleanup_input(&r3_core->input);
    result = _r3_cleanup_events(&r3_core->events);

    memx->dealloc(r3_core);

    libx_cleanup_fileio();
    libx_cleanup_math();
    libx_cleanup_structs();
    libx_cleanup_memory();
    return result;
}
