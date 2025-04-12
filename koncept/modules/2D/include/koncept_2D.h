#pragma once

#include <koncept/modules/2D/include/component2D.h>

typedef struct _koncept_2D_api {
    struct shape2D {
        KC_Vertex_Data (*triangle2D)(Vec2 size, Vec3 color);
        KC_Vertex_Data (*quad2D)(Vec2 size, Vec3 color);
    } shape2D;
} _koncept_2D_api;
extern _koncept_2D_api* koncept_2D;

// called internally via `koncept_init_2D`
u8 _koncept_init_shape2D(_koncept_2D_api* koncept_2D);
u8 _koncept_register_component2D(void);
u8 _koncept_unregister_component2D(void);

// called internally via `koncept_init_core`
u8 koncept_init_2D(void);
// called internally via `koncept_cleanup_core`
u8 koncept_cleanup_2D(void);
