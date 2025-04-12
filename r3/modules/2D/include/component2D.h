#pragma once

#include <r3/core/r3.core.h>

typedef enum R3_Component2D {
    R3_SPRITE2D,
    R3_TRANSFORM2D,
    R3_COMPONENT2D_COUNT
} R3_Component2D;

typedef struct R3_Sprite2D {
    Vec2* size;
    Vec3* color;                 // passed as a per-call uniform (`u_sprite_color`) to the render pipeline;.
    R3_Texture* texture;
    R3_Vertex_Data* vertex;
} R3_Sprite2D;

typedef struct R3_Transform2D {
    f32* speed;
    Mat4* model;                // passed per-call to the render pipeline, and set as the value of the uniform (`u_model`).
    Vec2* scale;
    f32* rotation;
    Vec2* velocity;
    Vec2* location;
} R3_Transform2D;
