#pragma once

#include "../utility/lotus_math.h"

typedef struct Lotus_Camera {
    Lotus_Vec3 globalUp;

    f32 yaw;
    f32 roll;
    f32 pitch;
    
    f32 fov;
    f32 speed;
    f32 sensitivity;
    
    f32 farPlane;
    f32 nearPlane;

    Lotus_Vec3 target;
    Lotus_Vec3 location;
    
    Lotus_Vec3 up;
    Lotus_Vec3 right;
    Lotus_Vec3 direction;

    Lotus_Mat4 view;
} Lotus_Camera;

LOTUS_API Lotus_Camera lotus_init_camera(Lotus_Vec3 location, Lotus_Vec3 target);

LOTUS_API void lotus_translate_camera(Lotus_Camera *camera, sbyte x, sbyte y, sbyte z);
LOTUS_API void lotus_rotate_camera(Lotus_Camera *camera, f32 deltaX, f32 deltaY);

LOTUS_API void lotus_update_camera(Lotus_Camera *camera);
LOTUS_API void lotus_freelook_camera(Lotus_Camera *camera);
