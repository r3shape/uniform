#pragma once

#include <math.h>
#include "../lotustypes.h"
#include "../defines.h"

#define _PI_ 3.14159265358979323846

float ltToRadians(float deg);

LTvec2 ltNewVec2(float x, float y);
LTvec2 ltScaleVec2(LTvec2 v, float scale);
LTvec2 ltAddVec2(LTvec2 v1, LTvec2 v2);
LTvec2 ltSubVec2(LTvec2 v1, LTvec2 v2);
float ltDotVec2(LTvec2 v1, LTvec2 v2);
LTvec2 ltNormVec2(LTvec2 v);

LTvec3 ltNewVec3(float x, float y, float z);
LTvec3 ltScaleVec3(LTvec3 v, float scale);
LTvec3 ltAddVec3(LTvec3 v1, LTvec3 v2);
LTvec3 ltSubVec3(LTvec3 v1, LTvec3 v2);
float ltDotVec3(LTvec3 a, LTvec3 b);
LTvec3 ltNormVec3(LTvec3 v);
LTvec3 ltCrossVec3(LTvec3 v1, LTvec3 v2);

LTvec4 ltNewVec4(float x, float y, float z, float w);
LTvec4 ltScaleVec4(LTvec4 v, float scale);
LTvec4 ltAddVec4(LTvec4 v1, LTvec4 v2);
LTvec4 ltSubVec4(LTvec4 v1, LTvec4 v2);
float ltDotVec4(LTvec4 v1, LTvec4 v2);
LTvec4 ltNormVec4(LTvec4 v);

LTmat4 ltIdentity();
LTmat4 ltMulMat4(LTmat4 a, LTmat4 b);
LTvec3 ltMulMat4Vec3(LTmat4 m, LTvec3 v);
LTmat4 ltTransMat4(float x, float y, float z);
LTmat4 ltScaleMat4(float x, float y, float z);
LTmat4 ltLookAt(LTvec3 eye, LTvec3 center, LTvec3 up);
LTmat4 ltRotMat4(float x, float y, float z, float angle);
LTmat4 ltPerspective(float fov, float aspect, float near, float far);
LTmat4 ltOrtho(float left, float right, float bottom, float top, float near, float far);
