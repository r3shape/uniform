#include "../../include/graphics/r3_gl.h"
#include "../../include/memory/r3_memory.h"
#include "../../include/graphics/r3_camera.h"
#include "../../include/platform/r3_input.h"

R3_Camera internal_camera = {0};

R3_Camera* r3_get_camera(void) { return &internal_camera; }

R3_Camera* r3_init_camera(R3_Vec3 location, R3_Camera_Mode mode) {
    internal_camera.mode = mode;
    internal_camera.globalUp = r3_new_vec3(0, 1.0, 0);
    
    internal_camera.yaw = -90.0;
    internal_camera.pitch = 0.0;

    internal_camera.fov = 45.0;
    internal_camera.speed = 0.1f;
    internal_camera.sensitivity = 0.1f;
    
    internal_camera.location = location;

    r3_update_camera();
    return &internal_camera;
}

void r3_rotate_camera( f32 deltaX, f32 deltaY) {
    internal_camera.yaw += deltaX * internal_camera.sensitivity;
    internal_camera.pitch += deltaY * internal_camera.sensitivity;
    
    internal_camera.yaw = fmod(internal_camera.yaw, 360.0);

    if (internal_camera.pitch > 89.0f) internal_camera.pitch = 89.0f;
    if (internal_camera.pitch < -89.0f) internal_camera.pitch = -89.0f;
}

void r3_translate_camera(sbyte x, sbyte y, sbyte z) {
    if (x > 0) {
        internal_camera.location = r3_add_vec3(internal_camera.location, r3_scale_vec3(r3_norm_vec3(r3_cross_vec3(
            internal_camera.direction,
            internal_camera.up
        )), internal_camera.speed));
    }
    else if (x < 0) {
        internal_camera.location = r3_sub_vec3(internal_camera.location, r3_scale_vec3(r3_norm_vec3(r3_cross_vec3(
            internal_camera.direction,
            internal_camera.up
        )), internal_camera.speed));
    }

    if (y > 0) {
        internal_camera.location = r3_add_vec3(
            internal_camera.location,
            r3_scale_vec3(internal_camera.up, internal_camera.speed)
        );
    }
    else if (y < 0) {
        internal_camera.location = r3_sub_vec3(
            internal_camera.location,
            r3_scale_vec3(internal_camera.up, internal_camera.speed)
        );
    }
    
    if (z > 0) {
        internal_camera.location = r3_add_vec3(
            internal_camera.location,
            r3_scale_vec3(internal_camera.direction, internal_camera.speed)
        );
    }
    else if (z < 0) {
        internal_camera.location = r3_sub_vec3(
            internal_camera.location,
            r3_scale_vec3(internal_camera.direction, internal_camera.speed)
        );
    }
}

void r3_update_camera(void) {
    switch(internal_camera.mode) {
        case R3_FLY_CAMERA: {
            sbyte2 x, y;
            r3_mouse_get_position(&x, &y);
            r3_rotate_camera(x, -y);
        } break;
        default: break;
    }

    R3_Vec3 direction;
    direction.x = cosf(r3_to_radians(internal_camera.yaw)) * cosf(r3_to_radians(internal_camera.pitch));
    direction.y = sinf(r3_to_radians(internal_camera.pitch));
    direction.z = sinf(r3_to_radians(internal_camera.yaw)) * cosf(r3_to_radians(internal_camera.pitch));

    internal_camera.direction = r3_norm_vec3(direction);
    internal_camera.right = r3_norm_vec3(r3_cross_vec3(internal_camera.globalUp, internal_camera.direction));
    internal_camera.up = r3_norm_vec3(r3_cross_vec3(internal_camera.direction, internal_camera.right));
    
    internal_camera.view = r3_look_at(internal_camera.location, r3_add_vec3(internal_camera.location, internal_camera.direction), internal_camera.up);
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_cam_location", &internal_camera.location);
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_view", &internal_camera.view);
}
