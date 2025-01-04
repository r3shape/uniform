#include "../include/utility/lotus_math.h"

float lotus_to_radians(float deg) {
    return deg * LOTUS_PI/180.0;
}

// Creates a Lotus_Vec2
Lotus_Vec2 lotus_new_vec2(float x, float y) {
    return (Lotus_Vec2){x, y};
}

// Scales a Lotus_Vec2 by a scalar value
Lotus_Vec2 lotus_scale_vec2(Lotus_Vec2 v, float scale) {
    return (Lotus_Vec2){v.x * scale, v.y * scale};
}

// Adds two Vec2s
Lotus_Vec2 lotus_add_vec2(Lotus_Vec2 v1, Lotus_Vec2 v2) {
    return (Lotus_Vec2){v1.x + v2.x, v1.y + v2.y};
}

// Subtracts one Lotus_Vec2 from another
Lotus_Vec2 lotus_sub_vec2(Lotus_Vec2 v1, Lotus_Vec2 v2) {
    return (Lotus_Vec2){v1.x - v2.x, v1.y - v2.y};
}

// Dot product of two Vec2s
float lotus_dot_vec2(Lotus_Vec2 v1, Lotus_Vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Normalize a Lotus_Vec2 (make it unit length)
Lotus_Vec2 lotus_norm_vec2(Lotus_Vec2 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    return lotus_scale_vec2(v, 1.0f / length);
}

// Creates a Lotus_Vec3
Lotus_Vec3 lotus_new_vec3(float x, float y, float z) {
    return (Lotus_Vec3){x, y, z};
}
// Scales a vector by a scalar value
Lotus_Vec3 lotus_scale_vec3(Lotus_Vec3 v, float scale) {
    Lotus_Vec3 result;
    result.x = v.x * scale;
    result.y = v.y * scale;
    result.z = v.z * scale;
    return result;
}

// Adds two vectors
Lotus_Vec3 lotus_add_vec3(Lotus_Vec3 v1, Lotus_Vec3 v2) {
    Lotus_Vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// Subtracts one vector from another
Lotus_Vec3 lotus_sub_vec3(Lotus_Vec3 v1, Lotus_Vec3 v2) {
    Lotus_Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

float lotus_dot_vec3(Lotus_Vec3 a, Lotus_Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Normalize a vector (make it unit length)
Lotus_Vec3 lotus_norm_vec3(Lotus_Vec3 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return lotus_scale_vec3(v, 1.0f / length);
}

// Cross product of two vectors
Lotus_Vec3 lotus_cross_vec3(Lotus_Vec3 v1, Lotus_Vec3 v2) {
    Lotus_Vec3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}


// Creates a Lotus_Vec4
Lotus_Vec4 lotus_new_vec4(float x, float y, float z, float w) {
    return (Lotus_Vec4){x, y, z, w};
}

// Scales a Lotus_Vec4 by a scalar value
Lotus_Vec4 lotus_scale_vec4(Lotus_Vec4 v, float scale) {
    return (Lotus_Vec4){v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

// Adds two Vec4s
Lotus_Vec4 lotus_add_vec4(Lotus_Vec4 v1, Lotus_Vec4 v2) {
    return (Lotus_Vec4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

// Subtracts one Lotus_Vec4 from another
Lotus_Vec4 lotus_sub_vec4(Lotus_Vec4 v1, Lotus_Vec4 v2) {
    return (Lotus_Vec4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// Dot product of two Vec4s
float lotus_dot_vec4(Lotus_Vec4 v1, Lotus_Vec4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// Normalize a Lotus_Vec4 (make it unit length)
Lotus_Vec4 lotus_norm_vec4(Lotus_Vec4 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    return lotus_scale_vec4(v, 1.0f / length);
}

Lotus_Mat4 lotus_identity() {
    Lotus_Mat4 result = {0};
    result.m[0] = 1.0f;
    result.m[5] = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;
    return result;
}

Lotus_Vec3 lotus_mul_mat4_vec3(Lotus_Mat4 m, Lotus_Vec3 v) {
    Lotus_Vec3 result;
    result.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
    result.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
    result.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
    return result;
}

Lotus_Mat4 lotus_trans_mat4(float x, float y, float z) {
    Lotus_Mat4 result = lotus_identity();
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
}

Lotus_Mat4 lotus_rot_mat4(float x, float y, float z, float angle) {
    Lotus_Mat4 result = lotus_identity();
    float rad = lotus_to_radians(angle);
    float cos_theta = cosf(rad);
    float sin_theta = sinf(rad);

    float axis_len = sqrtf(x * x + y * y + z * z);
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

Lotus_Mat4 lotus_scale_mat4(float x, float y, float z) {
    Lotus_Mat4 result = lotus_identity();
    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    return result;
}

Lotus_Mat4 lotus_mul_mat4(Lotus_Mat4 a, Lotus_Mat4 b) {
    Lotus_Mat4 result = {0};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result.m[row * 4 + col] += a.m[row * 4 + i] * b.m[i * 4 + col];
            }
        }
    }
    return result;
}

Lotus_Mat4 lotus_ortho(float left, float right, float bottom, float top, float near, float far) {
    Lotus_Mat4 result = lotus_identity();

    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);

    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far + near) / (far - near);

    return result;
}

Lotus_Mat4 lotus_perspective(float fov, float aspect, float near, float far) {
    Lotus_Mat4 result = lotus_identity();
    float tan_half_fov = tanf(fov / 2.0f);
    
    result.m[0] = 1.0f / (aspect * tan_half_fov);
    result.m[5] = 1.0f / tan_half_fov;
    result.m[10] = -(far + near) / (far - near);
    
    result.m[11] = -1.0f;
    result.m[14] = -(2.0f * far * near) / (far - near);
    
    return result;
}

Lotus_Mat4 lotus_look_at(Lotus_Vec3 eye, Lotus_Vec3 center, Lotus_Vec3 up) {
    Lotus_Vec3 f = lotus_norm_vec3(lotus_sub_vec3(center, eye));
    Lotus_Vec3 s = lotus_norm_vec3(lotus_cross_vec3(f, up));
    Lotus_Vec3 u = lotus_cross_vec3(s, f);

    Lotus_Mat4 result = lotus_identity();
    result.m[0] = s.x;
    result.m[4] = s.y;
    result.m[8] = s.z;
    result.m[1] = u.x;
    result.m[5] = u.y;
    result.m[9] = u.z;
    result.m[2] = -f.x;
    result.m[6] = -f.y;
    result.m[10] = -f.z;
    result.m[12] = -lotus_dot_vec3(s, eye);
    result.m[13] = -lotus_dot_vec3(u, eye);
    result.m[14] = lotus_dot_vec3(f, eye);
    return result;
}
