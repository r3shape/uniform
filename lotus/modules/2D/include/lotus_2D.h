#pragma once

#include "../../ecs/include/lotus_ecs.h"
#include "../../core/include/lotus_core.h"
#include "../../core/include/lotus_core_defines.h"

typedef enum Lotus_Component2D {
    LOTUS_MESH2D = 0,
    LOTUS_TRANSFORM2D,
    LOTUS_TEXTURE2D,
    LOTUS_2D_COMPONENTS
} Lotus_Component2D;

/* LOTUS2D COMPONENTS */
typedef struct Lotus_Transform2D {
    f32 speed;
    f32 rotation;
    Lotus_Mat4 model;
    Lotus_Vec2 scale;
    Lotus_Vec2 location;
    Lotus_Vec2 velocity;
} Lotus_Transform2D;

typedef struct Lotus_Mesh2D {
    Lotus_Vec2 size;
    Lotus_Vec3 color;
    Lotus_Vertex_Data vertexData;
} Lotus_Mesh2D;

typedef struct Lotus_2D_API {
    void (*set_color2D)(ubyte2 entity_id, Lotus_Vec3 color);

    void (*set_speed2D)(ubyte2 entity_id, f32 speed);
    void (*set_rotation2D)(ubyte2 entity_id, f32 rotation);
    void (*set_scale2D)(ubyte2 entity_id, Lotus_Vec2 scale);
    void (*set_velocity2D)(ubyte2 entity_id, Lotus_Vec2 velocity);
    void (*set_location2D)(ubyte2 entity_id, Lotus_Vec2 location);
    
    Lotus_Vertex_Data (*create_quad2D)(Lotus_Vec2 size, Lotus_Vec3 color);
    Lotus_Vertex_Data (*create_circle2D)(Lotus_Vec2 size, Lotus_Vec3 color);
    Lotus_Vertex_Data (*create_triangle2D)(Lotus_Vec2 size, Lotus_Vec3 color);
} Lotus_2D_API;

extern Lotus_2D_API* lotus_2D;

ubyte lotus_init_2D(void);
void lotus_shutdown_2D(void);
