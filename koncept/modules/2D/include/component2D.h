#pragma once

#include <koncept/core/koncept_core.h>

typedef enum KC_Component2D {
    KC_SPRITE2D,
    KC_TRANSFORM2D,
    KC_COMPONENT2D_COUNT
} KC_Component2D;

typedef struct KC_Sprite2D {
    Vec2* size;
    Vec3* color;                 // passed as a per-call uniform (`u_sprite_color`) to the render pipeline;.
    KC_Texture* texture;
    KC_Vertex_Data* vertex;
} KC_Sprite2D;

typedef struct KC_Transform2D {
    f32* speed;
    Mat4* model;                // passed per-call to the render pipeline, and set as the value of the uniform (`u_model`).
    Vec2* scale;
    f32* rotation;
    Vec2* velocity;
    Vec2* location;
} KC_Transform2D;
