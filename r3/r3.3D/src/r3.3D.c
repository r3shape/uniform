#include <r3/r3.3D/include/r3.3D.h>

_r3_3D_api* r3_3D = NULL;
_r3_core_api* core_3D = NULL;

u8 r3_init_3D(void) {
    if (r3_3D != NULL) return LIBX_TRUE;  // redundant call: 3D api initialized!

    r3_3D = memx->alloc(sizeof(_r3_3D_api), 8);
    if (!r3_3D) return LIBX_FALSE;  // error: failed to allocate 3D api! out of memory!

    if (!_r3_init_shape3D(r3_3D)) return LIBX_FALSE;    // error: failed to init shape3D api!
    
    if (!_r3_register_component3D(r3_3D)) return LIBX_FALSE;    // error: failed to register 3D components!

    return LIBX_TRUE;
}

u8 r3_cleanup_3D(void) {
    if (!r3_3D || !core_3D) return LIBX_TRUE;  // redumdamt call: 3D api not initialized!

    u8 result = LIBX_TRUE;
    if (!_r3_unregister_component3D()) result = LIBX_FALSE;    // error: failed to unregister 3D components!

    memx->dealloc(r3_3D);
    return result;
}
