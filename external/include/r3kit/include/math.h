#ifndef __R3_MATH_H__
#define __R3_MATH_H__

#include <r3kit/include/defines.h>

/*	TYPES	*/
typedef struct Rect { f32 data[4]; } Rect;
typedef struct AABB { f32 data[4]; } AABB;

typedef struct Vec2 { f32 data[2]; } Vec2;
typedef struct Vec3 { f32 data[3]; } Vec3;
typedef struct Vec4 { f32 data[4]; } Vec4;

#define VEC2(x, y) ((Vec2){ .data = {x, y} })
#define VEC3(x, y, z) ((Vec3){ .data = {x, y, z} })
#define VEC4(x, y, z, w) ((Vec4){ .data = {x, y, z, w} })

typedef struct Mat4 { f32 data[16]; } Mat4;

#define IDENTITY() ((Mat4){ .data = {\
	1.0, 0.0, 0.0, 0.0,\
	0.0, 1.0, 0.0, 0.0,\
	0.0, 0.0, 1.0, 0.0,\
	0.0, 0.0, 0.0, 1.0}\
})

// ACCESSORS
#define VEC_X(v) ((v).data[0])
#define VEC_Y(v) ((v).data[1])
#define VEC_Z(v) ((v).data[2])
#define VEC_W(v) ((v).data[3])

#define RECT_X(r) ((r).data[0])
#define RECT_Y(r) ((r).data[1])
#define RECT_W(r) ((r).data[2])
#define RECT_H(r) ((r).data[3])

#define AABB_X(a) ((a).data[0])
#define AABB_Y(a) ((a).data[1])
#define AABB_W(a) ((a).data[2])
#define AABB_H(a) ((a).data[3])

#define MAT4_RC(m, r, c) ((m).data[(r) * 4 + (c)])	// row-col
#define MAT4_CR(m, c, r) ((m).data[(c) * 4 + (r)])	// col-row

// ROW-MAJOR MATRIX AXIS
#define MAT4_X_RM(m) (Vec3){m.data[0], m.data[1], m.data[2]}
#define MAT4_Y_RM(m) (Vec3){m.data[4], m.data[5], m.data[6]}
#define MAT4_Z_RM(m) (Vec3){m.data[8], m.data[9], m.data[10]}

// COLUMN-MAJOR MATRIX AXIS
#define MAT4_X_CM(m) (Vec3){m.data[0], m.data[4], m.data[8]}
#define MAT4_Y_CM(m) (Vec3){m.data[1], m.data[5], m.data[9]}
#define MAT4_Z_CM(m) (Vec3){m.data[2], m.data[6], m.data[10]}

/*	API	*/
R3_API none r3_vec2_log(char* message, Vec2 vec2);
R3_API none r3_vec3_log(char* message, Vec3 vec3);
R3_API none r3_vec4_log(char* message, Vec4 vec4);
R3_API none r3_mat4_log(char* message, Mat4 mat4);


R3_API f32 r3_vec2_mag(Vec2 vec2);
R3_API f32 r3_vec3_mag(Vec3 vec3);
R3_API f32 r3_vec4_mag(Vec4 vec4);


R3_API Vec2 r3_vec2_norm(Vec2 vec2);
R3_API Vec3 r3_vec3_norm(Vec3 vec3);
R3_API Vec4 r3_vec4_norm(Vec4 vec4);


R3_API f32 r3_vec2_dot(Vec2 veca, Vec2 vecb);
R3_API f32 r3_vec3_dot(Vec3 veca, Vec3 vecb);
R3_API f32 r3_vec4_dot(Vec4 veca, Vec4 vecb);


R3_API Vec2 r3_vec2_add(Vec2 veca, Vec2 vecb);
R3_API Vec3 r3_vec3_add(Vec3 veca, Vec3 vecb);
R3_API Vec4 r3_vec4_add(Vec4 veca, Vec4 vecb);


R3_API Vec2 r3_vec2_sub(Vec2 veca, Vec2 vecb);
R3_API Vec3 r3_vec3_sub(Vec3 veca, Vec3 vecb);
R3_API Vec4 r3_vec4_sub(Vec4 veca, Vec4 vecb);


R3_API Vec2 r3_vec2_mul(Vec2 veca, Vec2 vecb);
R3_API Vec3 r3_vec3_mul(Vec3 veca, Vec3 vecb);
R3_API Vec4 r3_vec4_mul(Vec4 veca, Vec4 vecb);


R3_API Vec2 r3_vec2_div(Vec2 veca, Vec2 vecb);
R3_API Vec3 r3_vec3_div(Vec3 veca, Vec3 vecb);
R3_API Vec4 r3_vec4_div(Vec4 veca, Vec4 vecb);


R3_API Vec2 r3_vec2_scale(f32 scale, Vec2 vec2);
R3_API Vec3 r3_vec3_scale(f32 scale, Vec3 vec3);
R3_API Vec4 r3_vec4_scale(f32 scale, Vec4 vec4);


R3_API Vec3 r3_vec3_cross(Vec3 veca, Vec3 vecb);

R3_API Vec3 r3_mat4_mul_vec3(Vec3 vec3, Mat4 mat4);
R3_API Mat4 r3_mat4_mul_mat4(Mat4 mata, Mat4 matb);

R3_API Mat4 r3_mat4_rotate_x(f32 angle);
R3_API Mat4 r3_mat4_rotate_y(f32 angle);
R3_API Mat4 r3_mat4_rotate_z(f32 angle);
R3_API Mat4 r3_mat4_rotate(Vec3 axis, f32 angle);
R3_API Mat4 r3_mat4_scale(Vec3 scale, Mat4 mat4);
R3_API Mat4 r3_mat4_translate(Vec3 translation, Mat4 mat4);

R3_API Mat4 r3_mat4_lookat(Vec3 eye, Vec3 center, Vec3 up);
R3_API Mat4 r3_mat4_perspective(f32 fov, f32 aspect, f32 near, f32 far);
R3_API Mat4 r3_mat4_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

#endif // __R3_MATH_H__
