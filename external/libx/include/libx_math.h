#pragma once

#include <math.h>
#include "libx_def.h"

/* ---------------- VECTOR ---------------- */
typedef enum Vector_Type {
    LIBX_VEC2,
    LIBX_VEC3,
    LIBX_VEC4,
    LIBX_VEC_TYPES,
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
} _libx_math_api;
extern _libx_math_api* math_api;

LIBX_API u8 libx_init_math(void);
LIBX_API void libx_cleanup_math(void);
