#pragma once

#include <math.h>
#include "../lotustypes.h"
#include "../defines.h"

#define LOTUS_PI 3.14159265358979323846

static f32 LOTUS_IDENTITY_MATRIX[] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
};

LOTUS_API_ENTRY float ltToRadians(float deg);

LOTUS_API_ENTRY LTvec2 ltNewVec2(float x, float y);
LOTUS_API_ENTRY LTvec2 ltScaleVec2(LTvec2 v, float scale);
LOTUS_API_ENTRY LTvec2 ltAddVec2(LTvec2 v1, LTvec2 v2);
LOTUS_API_ENTRY LTvec2 ltSubVec2(LTvec2 v1, LTvec2 v2);
LOTUS_API_ENTRY float ltDotVec2(LTvec2 v1, LTvec2 v2);
LOTUS_API_ENTRY LTvec2 ltNormVec2(LTvec2 v);

LOTUS_API_ENTRY LTvec3 ltNewVec3(float x, float y, float z);
LOTUS_API_ENTRY LTvec3 ltScaleVec3(LTvec3 v, float scale);
LOTUS_API_ENTRY LTvec3 ltAddVec3(LTvec3 v1, LTvec3 v2);
LOTUS_API_ENTRY LTvec3 ltSubVec3(LTvec3 v1, LTvec3 v2);
LOTUS_API_ENTRY float ltDotVec3(LTvec3 a, LTvec3 b);
LOTUS_API_ENTRY LTvec3 ltNormVec3(LTvec3 v);
LOTUS_API_ENTRY LTvec3 ltCrossVec3(LTvec3 v1, LTvec3 v2);

LOTUS_API_ENTRY LTvec4 ltNewVec4(float x, float y, float z, float w);
LOTUS_API_ENTRY LTvec4 ltScaleVec4(LTvec4 v, float scale);
LOTUS_API_ENTRY LTvec4 ltAddVec4(LTvec4 v1, LTvec4 v2);
LOTUS_API_ENTRY LTvec4 ltSubVec4(LTvec4 v1, LTvec4 v2);
LOTUS_API_ENTRY float ltDotVec4(LTvec4 v1, LTvec4 v2);
LOTUS_API_ENTRY LTvec4 ltNormVec4(LTvec4 v);

LOTUS_API_ENTRY LTmat4 ltIdentity();
LOTUS_API_ENTRY LTmat4 ltMulMat4(LTmat4 a, LTmat4 b);
LOTUS_API_ENTRY LTvec3 ltMulMat4Vec3(LTmat4 m, LTvec3 v);
LOTUS_API_ENTRY LTmat4 ltTransMat4(float x, float y, float z);
LOTUS_API_ENTRY LTmat4 ltScaleMat4(float x, float y, float z);
LOTUS_API_ENTRY LTmat4 ltLookAt(LTvec3 eye, LTvec3 center, LTvec3 up);
LOTUS_API_ENTRY LTmat4 ltRotMat4(float x, float y, float z, float angle);
LOTUS_API_ENTRY LTmat4 ltPerspective(float fov, float aspect, float near, float far);
LOTUS_API_ENTRY LTmat4 ltOrtho(float left, float right, float bottom, float top, float near, float far);
