#include "ltmath.h"

float ltToRadians(float deg) {
    return deg * _PI_/180.0;
}

// Creates a LTvec2
LTvec2 ltNewVec2(float x, float y) {
    return (LTvec2){x, y};
}

// Scales a LTvec2 by a scalar value
LTvec2 ltScaleVec2(LTvec2 v, float scale) {
    return (LTvec2){v.x * scale, v.y * scale};
}

// Adds two Vec2s
LTvec2 ltAddVec2(LTvec2 v1, LTvec2 v2) {
    return (LTvec2){v1.x + v2.x, v1.y + v2.y};
}

// Subtracts one LTvec2 from another
LTvec2 ltSubVec2(LTvec2 v1, LTvec2 v2) {
    return (LTvec2){v1.x - v2.x, v1.y - v2.y};
}

// Dot product of two Vec2s
float ltDotVec2(LTvec2 v1, LTvec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Normalize a LTvec2 (make it unit length)
LTvec2 ltNormVec2(LTvec2 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y);
    return ltScaleVec2(v, 1.0f / length);
}


// Creates a LTvec3
LTvec3 ltNewVec3(float x, float y, float z) {
    return (LTvec3){x, y, z};
}
// Scales a vector by a scalar value
LTvec3 ltScaleVec3(LTvec3 v, float scale) {
    LTvec3 result;
    result.x = v.x * scale;
    result.y = v.y * scale;
    result.z = v.z * scale;
    return result;
}

// Adds two vectors
LTvec3 ltAddVec3(LTvec3 v1, LTvec3 v2) {
    LTvec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// Subtracts one vector from another
LTvec3 ltSubVec3(LTvec3 v1, LTvec3 v2) {
    LTvec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

float ltDotVec3(LTvec3 a, LTvec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Normalize a vector (make it unit length)
LTvec3 ltNormVec3(LTvec3 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return ltScaleVec3(v, 1.0f / length);
}

// Cross product of two vectors
LTvec3 ltCrossVec3(LTvec3 v1, LTvec3 v2) {
    LTvec3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}


// Creates a LTvec4
LTvec4 ltNewVec4(float x, float y, float z, float w) {
    return (LTvec4){x, y, z, w};
}

// Scales a LTvec4 by a scalar value
LTvec4 ltScaleVec4(LTvec4 v, float scale) {
    return (LTvec4){v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

// Adds two Vec4s
LTvec4 ltAddVec4(LTvec4 v1, LTvec4 v2) {
    return (LTvec4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

// Subtracts one LTvec4 from another
LTvec4 ltSubVec4(LTvec4 v1, LTvec4 v2) {
    return (LTvec4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// Dot product of two Vec4s
float ltDotVec4(LTvec4 v1, LTvec4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// Normalize a LTvec4 (make it unit length)
LTvec4 ltNormVec4(LTvec4 v) {
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    return ltScaleVec4(v, 1.0f / length);
}


LTmat4 ltIdentity() {
    LTmat4 result = {1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f};
    return result;
}

LTvec3 ltMulMat4Vec3(LTmat4 m, LTvec3 v) {
    LTvec3 result;
    result.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
    result.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
    result.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
    return result;
}

LTmat4 ltTransMat4(float x, float y, float z) {
    LTmat4 result = ltIdentity();
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
}

LTmat4 ltRotMat4(float x, float y, float z, float angle) {
    LTmat4 result = ltIdentity();
    float rad = ltToRadians(angle);
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

LTmat4 ltScaleMat4(float x, float y, float z) {
    LTmat4 result = ltIdentity();
    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    return result;
}

LTmat4 ltMulMat4(LTmat4 a, LTmat4 b) {
    LTmat4 result = {0};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result.m[row * 4 + col] += a.m[row * 4 + i] * b.m[i * 4 + col];
            }
        }
    }
    return result;
}

LTmat4 ltOrtho(float left, float right, float bottom, float top, float near, float far) {
    LTmat4 result = ltIdentity();
    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (top - bottom);
    result.m[10] = -2.0f / (far - near);
    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(top + bottom) / (top - bottom);
    result.m[14] = -(far + near) / (far - near);
    return result;
}

LTmat4 ltPerspective(float fov, float aspect, float near, float far) {
    float tan_half_fov = tanf(fov / 2.0f);
    LTmat4 result = {0};
    result.m[0] = 1.0f / (aspect * tan_half_fov);
    result.m[5] = 1.0f / tan_half_fov;
    result.m[10] = -(far + near) / (far - near);
    result.m[11] = -1.0f;
    result.m[14] = -(2.0f * far * near) / (far - near);
    return result;
}

LTmat4 ltLookAt(LTvec3 eye, LTvec3 center, LTvec3 up) {
    LTvec3 f = ltNormVec3(ltSubVec3(center, eye));
    LTvec3 s = ltNormVec3(ltCrossVec3(f, up));
    LTvec3 u = ltCrossVec3(s, f);

    LTmat4 result = ltIdentity();
    result.m[0] = s.x;
    result.m[4] = s.y;
    result.m[8] = s.z;
    result.m[1] = u.x;
    result.m[5] = u.y;
    result.m[9] = u.z;
    result.m[2] = -f.x;
    result.m[6] = -f.y;
    result.m[10] = -f.z;
    result.m[12] = -ltDotVec3(s, eye);
    result.m[13] = -ltDotVec3(u, eye);
    result.m[14] = ltDotVec3(f, eye);
    return result;
}
