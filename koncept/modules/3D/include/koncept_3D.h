#pragma once

#include <koncept/modules/3D/include/component3D.h>

typedef struct _koncept_3D_api {
    struct shape3D {
        KC_Vertex_Data (*cube3D)(Vec3 size, Vec3 color);
    } shape3D;
    u8 (*set_shadekonceptD)(u32 entity, KC_Shader shader);
} _koncept_3D_api;
extern _koncept_3D_api* koncept_3D;

// called internally via `koncept_init_3D`
u8 _koncept_init_shape3D(_koncept_3D_api* koncept_3D);
u8 _koncept_register_component3D(_koncept_3D_api* koncept_3D);
u8 _koncept_unregister_component3D(void);

// called internally via `koncept_init_core`
u8 koncept_init_3D(void);
// called internally via `koncept_cleanup_core`
u8 koncept_cleanup_3D(void);
