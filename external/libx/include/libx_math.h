#pragma once

#include <math.h>
#include "libx_def.h"

#define LIBX_PI 3.14159265358979323846

/* ---------------- VECTOR ---------------- */
typedef struct Vec2 { f32 x, y; } Vec2;
typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef struct Vec4 { f32 x, y, z, w; } Vec4;
/* ---------------- VECTOR ---------------- */

/* ---------------- MATRIX 4 ---------------- */
typedef struct Mat2 { f32  m[4]; } Mat2;
typedef struct Mat3 { f32  m[9]; } Mat3;
typedef struct Mat4 { f32 m[16]; } Mat4;
/* ---------------- MATRIX 4 ---------------- */

typedef struct _libx_math_api {
    struct scalar {
        // Scalar Math
        f32 (*radians)(f32 deg);
    } scalar;
    
    struct vec {
        // Utility
        void (*print2)(cstr name, Vec2 vec);
        void (*print3)(cstr name, Vec3 vec);
        void (*print4)(cstr name, Vec4 vec);
        
        // Vec2 Operations
        Vec2 (*vec2)(f32 x, f32 y);
        Vec2 (*scale2)(Vec2 v, f32 scale);
        Vec2 (*add2)(Vec2 v1, Vec2 v2);
        Vec2 (*sub2)(Vec2 v1, Vec2 v2);
        f32 (*dot2)(Vec2 v1, Vec2 v2);
        f32 (*mag2)(Vec2 v);
        Vec2 (*norm2)(Vec2 v);
    
        // Vec3 Operations
        Vec3 (*vec3)(f32 x, f32 y, f32 z);
        Vec3 (*scale3)(Vec3 v, f32 scale);
        Vec3 (*add3)(Vec3 v1, Vec3 v2);
        Vec3 (*sub3)(Vec3 v1, Vec3 v2);
        f32 (*dot3)(Vec3 a, Vec3 b);
        f32 (*mag3)(Vec3 v);
        Vec3 (*norm3)(Vec3 v);
        Vec3 (*cross3)(Vec3 v1, Vec3 v2);
    
        // Vec4 Operations
        Vec4 (*vec4)(f32 x, f32 y, f32 z, f32 w);
        Vec4 (*scale4)(Vec4 v, f32 scale);
        Vec4 (*add4)(Vec4 v1, Vec4 v2);
        Vec4 (*sub4)(Vec4 v1, Vec4 v2);
        f32 (*dot4)(Vec4 v1, Vec4 v2);
        f32 (*mag4)(Vec4 v);
        Vec4 (*norm4)(Vec4 v);
    } vec;

    /**
     * Right-Handed Column-Major Matrix Transformation API
     * (OpenGL Friendly)
     */
    struct mat {
        // Utility
        void (*print2)(cstr name, Mat2 mat);
        void (*print3)(cstr name, Mat3 mat);
        void (*print4)(cstr name, Mat4 mat);
        
        // Mat2 Operations
        Mat2 (*identity2)(void);
        Mat2 (*transpose2)(Mat2 mat);
        Mat2 (*mult2)(Mat2 a, Mat2 b);
        
        // Mat3 Operations
        Mat3 (*identity3)(void);
        Mat3 (*transpose3)(Mat3 mat);
        Mat3 (*mult3)(Mat3 a, Mat3 b);
    
        // Mat4 Operations
        Mat4 (*identity4)(void);
        Mat4 (*transpose4)(Mat4 mat);
        Mat4 (*trans4)(f32 x, f32 y, f32 z);
        Vec3 (*mult4v3)(Mat4 m, Vec3 v);
        Mat4 (*rot4)(Vec3 axis, f32 angle);
        Mat4 (*rotx4)(f32 angle);
        Mat4 (*roty4)(f32 angle);
        Mat4 (*rotz4)(f32 angle);
        Mat4 (*scale4)(f32 x, f32 y, f32 z);
        Mat4 (*mult4)(Mat4 a, Mat4 b);
        Mat4 (*lookat)(Vec3 eye, Vec3 center, Vec3 up);
        Mat4 (*ortho)(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

        /**
         * symmetric frustum using vertical FOV
        */
        Mat4 (*perspective)(f32 fov, f32 aspect, f32 near, f32 far);
    } mat;    
} _libx_math_api;
extern _libx_math_api* mathx;

LIBX_API u8 libx_init_math(void);
LIBX_API void libx_cleanup_math(void);
