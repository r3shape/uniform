#include <koncept/modules/3D/include/koncept_3D.h>

_koncept_3D_api* koncept_3D = NULL;
_koncept_core_api* core_3D = NULL;

u8 koncept_init_3D(void) {
    if (koncept_3D != NULL) return COREX_TRUE;  // redundant call: 3D api initialized!

    koncept_3D = corex->memx.alloc(sizeof(_koncept_3D_api), 8);
    if (!koncept_3D) return COREX_FALSE;  // error: failed to allocate 3D api! out of memory!

    if (!_koncept_init_shape3D(koncept_3D)) return COREX_FALSE;    // error: failed to init shape3D api!

    if (!_koncept_register_component3D(koncept_3D)) return COREX_FALSE;    // error: failed to register 3D components!

    return COREX_TRUE;
}

u8 koncept_cleanup_3D(void) {
    if (!koncept_3D || !core_3D) return COREX_TRUE;  // redumdamt call: 3D api not initialized!

    u8 result = COREX_TRUE;
    if (!_koncept_unregister_component3D()) result = COREX_FALSE;    // error: failed to unregister 3D components!

    corex->memx.dealloc(koncept_3D);
    return result;
}
