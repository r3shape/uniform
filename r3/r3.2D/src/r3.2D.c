#include <r3/r3.2D/include/r3.2D.h>

_r3_2D_api* r3_2D = NULL;
_r3_core_api* core_2D = NULL;

u8 r3_init_2D(void) {
    if (r3_2D != NULL) return LIBX_TRUE;  // redundant call: 2D api initialized!

    r3_2D = memx->alloc(sizeof(_r3_2D_api), 8);
    if (!r3_2D) return LIBX_FALSE;  // error: failed to allocate 2D api! out of memory!

    if (!_r3_init_shape2D(r3_2D)) return LIBX_FALSE;    // error: failed to init shape2D api!
    
    if (!_r3_register_component2D()) return LIBX_FALSE;    // error: failed to register 2D components!

    return LIBX_TRUE;
}

u8 r3_cleanup_2D(void) {
    if (!r3_2D || !core_2D) return LIBX_TRUE;  // redumdamt call: 2D api not initialized!

    u8 result = LIBX_TRUE;
    if (!_r3_unregister_component2D()) result = LIBX_FALSE;    // error: failed to unregister 2D components!

    memx->dealloc(r3_2D);
    return result;
}
