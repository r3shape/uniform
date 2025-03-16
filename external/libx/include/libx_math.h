#pragma once

#include <math.h>
#include "libx_def.h"

/* ---------------- VECTOR ---------------- */
typedef enum Vector_Type {
    LIBX_VEC2,
    LIBX_VEC3,
    LIBX_VEC4,
    LIBX_VEC_TYPES
} Vector_Type;

typedef struct Vector {
    Vector_Type type;
    union vec {
        f32 vec2[2];
        f32 vec3[3];
        f32 vec4[4];
    } vec;
} Vector;
/* ---------------- VECTOR ---------------- */

/* ---------------- MATRIX 4 ---------------- */
typedef struct Matrix4{
    f32 mat[16];
} Matrix4;
/* ---------------- MATRIX 4 ---------------- */

typedef struct _libx_math_api {
    Vector (*create_vector)(Vector_Type type, f32* vector);
    Vector (*scale_vector)(f32 scalar, Vector* vector);
    void (*print_vector)(cstr name, Vector* vector);
    Vector (*cross_vector)(Vector* v1, Vector* v2);
    Vector (*add_vector)(Vector* v1, Vector* v2);
    Vector (*sub_vector)(Vector* v1, Vector* v2);
    f32 (*dot_vector)(Vector* v1, Vector* v2);
    Vector (*norm_vector)(Vector* vector);
    f32 (*mag_vector)(Vector* vector);

    void (*print_matrix4)(cstr name, Matrix4* matrix4);
    Matrix4 (*identity_matrix4)(void);
    Matrix4 (*create_matrix4)(f32* matrix4);
    Matrix4 (*rot_matrix4)(f32 x, f32 y, f32 z);
    Matrix4 (*trans_matrix4)(f32 x, f32 y, f32 z);
    Matrix4 (*scale_matrix4)(f32 x, f32 y, f32 z);
    Matrix4 (*mult_matrix4)(Matrix4 m1, Matrix4 m2);
    Vector (*mult_matrix4_vector3)(Matrix4 matrix4, Vector vector);
    Matrix4 (*look_at)(Vector eye, Vector center, Vector up);
    Matrix4 (*perspective)(f32 fov, f32 aspect, f32 near, f32 far);
    Matrix4 (*ortho)(f32 left, f32 top, f32 right, f32 bottom, f32 near, f32 far);
} _libx_math_api;
extern _libx_math_api* math_api;

LIBX_API u8 libx_init_math(void);
LIBX_API void libx_cleanup_math(void);
