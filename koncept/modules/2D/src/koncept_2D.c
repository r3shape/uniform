#include <koncept/modules/2D/include/koncept_2D.h>

_koncept_2D_api* koncept_2D = NULL;
_koncept_core_api* core_2D = NULL;

u8 koncept_init_2D(void) {
    if (koncept_2D != NULL) return COREX_TRUE;  // redundant call: 2D api initialized!

    koncept_2D = corex->memx.alloc(sizeof(_koncept_2D_api), 8);
    if (!koncept_2D) return COREX_FALSE;  // error: failed to allocate 2D api! out of memory!

    if (!_koncept_init_shape2D(koncept_2D)) return COREX_FALSE;    // error: failed to init shape2D api!
    
    if (!_koncept_register_component2D()) return COREX_FALSE;    // error: failed to register 2D components!

    return COREX_TRUE;
}

u8 koncept_cleanup_2D(void) {
    if (!koncept_2D || !core_2D) return COREX_TRUE;  // redumdamt call: 2D api not initialized!

    u8 result = COREX_TRUE;
    if (!_koncept_unregister_component2D()) result = COREX_FALSE;    // error: failed to unregister 2D components!

    corex->memx.dealloc(koncept_2D);
    return result;
}
