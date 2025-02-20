#include "../../include/graphics/r3_gl.h"
#include "../../include/graphics/r3_camera.h"
#include "../../include/platform/r3_input.h"

R3_Camera r3_init_camera(R3_Vec3 location, R3_Camera_Mode mode) {
    R3_Camera camera = {
    .mode = mode,
        .globalUp = r3_new_vec3(0, 1.0, 0),
        
        .yaw = -90.0,
        .pitch = 0.0,

        .fov = 45.0,
        .speed = 0.1f,
        .sensitivity = 0.1f,
        
        .location = location
    };
    r3_update_camera(&camera);
    return camera;
}

void r3_rotate_camera(R3_Camera *camera, f32 deltaX, f32 deltaY) {
    if (!camera) return; // error: null pointer!
    
    camera->yaw += deltaX * camera->sensitivity;
    camera->pitch += deltaY * camera->sensitivity;
    
    camera->yaw = fmod(camera->yaw, 360.0);

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;
}

void r3_translate_camera(R3_Camera *camera, sbyte x, sbyte y, sbyte z) {
    if (!camera) return; // error: null pointer!

    if (x > 0) {
        camera->location = r3_add_vec3(camera->location, r3_scale_vec3(r3_norm_vec3(r3_cross_vec3(
            camera->direction,
            camera->up
        )), camera->speed));
    }
    else if (x < 0) {
        camera->location = r3_sub_vec3(camera->location, r3_scale_vec3(r3_norm_vec3(r3_cross_vec3(
            camera->direction,
            camera->up
        )), camera->speed));
    }

    if (y > 0) {
        camera->location = r3_add_vec3(
            camera->location,
            r3_scale_vec3(camera->up, camera->speed)
        );
    }
    else if (y < 0) {
        camera->location = r3_sub_vec3(
            camera->location,
            r3_scale_vec3(camera->up, camera->speed)
        );
    }
    
    if (z > 0) {
        camera->location = r3_add_vec3(
            camera->location,
            r3_scale_vec3(camera->direction, camera->speed)
        );
    }
    else if (z < 0) {
        camera->location = r3_sub_vec3(
            camera->location,
            r3_scale_vec3(camera->direction, camera->speed)
        );
    }
}

void r3_update_camera(R3_Camera *camera) {
    if (!camera) return;    // error: null ptr!

    switch(camera->mode) {
        case R3_FLY_CAMERA: {
            sbyte2 x, y;
            r3_mouse_get_position(&x, &y);
            r3_rotate_camera(camera, x, -y);        
        } break;
        default: break;
    }

    R3_Vec3 direction;
    direction.x = cosf(r3_to_radians(camera->yaw)) * cosf(r3_to_radians(camera->pitch));
    direction.y = sinf(r3_to_radians(camera->pitch));
    direction.z = sinf(r3_to_radians(camera->yaw)) * cosf(r3_to_radians(camera->pitch));

    camera->direction = r3_norm_vec3(direction);
    camera->right = r3_norm_vec3(r3_cross_vec3(camera->globalUp, camera->direction));
    camera->up = r3_norm_vec3(r3_cross_vec3(camera->direction, camera->right));
    
    camera->view = r3_look_at(camera->location, r3_add_vec3(camera->location, camera->direction), camera->up);
    r3_graphics_api->set_uniform(r3_graphics_api->get_state()->shader, "u_view", &camera->view);
}
