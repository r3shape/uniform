#include "../../include/graphics/lotus_gl.h"
#include "../../include/graphics/lotus_camera.h"
#include "../../include/platform/lotus_input.h"

Lotus_Camera lotus_init_camera(Lotus_Vec3 location, Lotus_Camera_Mode mode) {
    Lotus_Camera camera = {
    .mode = mode,
        .globalUp = lotus_new_vec3(0, 1.0, 0),
        
        .yaw = -90.0,
        .pitch = 0.0,

        .fov = 45.0,
        .speed = 0.1f,
        .sensitivity = 0.1f,
        
        .location = location
    };
    lotus_update_camera(&camera);
    return camera;
}

void lotus_rotate_camera(Lotus_Camera *camera, f32 deltaX, f32 deltaY) {
    if (!camera) return; // error: null pointer!
    
    camera->yaw += deltaX * camera->sensitivity;
    camera->pitch += deltaY * camera->sensitivity;
    
    camera->yaw = fmod(camera->yaw, 360.0);

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;
}

void lotus_translate_camera(Lotus_Camera *camera, sbyte x, sbyte y, sbyte z) {
    if (!camera) return; // error: null pointer!

    if (x > 0) {
        camera->location = lotus_add_vec3(camera->location, lotus_scale_vec3(lotus_norm_vec3(lotus_cross_vec3(
            camera->direction,
            camera->up
        )), camera->speed));
    }
    else if (x < 0) {
        camera->location = lotus_sub_vec3(camera->location, lotus_scale_vec3(lotus_norm_vec3(lotus_cross_vec3(
            camera->direction,
            camera->up
        )), camera->speed));
    }

    if (y > 0) {
        camera->location = lotus_add_vec3(
            camera->location,
            lotus_scale_vec3(camera->up, camera->speed)
        );
    }
    else if (y < 0) {
        camera->location = lotus_sub_vec3(
            camera->location,
            lotus_scale_vec3(camera->up, camera->speed)
        );
    }
    
    if (z > 0) {
        camera->location = lotus_add_vec3(
            camera->location,
            lotus_scale_vec3(camera->direction, camera->speed)
        );
    }
    else if (z < 0) {
        camera->location = lotus_sub_vec3(
            camera->location,
            lotus_scale_vec3(camera->direction, camera->speed)
        );
    }
}

void lotus_update_camera(Lotus_Camera *camera) {
    if (!camera) return;    // error: null ptr!

    switch(camera->mode) {
        case LOTUS_FLY_CAMERA: {
            sbyte2 x, y;
            lotus_mouse_get_position(&x, &y);
            lotus_rotate_camera(camera, x, -y);        
        } break;
        default: break;
    }

    Lotus_Vec3 direction;
    direction.x = cosf(lotus_to_radians(camera->yaw)) * cosf(lotus_to_radians(camera->pitch));
    direction.y = sinf(lotus_to_radians(camera->pitch));
    direction.z = sinf(lotus_to_radians(camera->yaw)) * cosf(lotus_to_radians(camera->pitch));

    camera->direction = lotus_norm_vec3(direction);
    camera->right = lotus_norm_vec3(lotus_cross_vec3(camera->globalUp, camera->direction));
    camera->up = lotus_norm_vec3(lotus_cross_vec3(camera->direction, camera->right));
    
    camera->view = lotus_look_at(camera->location, lotus_add_vec3(camera->location, camera->direction), camera->up);
    lotus_graphics_api->set_uniform(lotus_graphics_api->get_state()->shader, "u_view", &camera->view);
}
