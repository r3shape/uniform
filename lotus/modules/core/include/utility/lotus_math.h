#pragma once

#include <math.h>
#include "../lotus_core_defines.h"

#define LOTUS_PI 3.14159265358979323846

typedef struct Lotus_Vec2 {
    f32 x, y;
} Lotus_Vec2;

typedef struct Lotus_Vec2i {
    sbyte4 x, y;
} Lotus_Vec2i;

typedef struct Lotus_Vec2b {
    ubyte x, y;
} Lotus_Vec2b;

typedef struct Lotus_Vec3 {
    f32 x, y, z;
} Lotus_Vec3;

typedef struct Lotus_Vec3i {
    sbyte4 x, y, z;
} Lotus_Vec3i;

typedef struct Lotus_Vec3b {
    ubyte x, y, z;
} Lotus_Vec3b;

typedef struct Lotus_Vec4 {
    f32 x, y, z, w;
} Lotus_Vec4;

typedef struct Lotus_Vec4i {
    sbyte4 x, y, z, w;
} Lotus_Vec4i;

typedef struct Lotus_Vec4b {
    ubyte x, y, z, w;
} Lotus_Vec4b;

typedef struct Lotus_Mat4{
    f32 m[16];
} Lotus_Mat4;

#define LOTUS_VEC2(type, x, y) (type[2]){x, y}
#define LOTUS_VEC3(type, x, y, z) (type[3]){x, y, z}
#define LOTUS_VEC4(type, x, y, z, w) (type[4]){x, y, z, w}

#define LOTUS_MAT4(type) (type[16]){0}
#define LOTUS_IDENTITY(type) (type[16]){ \
    1.0, 0.0, 0.0, 0.0, \
    0.0, 1.0, 0.0, 0.0, \
    0.0, 0.0, 1.0, 0.0, \
    0.0, 0.0, 0.0, 1.0  \
}

LOTUS_API f32 lotus_to_radians(f32 deg);

LOTUS_API Lotus_Vec2 lotus_new_vec2(f32 x, f32 y);
LOTUS_API Lotus_Vec2 lotus_scale_vec2(Lotus_Vec2 v, f32 scale);
LOTUS_API Lotus_Vec2 lotus_add_vec2(Lotus_Vec2 v1, Lotus_Vec2 v2);
LOTUS_API Lotus_Vec2 lotus_sub_vec2(Lotus_Vec2 v1, Lotus_Vec2 v2);
LOTUS_API f32 lotus_dot_vec2(Lotus_Vec2 v1, Lotus_Vec2 v2);
LOTUS_API Lotus_Vec2 lotus_norm_vec2(Lotus_Vec2 v);

LOTUS_API Lotus_Vec3 lotus_new_vec3(f32 x, f32 y, f32 z);
LOTUS_API Lotus_Vec3 lotus_scale_vec3(Lotus_Vec3 v, f32 scale);
LOTUS_API Lotus_Vec3 lotus_add_vec3(Lotus_Vec3 v1, Lotus_Vec3 v2);
LOTUS_API Lotus_Vec3 lotus_sub_vec3(Lotus_Vec3 v1, Lotus_Vec3 v2);
LOTUS_API f32 lotus_dot_vec3(Lotus_Vec3 a, Lotus_Vec3 b);
LOTUS_API Lotus_Vec3 lotus_norm_vec3(Lotus_Vec3 v);
LOTUS_API Lotus_Vec3 lotus_cross_vec3(Lotus_Vec3 v1, Lotus_Vec3 v2);

LOTUS_API Lotus_Vec4 lotus_new_vec4(f32 x, f32 y, f32 z, f32 w);
LOTUS_API Lotus_Vec4 lotus_scale_vec4(Lotus_Vec4 v, f32 scale);
LOTUS_API Lotus_Vec4 lotus_add_vec4(Lotus_Vec4 v1, Lotus_Vec4 v2);
LOTUS_API Lotus_Vec4 lotus_sub_vec4(Lotus_Vec4 v1, Lotus_Vec4 v2);
LOTUS_API f32 lotus_dot_vec4(Lotus_Vec4 v1, Lotus_Vec4 v2);
LOTUS_API Lotus_Vec4 lotus_norm_vec4(Lotus_Vec4 v);

LOTUS_API Lotus_Mat4 lotus_identity();
LOTUS_API Lotus_Mat4 lotus_mul_mat4(Lotus_Mat4 a, Lotus_Mat4 b);
LOTUS_API Lotus_Vec3 lotus_mul_mat4_vec3(Lotus_Mat4 m, Lotus_Vec3 v);
LOTUS_API Lotus_Mat4 lotus_trans_mat4(f32 x, f32 y, f32 z);
LOTUS_API Lotus_Mat4 lotus_scale_mat4(f32 x, f32 y, f32 z);
LOTUS_API Lotus_Mat4 lotus_look_at(Lotus_Vec3 eye, Lotus_Vec3 center, Lotus_Vec3 up);

LOTUS_API Lotus_Mat4 lotus_rotx_mat4(f32 angle);
LOTUS_API Lotus_Mat4 lotus_roty_mat4(f32 angle);
LOTUS_API Lotus_Mat4 lotus_rotz_mat4(f32 angle);
LOTUS_API Lotus_Mat4 lotus_rot_mat4(f32 x, f32 y, f32 z, f32 angle);

LOTUS_API Lotus_Mat4 lotus_perspective(f32 fov, f32 aspect, f32 near, f32 far);
LOTUS_API Lotus_Mat4 lotus_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
