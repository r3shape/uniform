#pragma once

#include "../utility/lotus_math.h"

typedef enum Lotus_Camera_Mode {
    LOTUS_FLY_CAMERA = 0,
    LOTUS_CAMERA_MODES
} Lotus_Camera_Mode;

typedef struct Lotus_Camera {
    Lotus_Vec3 globalUp;

    f32 yaw;
    f32 pitch;
    
    f32 fov;
    f32 speed;
    f32 sensitivity;
    
    Lotus_Vec3 up;
    Lotus_Vec3 right;
    Lotus_Vec3 location;
    Lotus_Vec3 direction;

    Lotus_Mat4 view;

    Lotus_Camera_Mode mode;
} Lotus_Camera;

LOTUS_API Lotus_Camera lotus_init_camera(Lotus_Vec3 location, Lotus_Camera_Mode mode);
LOTUS_API void lotus_update_camera(Lotus_Camera *camera);

LOTUS_API void lotus_rotate_camera(Lotus_Camera *camera, f32 deltaX, f32 deltaY);
LOTUS_API void lotus_translate_camera(Lotus_Camera *camera, sbyte x, sbyte y, sbyte z);

