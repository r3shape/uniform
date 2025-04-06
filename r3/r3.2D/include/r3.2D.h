#pragma once

#include <r3/r3.core/include/r3.core.h>
#include <r3/r3.2D/include/component2D.h>

typedef struct _r3_2D_api {
    struct shape2D {
        R3_Vertex_Data (*triangle2D)(Vec2 size, Vec3 color);
        R3_Vertex_Data (*quad2D)(Vec2 size, Vec3 color);
    } shape2D;
} _r3_2D_api;
extern _r3_2D_api* r3_2D;

// called internally via `r3_init_2D`
u8 _r3_init_shape2D(_r3_2D_api* r3_2D);
u8 _r3_register_component2D(void);
u8 _r3_unregister_component2D(void);

// called internally via `r3_init_core`
u8 r3_init_2D(void* r3_core);
// called internally via `r3_cleanup_core`
u8 r3_cleanup_2D(void);
