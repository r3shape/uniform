#pragma once

#include <koncept/core/koncept_core.h>

typedef enum KC_Component3D {
    KC_MESH3D,
    KC_TRANSFORM3D,
    KC_MATERIAL3D,
    KC_SHADER3D,
    KC_COMPONENT3D_COUNT
} KC_Component3D;

typedef struct KC_Mesh3D {
    Vec3* size;
    Vec3* color;                 // passed as a per-call uniform (`u_sprite_color`) to the render pipeline;.
    KC_Texture* texture;
    KC_Vertex_Data* vertex;
} KC_Mesh3D;

typedef struct KC_Transform3D {
    f32* speed;
    Mat4* model;                // passed per-call to the render pipeline, and set as the value of the uniform (`u_model`).
    Vec3* scale;
    Vec3* rotation;
    Vec3* velocity;
    Vec3* location;
} KC_Transform3D;

typedef struct KC_Material3D {
    f32* shine;
    Vec3* ambient;
    Vec3* diffuse;
    Vec3* specular;
} KC_Material3D;
