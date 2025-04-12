#include <koncept/modules/2D/include/koncept_2D.h>

KC_Vertex_Data _triangle2D_impl(Vec2 size, Vec3 color) {
    f32 half_width =    size.x * 0.5f;
    f32 half_height =   size.y * 0.5f;

    return koncept_core->graphics.create_vertex_data(
        (f32[]) {
            -half_width, -half_height, 0.0,  color.x, color.y, color.z,   0.0, 0.0,
             half_width, -half_height, 0.0,  color.x, color.y, color.z,   1.0, 0.0,
             0.0,         half_height, 0.0,  color.x, color.y, color.z,   0.5, 1.0
        },   3,
        NULL,0,
        KC_LOCATION_ATTR|KC_COLOR_ATTR|KC_TCOORD_ATTR
    );
}

KC_Vertex_Data _quad2D_impl(Vec2 size, Vec3 color) {
    f32 half_width =    size.x * 0.5f;
    f32 half_height =   size.y * 0.5f;

    return koncept_core->graphics.create_vertex_data(
        (f32[]) {
            -half_width, -half_height, 0.0,  color.x, color.y, color.z,   0.0, 0.0,
             half_width, -half_height, 0.0,  color.x, color.y, color.z,   1.0, 0.0,
             half_width,  half_height, 0.0,  color.x, color.y, color.z,   1.0, 1.0,
            -half_width,  half_height, 0.0,  color.x, color.y, color.z,   0.0, 1.0
        },  4,
        (u32[]) {
            0, 1, 2,
            2, 3, 0
        },  6,
        KC_LOCATION_ATTR|KC_COLOR_ATTR|KC_TCOORD_ATTR
    );
}


u8 _koncept_init_shape2D(_koncept_2D_api* koncept_2D) {
    if (!koncept_2D) return COREX_FALSE;  // error: null ptr!
    
    if (koncept_2D->shape2D.triangle2D != NULL ||
        koncept_2D->shape2D.quad2D != NULL
    ) return COREX_TRUE; // error: reduntant call: shape2D api initialized!

    koncept_2D->shape2D.triangle2D = _triangle2D_impl;
    koncept_2D->shape2D.quad2D = _quad2D_impl;

    return COREX_TRUE;
}
