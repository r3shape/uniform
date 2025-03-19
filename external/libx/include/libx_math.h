#pragma once

#include <math.h>
#include "libx_def.h"

#define LIBX_PI 3.14159265358979323846

/* ---------------- VECTOR ---------------- */
typedef struct Vec2 {
    f32 x, y;
} Vec2;

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Vec4 {
    f32 x, y, z, w;
} Vec4;
/* ---------------- VECTOR ---------------- */

/* ---------------- MATRIX 4 ---------------- */
typedef struct Mat4 {
    f32 m[16];
} Mat4;
/* ---------------- MATRIX 4 ---------------- */

typedef struct _libx_math_api {
    // Utility
    void (*print_vec2)(cstr name, Vec2 vector);
    void (*print_vec3)(cstr name, Vec3 vector);
    void (*print_vec4)(cstr name, Vec4 vector);
    void (*print_mat4)(cstr name, Mat4 matrix);

    // Scalar Math
    f32 (*to_radians)(f32 deg);

    // Vec2 Operations
    Vec2 (*new_vec2)(f32 x, f32 y);
    Vec2 (*scale_vec2)(Vec2 v, f32 scale);
    Vec2 (*add_vec2)(Vec2 v1, Vec2 v2);
    Vec2 (*sub_vec2)(Vec2 v1, Vec2 v2);
    f32 (*dot_vec2)(Vec2 v1, Vec2 v2);
    Vec2 (*norm_vec2)(Vec2 v);

    // Vec3 Operations
    Vec3 (*new_vec3)(f32 x, f32 y, f32 z);
    Vec3 (*scale_vec3)(Vec3 v, f32 scale);
    Vec3 (*add_vec3)(Vec3 v1, Vec3 v2);
    Vec3 (*sub_vec3)(Vec3 v1, Vec3 v2);
    f32 (*dot_vec3)(Vec3 a, Vec3 b);
    Vec3 (*norm_vec3)(Vec3 v);
    Vec3 (*cross_vec3)(Vec3 v1, Vec3 v2);

    // Vec4 Operations
    Vec4 (*new_vec4)(f32 x, f32 y, f32 z, f32 w);
    Vec4 (*scale_vec4)(Vec4 v, f32 scale);
    Vec4 (*add_vec4)(Vec4 v1, Vec4 v2);
    Vec4 (*sub_vec4)(Vec4 v1, Vec4 v2);
    f32 (*dot_vec4)(Vec4 v1, Vec4 v2);
    Vec4 (*norm_vec4)(Vec4 v);

    // Mat4 Operations
    Mat4 (*identity)(void);
    Mat4 (*transpose)(Mat4 mat);
    Mat4 (*translate)(f32 x, f32 y, f32 z);
    Vec3 (*mul_mat4_vec3)(Mat4 m, Vec3 v);
    Mat4 (*rotate)(f32 x, f32 y, f32 z, f32 angle);
    Mat4 (*rotate_x)(f32 angle);
    Mat4 (*rotate_y)(f32 angle);
    Mat4 (*rotate_z)(f32 angle);
    Mat4 (*scale_mat4)(f32 x, f32 y, f32 z);
    Mat4 (*mul_mat4)(Mat4 a, Mat4 b);
    Mat4 (*ortho)(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
    Mat4 (*perspective)(f32 fov, f32 aspect, f32 near, f32 far);
    Mat4 (*look_at)(Vec3 eye, Vec3 center, Vec3 up);
} _libx_math_api;
extern _libx_math_api* math_api;

LIBX_API u8 libx_init_math(void);
LIBX_API void libx_cleanup_math(void);
