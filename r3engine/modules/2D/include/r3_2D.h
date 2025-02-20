#pragma once

#include "../../ecs/include/r3_ecs.h"
#include "../../core/include/r3_core.h"
#include "../../core/include/r3_core_defines.h"

typedef enum R3_Component2D {
    R3_MESH2D = 0,
    R3_TRANSFORM2D,
    R3_TEXTURE2D,
    R3_2D_COMPONENTS
} R3_Component2D;

/* R32D COMPONENTS */
typedef struct R3_Transform2D {
    f32 speed;
    f32 rotation;
    R3_Mat4 model;
    R3_Vec2 scale;
    R3_Vec2 location;
    R3_Vec2 velocity;
} R3_Transform2D;

typedef struct R3_Mesh2D {
    R3_Vec2 size;
    R3_Vec3 color;
    R3_Vertex_Data vertexData;
} R3_Mesh2D;

typedef struct R3_2D_API {
    void (*set_color2D)(ubyte2 entity_id, R3_Vec3 color);

    void (*set_speed2D)(ubyte2 entity_id, f32 speed);
    void (*set_rotation2D)(ubyte2 entity_id, f32 rotation);
    void (*set_scale2D)(ubyte2 entity_id, R3_Vec2 scale);
    void (*set_velocity2D)(ubyte2 entity_id, R3_Vec2 velocity);
    void (*set_location2D)(ubyte2 entity_id, R3_Vec2 location);
    
    R3_Vertex_Data (*create_quad2D)(R3_Vec2 size, R3_Vec3 color);
    R3_Vertex_Data (*create_circle2D)(R3_Vec2 size, R3_Vec3 color);
    R3_Vertex_Data (*create_triangle2D)(R3_Vec2 size, R3_Vec3 color);
} R3_2D_API;

extern R3_2D_API* r3_2D;

ubyte r3_init_2D(void);
void r3_shutdown_2D(void);
