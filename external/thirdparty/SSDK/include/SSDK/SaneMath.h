#ifndef __SANEMATH_H__
#define __SANEMATH_H__

#include <include/SSDK/globals.h>

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

typedef struct SaneMath {
    SaneModule module;

    struct scalar {
        // Scalar Math
        SSDK_FNPTR(f32, radians, f32 deg);
    } scalar;
    
    struct vector {
        // Utility
        SSDK_FNPTR(void, log2, cstr name, Vec2 vec);
        SSDK_FNPTR(void, log3, cstr name, Vec3 vec);
        SSDK_FNPTR(void, log4, cstr name, Vec4 vec);
        
        // Vec2 Operations
        SSDK_FNPTR(Vec2, vec2, f32 x, f32 y);
        SSDK_FNPTR(Vec2, scale2, Vec2 v, f32 scale);
        SSDK_FNPTR(Vec2, add2, Vec2 v1, Vec2 v2);
        SSDK_FNPTR(Vec2, sub2, Vec2 v1, Vec2 v2);
        SSDK_FNPTR(f32, dot2, Vec2 v1, Vec2 v2);
        SSDK_FNPTR(f32, mag2, Vec2 v);
        SSDK_FNPTR(Vec2, norm2, Vec2 v);
    
        // Vec3 Operations
        SSDK_FNPTR(Vec3, vec3, f32 x, f32 y, f32 z);
        SSDK_FNPTR(Vec3, scale3, Vec3 v, f32 scale);
        SSDK_FNPTR(Vec3, add3, Vec3 v1, Vec3 v2);
        SSDK_FNPTR(Vec3, sub3, Vec3 v1, Vec3 v2);
        SSDK_FNPTR(f32, dot3, Vec3 a, Vec3 b);
        SSDK_FNPTR(f32, mag3, Vec3 v);
        SSDK_FNPTR(Vec3, norm3, Vec3 v);
        SSDK_FNPTR(Vec3, cross3, Vec3 v1, Vec3 v2);
    
        // Vec4 Operations
        SSDK_FNPTR(Vec4, vec4, f32 x, f32 y, f32 z, f32 w);
        SSDK_FNPTR(Vec4, scale4, Vec4 v, f32 scale);
        SSDK_FNPTR(Vec4, add4, Vec4 v1, Vec4 v2);
        SSDK_FNPTR(Vec4, sub4, Vec4 v1, Vec4 v2);
        SSDK_FNPTR(f32, dot4, Vec4 v1, Vec4 v2);
        SSDK_FNPTR(f32, mag4, Vec4 v);
        SSDK_FNPTR(Vec4, norm4, Vec4 v);
    } vector;

    /**
     * Right-Handed Column-Major Matrix Transformation API
     * (OpenGL Friendly)
     */
    struct matrix {
        // Utility
        SSDK_FNPTR(void, log2, cstr name, Mat2 mat);
        SSDK_FNPTR(void, log3, cstr name, Mat3 mat);
        SSDK_FNPTR(void, log4, cstr name, Mat4 mat);
        
        // Mat2 Operations
        SSDK_FNPTR(Mat2, identity2, void);
        SSDK_FNPTR(Mat2, transpose2, Mat2 mat);
        SSDK_FNPTR(Mat2, mult2, Mat2 a, Mat2 b);
        
        // Mat3 Operations
        SSDK_FNPTR(Mat3, identity3, void);
        SSDK_FNPTR(Mat3, transpose3, Mat3 mat);
        SSDK_FNPTR(Mat3, mult3, Mat3 a, Mat3 b);
    
        // Mat4 Operations
        SSDK_FNPTR(Mat4, identity4, void);
        SSDK_FNPTR(Mat4, transpose4, Mat4 mat);
        SSDK_FNPTR(Mat4, trans4, f32 x, f32 y, f32 z);
        SSDK_FNPTR(Vec3, mult4v3, Mat4 m, Vec3 v);
        SSDK_FNPTR(Mat4, rot4, Vec3 axis, f32 angle);
        SSDK_FNPTR(Mat4, rotx4, f32 angle);
        SSDK_FNPTR(Mat4, roty4, f32 angle);
        SSDK_FNPTR(Mat4, rotz4, f32 angle);
        SSDK_FNPTR(Mat4, scale4, f32 x, f32 y, f32 z);
        SSDK_FNPTR(Mat4, mult4, Mat4 a, Mat4 b);
        SSDK_FNPTR(Mat4, lookat, Vec3 eye, Vec3 center, Vec3 up);
        SSDK_FNPTR(Mat4, ortho, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

        /**
         * symmetric frustum using vertical FOV
        */
        SSDK_FNPTR(Mat4, perspective, f32 fov, f32 aspect, f32 near, f32 far);
    } matrix;
} SaneMath;
extern SaneMath* saneMath;

SSDK_FUNC byte ssdkInitMath(none);
SSDK_FUNC void ssdkExitMath(none);

#endif  // __SANEMATH_H__