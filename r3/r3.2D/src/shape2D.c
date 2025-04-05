#include <r3/r3.2D/include/r3.2D.h>
#include <r3/r3.core/include/r3.core.h>

R3_Vertex_Data _triangle2D_impl(Vec2 size, Vec3 color) {
    f32 half_width =    size.x / 2.0f;
    f32 half_height =   size.y / 2.0f;

    f32 vertices[] = {
        -half_width, -half_height, 0.0,  color.x, color.y, color.z,   0.0, 0.0,
         half_width, -half_height, 0.0,  color.x, color.y, color.z,   1.0, 0.0,
         0.0,         half_height, 0.0,  color.x, color.y, color.z,   0.5, 1.0
    };

    return r3_core->graphics.create_vertex_data(
        vertices, 3,
        NULL,     0,
        R3_LOCATION_ATTR|R3_COLOR_ATTR|R3_TCOORD_ATTR
    );
}

R3_Vertex_Data _quad2D_impl(Vec2 size, Vec3 color) {
    f32 half_width =    size.x / 2.0f;
    f32 half_height =   size.y / 2.0f;

    f32 vertices[] = {
        -half_width, -half_height, 0.0,  color.x, color.y, color.z,   0.0, 0.0,
         half_width, -half_height, 0.0,  color.x, color.y, color.z,   1.0, 0.0,
         half_width,  half_height, 0.0,  color.x, color.y, color.z,   1.0, 1.0,
        -half_width,  half_height, 0.0,  color.x, color.y, color.z,   0.0, 1.0
    };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    return r3_core->graphics.create_vertex_data(
        vertices, 4,
        indices,  6,
        R3_LOCATION_ATTR|R3_COLOR_ATTR|R3_TCOORD_ATTR
    );
}

u8 _r3_init_shape2D(_r3_2D_api* r3_2D) {
    if (!r3_2D) return LIBX_FALSE;  // error: null ptr!
    
    if (r3_2D->shape2D.triangle2D != NULL ||
        r3_2D->shape2D.quad2D != NULL
    ) return LIBX_TRUE; // error: reduntant call: shape2D api initialized!

    r3_2D->shape2D.triangle2D = _triangle2D_impl;
    r3_2D->shape2D.quad2D = _quad2D_impl;

    return LIBX_TRUE;
}
