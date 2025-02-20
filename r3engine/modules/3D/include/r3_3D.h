#pragma once

#include "../../ecs/include/r3_ecs.h"
#include "../../core/include/r3_core.h"
#include "../../core/include/r3_core_defines.h"

typedef enum R3_Component3D {
    R3_MESH3D = 0,
    R3_TRANSFORM3D,
    R3_TEXTURE2D,
    R3_3D_COMPONENTS
} R3_Component3D;

/* R33D COMPONENTS */
typedef struct R3_Transform3D {
    f32 speed;
    R3_Mat4 model;
    R3_Vec3 scale;
    R3_Vec3 rotation;
    R3_Vec3 velocity;
    R3_Vec3 location;
} R3_Transform3D;

typedef struct R3_Mesh3D {
    R3_Vec3 size;
    R3_Vec3 color;
    R3_Vertex_Data vertexData;
} R3_Mesh3D;

typedef struct R3_3D_API {
    void (*set_color3D)(ubyte2 entity_id, R3_Vec3 color);
    
    void (*set_speed3D)(ubyte2 entity_id, f32 speed);
    void (*set_scale3D)(ubyte2 entity_id, R3_Vec3 scale);
    void (*set_rotation3D)(ubyte2 entity_id, R3_Vec3 rotation);
    void (*set_velocity3D)(ubyte2 entity_id, R3_Vec3 velocity);
    void (*set_location3D)(ubyte2 entity_id, R3_Vec3 location);

    R3_Vertex_Data (*create_cube3D)(R3_Vec3 size, R3_Vec3 color);
    R3_Vertex_Data (*create_quad3D)(R3_Vec3 size, R3_Vec3 color);
} R3_3D_API;

extern R3_3D_API* r3_3D;

ubyte r3_init_3D(void);
void r3_shutdown_3D(void);
