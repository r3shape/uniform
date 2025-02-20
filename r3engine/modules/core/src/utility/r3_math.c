#include "../include/utility/r3_math.h"

f32 r3_to_radians(f32 deg) {
    return deg * R3_PI/180.0;
}

// Creates a R3_Vec2
R3_Vec2 r3_new_vec2(f32 x, f32 y) {
    return (R3_Vec2){x, y};
}

// Scales a R3_Vec2 by a scalar value
R3_Vec2 r3_scale_vec2(R3_Vec2 v, f32 scale) {
    return (R3_Vec2){v.x * scale, v.y * scale};
}

// Adds two Vec2s
R3_Vec2 r3_add_vec2(R3_Vec2 v1, R3_Vec2 v2) {
    return (R3_Vec2){v1.x + v2.x, v1.y + v2.y};
}

// Subtracts one R3_Vec2 from another
R3_Vec2 r3_sub_vec2(R3_Vec2 v1, R3_Vec2 v2) {
    return (R3_Vec2){v1.x - v2.x, v1.y - v2.y};
}

// Dot product of two Vec2s
f32 r3_dot_vec2(R3_Vec2 v1, R3_Vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Normalize a R3_Vec2 (make it unit length)
R3_Vec2 r3_norm_vec2(R3_Vec2 v) {
    f32 length = sqrtf(v.x * v.x + v.y * v.y);
    return r3_scale_vec2(v, 1.0f / length);
}

// Creates a R3_Vec3
R3_Vec3 r3_new_vec3(f32 x, f32 y, f32 z) {
    return (R3_Vec3){x, y, z};
}
// Scales a vector by a scalar value
R3_Vec3 r3_scale_vec3(R3_Vec3 v, f32 scale) {
    R3_Vec3 result;
    result.x = v.x * scale;
    result.y = v.y * scale;
    result.z = v.z * scale;
    return result;
}

// Adds two vectors
R3_Vec3 r3_add_vec3(R3_Vec3 v1, R3_Vec3 v2) {
    R3_Vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// Subtracts one vector from another
R3_Vec3 r3_sub_vec3(R3_Vec3 v1, R3_Vec3 v2) {
    R3_Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

f32 r3_dot_vec3(R3_Vec3 a, R3_Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Normalize a vector (make it unit length)
R3_Vec3 r3_norm_vec3(R3_Vec3 v) {
    f32 length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return r3_scale_vec3(v, 1.0f / length);
}

// Cross product of two vectors
R3_Vec3 r3_cross_vec3(R3_Vec3 v1, R3_Vec3 v2) {
    R3_Vec3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}


// Creates a R3_Vec4
R3_Vec4 r3_new_vec4(f32 x, f32 y, f32 z, f32 w) {
    return (R3_Vec4){x, y, z, w};
}

// Scales a R3_Vec4 by a scalar value
R3_Vec4 r3_scale_vec4(R3_Vec4 v, f32 scale) {
    return (R3_Vec4){v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

// Adds two Vec4s
R3_Vec4 r3_add_vec4(R3_Vec4 v1, R3_Vec4 v2) {
    return (R3_Vec4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

// Subtracts one R3_Vec4 from another
R3_Vec4 r3_sub_vec4(R3_Vec4 v1, R3_Vec4 v2) {
    return (R3_Vec4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// Dot product of two Vec4s
f32 r3_dot_vec4(R3_Vec4 v1, R3_Vec4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// Normalize a R3_Vec4 (make it unit length)
R3_Vec4 r3_norm_vec4(R3_Vec4 v) {
    f32 length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    return r3_scale_vec4(v, 1.0f / length);
}

R3_Mat4 r3_identity() {
    R3_Mat4 result = {0};
    result.m[0] = 1.0f;
    result.m[5] = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;
    return result;
}

R3_Vec3 r3_mul_mat4_vec3(R3_Mat4 m, R3_Vec3 v) {
    R3_Vec3 result;
    result.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
    result.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
    result.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
    return result;
}

R3_Mat4 r3_trans_mat4(f32 x, f32 y, f32 z) {
    R3_Mat4 result = r3_identity();
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
}

// arbitrary 3D axis rotation
R3_Mat4 r3_rot_mat4(f32 x, f32 y, f32 z, f32 angle) {
    R3_Mat4 result = r3_identity();
    f32 rad = r3_to_radians(angle);
    f32 cos_theta = cosf(rad);
    f32 sin_theta = sinf(rad);

    f32 axis_len = sqrtf(x * x + y * y + z * z);
    if (axis_len > 0.0f) {
        x /= axis_len;
        y /= axis_len;
        z /= axis_len;
    }

    result.m[0] = cos_theta + (1 - cos_theta) * x * x;
    result.m[1] = (1 - cos_theta) * x * y + sin_theta * z;
    result.m[2] = (1 - cos_theta) * x * z - sin_theta * y;

    result.m[4] = (1 - cos_theta) * y * x - sin_theta * z;
    result.m[5] = cos_theta + (1 - cos_theta) * y * y;
    result.m[6] = (1 - cos_theta) * y * z + sin_theta * x;

    result.m[8] = (1 - cos_theta) * z * x + sin_theta * y;
    result.m[9] = (1 - cos_theta) * z * y - sin_theta * x;
    result.m[10] = cos_theta + (1 - cos_theta) * z * z;

    return result;
}

R3_Mat4 r3_rotx_mat4(f32 angle) {
    R3_Mat4 rotx = r3_identity();
    f32 rad = r3_to_radians(angle);
    f32 cos_angle = cosf(rad);
    f32 sin_angle = sin(rad);

    rotx.m[0] = 1.0;
    
    rotx.m[5] = cos_angle;
    rotx.m[6] = sin_angle;
    
    rotx.m[9] = -sin_angle;
    rotx.m[10] = cos_angle;

    return rotx;
}

R3_Mat4 r3_roty_mat4(f32 angle) {
    R3_Mat4 roty = r3_identity();
    f32 rad = r3_to_radians(angle);
    f32 cos_angle = cosf(rad);
    f32 sin_angle = sin(rad);

    roty.m[0] = cos_angle;
    roty.m[2] = -sin_angle;
    
    roty.m[5] = 1.0;
    
    roty.m[8] = sin_angle;
    roty.m[10] = cos_angle;

    return roty;
}

R3_Mat4 r3_rotz_mat4(f32 angle) {
    R3_Mat4 rotz = r3_identity();
    f32 rad = r3_to_radians(angle);
    f32 cos_angle = cosf(rad);
    f32 sin_angle = sin(rad);

    rotz.m[0] = cos_angle;
    rotz.m[1] = sin_angle;
    
    rotz.m[4] = -sin_angle;
    rotz.m[5] = cos_angle;
    
    rotz.m[10] = 1.0;
    rotz.m[15] = 1.0;

    return rotz;
}

R3_Mat4 r3_scale_mat4(f32 x, f32 y, f32 z) {
    R3_Mat4 result = r3_identity();
    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    return result;
}

R3_Mat4 r3_mul_mat4(R3_Mat4 a, R3_Mat4 b) {
    R3_Mat4 result = {0};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result.m[row * 4 + col] += a.m[row * 4 + i] * b.m[i * 4 + col];
            }
        }
    }
    return result;
}

R3_Mat4 r3_ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    R3_Mat4 result = r3_identity();

    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);

    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far + near) / (far - near);

    return result;
}

R3_Mat4 r3_perspective(f32 fov, f32 aspect, f32 near, f32 far) {
    R3_Mat4 result = r3_identity();
    f32 tan_half_fov = tanf(fov / 2.0f);
    
    result.m[0] = 1.0f / (aspect * tan_half_fov);
    result.m[5] = 1.0f / tan_half_fov;
    result.m[10] = -(far + near) / (far - near);
    
    result.m[11] = -1.0f;
    result.m[14] = -(2.0f * far * near) / (far - near);
    
    return result;
}

R3_Mat4 r3_look_at(R3_Vec3 eye, R3_Vec3 center, R3_Vec3 up) {
    R3_Vec3 directionVec = r3_norm_vec3(r3_sub_vec3(center, eye));
    R3_Vec3 rightVec = r3_norm_vec3(r3_cross_vec3(directionVec, up));
    R3_Vec3 upVec = r3_cross_vec3(rightVec, directionVec);
    
    R3_Mat4 result = r3_identity();
    result.m[0] = rightVec.x;
    result.m[4] = rightVec.y;
    result.m[8] = rightVec.z;
    
    result.m[1] = upVec.x;
    result.m[5] = upVec.y;
    result.m[9] = upVec.z;

    result.m[2] = -directionVec.x;
    result.m[6] = -directionVec.y;
    result.m[10] = -directionVec.z;
    
    result.m[12] = -r3_dot_vec3(rightVec, eye);
    result.m[13] = -r3_dot_vec3(upVec, eye);
    result.m[14] = r3_dot_vec3(directionVec, eye);
    return result;
}
