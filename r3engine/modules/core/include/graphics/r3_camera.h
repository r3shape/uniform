#pragma once

#include "../utility/r3_math.h"

typedef enum R3_Camera_Mode {
    R3_FLY_CAMERA = 0,
    R3_CAMERA_MODES
} R3_Camera_Mode;

typedef struct R3_Camera {
    R3_Vec3 globalUp;

    f32 yaw;
    f32 pitch;
    
    f32 fov;
    f32 speed;
    f32 sensitivity;
    
    R3_Vec3 up;
    R3_Vec3 right;
    R3_Vec3 location;
    R3_Vec3 direction;

    R3_Mat4 view;

    R3_Camera_Mode mode;
} R3_Camera;

R3_API R3_Camera* r3_init_camera(R3_Vec3 location, R3_Camera_Mode mode);
R3_API R3_Camera* r3_get_camera(void);
R3_API void r3_update_camera();

R3_API void r3_rotate_camera(f32 deltaX, f32 deltaY);
R3_API void r3_translate_camera(sbyte x, sbyte y, sbyte z);
