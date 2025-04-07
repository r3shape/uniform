#pragma once

#include <r3/r3.core/include/r3.core.h>

typedef enum R3_Component3D {
    R3_MESH3D,
    R3_TRANSFORM3D,
    R3_MATERIAL3D,
    R3_SHADER3D,
    R3_COMPONENT3D_COUNT
} R3_Component3D;

typedef struct R3_Mesh3D {
    Vec3* size;
    Vec3* color;                 // passed as a per-call uniform (`u_sprite_color`) to the render pipeline;.
    R3_Texture* texture;
    R3_Vertex_Data* vertex;
} R3_Mesh3D;

typedef struct R3_Transform3D {
    f32* speed;
    Mat4* model;                // passed per-call to the render pipeline, and set as the value of the uniform (`u_model`).
    Vec3* scale;
    Vec3* rotation;
    Vec3* velocity;
    Vec3* location;
} R3_Transform3D;

typedef struct R3_Material3D {
    f32* shine;
    Vec3* ambient;
    Vec3* diffuse;
    Vec3* specular;
} R3_Material3D;
