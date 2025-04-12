#pragma once

#include <r3/modules/3D/include/component3D.h>

typedef struct _r3_3D_api {
    struct shape3D {
        R3_Vertex_Data (*cube3D)(Vec3 size, Vec3 color);
    } shape3D;
    u8 (*set_shader3D)(u32 entity, R3_Shader shader);
} _r3_3D_api;
extern _r3_3D_api* r3_3D;

// called internally via `r3_init_3D`
u8 _r3_init_shape3D(_r3_3D_api* r3_3D);
u8 _r3_register_component3D(_r3_3D_api* r3_3D);
u8 _r3_unregister_component3D(void);

// called internally via `r3_init_core`
u8 r3_init_3D(void);
// called internally via `r3_cleanup_core`
u8 r3_cleanup_3D(void);
