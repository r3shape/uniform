#pragma once

#include <math.h>
#include "../r3_core_defines.h"

#define R3_PI 3.14159265358979323846

typedef struct R3_Vec2 {
    f32 x, y;
} R3_Vec2;

typedef struct R3_Vec2i {
    sbyte4 x, y;
} R3_Vec2i;

typedef struct R3_Vec2b {
    ubyte x, y;
} R3_Vec2b;

typedef struct R3_Vec3 {
    f32 x, y, z;
} R3_Vec3;

typedef struct R3_Vec3i {
    sbyte4 x, y, z;
} R3_Vec3i;

typedef struct R3_Vec3b {
    ubyte x, y, z;
} R3_Vec3b;

typedef struct R3_Vec4 {
    f32 x, y, z, w;
} R3_Vec4;

typedef struct R3_Vec4i {
    sbyte4 x, y, z, w;
} R3_Vec4i;

typedef struct R3_Vec4b {
    ubyte x, y, z, w;
} R3_Vec4b;

typedef struct R3_Mat4{
    f32 m[16];
} R3_Mat4;

#define R3_VEC2(type, x, y) (type[2]){x, y}
#define R3_VEC3(type, x, y, z) (type[3]){x, y, z}
#define R3_VEC4(type, x, y, z, w) (type[4]){x, y, z, w}

#define R3_MAT4(type) (type[16]){0}
#define R3_IDENTITY(type) (type[16]){ \
    1.0, 0.0, 0.0, 0.0, \
    0.0, 1.0, 0.0, 0.0, \
    0.0, 0.0, 1.0, 0.0, \
    0.0, 0.0, 0.0, 1.0  \
}

R3_API f32 r3_to_radians(f32 deg);

R3_API R3_Vec2 r3_new_vec2(f32 x, f32 y);
R3_API R3_Vec2 r3_scale_vec2(R3_Vec2 v, f32 scale);
R3_API R3_Vec2 r3_add_vec2(R3_Vec2 v1, R3_Vec2 v2);
R3_API R3_Vec2 r3_sub_vec2(R3_Vec2 v1, R3_Vec2 v2);
R3_API f32 r3_dot_vec2(R3_Vec2 v1, R3_Vec2 v2);
R3_API R3_Vec2 r3_norm_vec2(R3_Vec2 v);

R3_API R3_Vec3 r3_new_vec3(f32 x, f32 y, f32 z);
R3_API R3_Vec3 r3_scale_vec3(R3_Vec3 v, f32 scale);
R3_API R3_Vec3 r3_add_vec3(R3_Vec3 v1, R3_Vec3 v2);
R3_API R3_Vec3 r3_sub_vec3(R3_Vec3 v1, R3_Vec3 v2);
R3_API f32 r3_dot_vec3(R3_Vec3 a, R3_Vec3 b);
R3_API R3_Vec3 r3_norm_vec3(R3_Vec3 v);
R3_API R3_Vec3 r3_cross_vec3(R3_Vec3 v1, R3_Vec3 v2);

R3_API R3_Vec4 r3_new_vec4(f32 x, f32 y, f32 z, f32 w);
R3_API R3_Vec4 r3_scale_vec4(R3_Vec4 v, f32 scale);
R3_API R3_Vec4 r3_add_vec4(R3_Vec4 v1, R3_Vec4 v2);
R3_API R3_Vec4 r3_sub_vec4(R3_Vec4 v1, R3_Vec4 v2);
R3_API f32 r3_dot_vec4(R3_Vec4 v1, R3_Vec4 v2);
R3_API R3_Vec4 r3_norm_vec4(R3_Vec4 v);

R3_API R3_Mat4 r3_identity();
R3_API R3_Mat4 r3_mul_mat4(R3_Mat4 a, R3_Mat4 b);
R3_API R3_Vec3 r3_mul_mat4_vec3(R3_Mat4 m, R3_Vec3 v);
R3_API R3_Mat4 r3_trans_mat4(f32 x, f32 y, f32 z);
R3_API R3_Mat4 r3_scale_mat4(f32 x, f32 y, f32 z);
R3_API R3_Mat4 r3_look_at(R3_Vec3 eye, R3_Vec3 center, R3_Vec3 up);

R3_API R3_Mat4 r3_rotx_mat4(f32 angle);
R3_API R3_Mat4 r3_roty_mat4(f32 angle);
R3_API R3_Mat4 r3_rotz_mat4(f32 angle);
R3_API R3_Mat4 r3_rot_mat4(f32 x, f32 y, f32 z, f32 angle);

R3_API R3_Mat4 r3_perspective(f32 fov, f32 aspect, f32 near, f32 far);
R3_API R3_Mat4 r3_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
