#pragma once

#include "../../core/include/lotus_core.h"
#include "../../core/include/lotus_core_defines.h"

#include "../../ecs/include/lotus_ecs.h"

typedef enum Lotus_Component3D {
    LOTUS_MESH3D = 0,
    LOTUS_TRANSFORM3D,
    LOTUS_TEXTURE2D,
    LOTUS_3D_COMPONENTS
} Lotus_Component3D;

/* LOTUS3D COMPONENTS */
typedef struct Lotus_Transform3D {
    f32 speed;
    Lotus_Mat4 model;
    Lotus_Vec3 scale;
    Lotus_Vec3 rotation;
    Lotus_Vec3 velocity;
    Lotus_Vec3 location;
} Lotus_Transform3D;

typedef struct Lotus_Mesh3D {
    Lotus_Vec3 size;
    Lotus_Vec3 color;
    Lotus_Vertex_Data vertexData;
} Lotus_Mesh3D;

typedef struct Lotus_3D_API {
    void (*set_color3D)(ubyte2 entity_id, Lotus_Vec3 color);
    
    void (*set_speed3D)(ubyte2 entity_id, f32 speed);
    void (*set_scale3D)(ubyte2 entity_id, Lotus_Vec3 scale);
    void (*set_rotation3D)(ubyte2 entity_id, Lotus_Vec3 rotation);
    void (*set_velocity3D)(ubyte2 entity_id, Lotus_Vec3 velocity);
    void (*set_location3D)(ubyte2 entity_id, Lotus_Vec3 location);

    Lotus_Vertex_Data (*create_cube3D)(Lotus_Vec3 size, Lotus_Vec3 color);
    Lotus_Vertex_Data (*create_quad3D)(Lotus_Vec3 size, Lotus_Vec3 color);
} Lotus_3D_API;

extern Lotus_3D_API* lotus_3D;

ubyte lotus_init_3D(void);
void lotus_shutdown_3D(void);
