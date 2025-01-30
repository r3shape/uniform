#include "../../include/graphics/lotus_camera.h"
#include "../../include/platform/lotus_input.h"


Lotus_Camera lotus_init_camera(Lotus_Vec3 location, Lotus_Vec3 target) {
    Lotus_Camera cam = {
        .globalUp = lotus_new_vec3(0, 1.0, 0),
        
        .yaw = -90.0,
        .roll = 0.0,
        .pitch = 0.0,

        .fov = 45.0,
        .speed = 0.1f,
        .sensitivity = 0.1f,

        .nearPlane = 0.1f,
        .farPlane =  1000.0f,

        .location = location,
        .target = target
    };

    cam.direction = lotus_new_vec3(0, 0, -1);

    cam.right = lotus_norm_vec3(lotus_cross_vec3(cam.direction, cam.globalUp));
    cam.up = lotus_cross_vec3(cam.right, cam.direction);
    
    cam.view = lotus_look_at(cam.location, lotus_add_vec3(cam.location, cam.direction), cam.up);

    return cam;
}

void lotus_update_camera(Lotus_Camera *camera) {
    if (!camera) return;    // error: null ptr!

    Lotus_Vec3 direction;
    direction.x = cosf(lotus_to_radians(camera->yaw)) * cosf(lotus_to_radians(camera->pitch));
    direction.y = sinf(lotus_to_radians(camera->pitch));
    direction.z = sinf(lotus_to_radians(camera->yaw)) * cosf(lotus_to_radians(camera->pitch));

    camera->direction = lotus_norm_vec3(direction);

    camera->right = lotus_norm_vec3(lotus_cross_vec3(camera->direction, camera->globalUp));
    camera->up = lotus_cross_vec3(camera->right, camera->direction);
    
    camera->view = lotus_look_at(camera->location, lotus_add_vec3(camera->location, camera->direction), camera->up);
}

void lotus_freelook_camera(Lotus_Camera *camera) {
    if (!camera) return; // error: null pointer!
    sbyte2 x, y;
    lotus_mouse_get_position(&x, &y);
    lotus_rotate_camera(camera, x, -y);
}

void lotus_translate_camera(Lotus_Camera *camera, sbyte x, sbyte y, sbyte z) {
    if (!camera) return; // error: null pointer!

    if (x > 0) camera->location = lotus_add_vec3(camera->location, lotus_scale_vec3(lotus_norm_vec3(lotus_cross_vec3(camera->direction, camera->up)), camera->speed));
    else if (x < 0) camera->location = lotus_sub_vec3(camera->location, lotus_scale_vec3(lotus_norm_vec3(lotus_cross_vec3(camera->direction, camera->up)), camera->speed));

    if (y > 0) camera->location = lotus_add_vec3(camera->location, lotus_scale_vec3(camera->up, camera->speed));
    else if (y < 0) camera->location = lotus_sub_vec3(camera->location, lotus_scale_vec3(camera->up, camera->speed));
    
    if (z > 0) camera->location = lotus_add_vec3(camera->location, lotus_scale_vec3(camera->direction, camera->speed));
    else if (z < 0) camera->location = lotus_sub_vec3(camera->location, lotus_scale_vec3(camera->direction, camera->speed));
}

void lotus_rotate_camera(Lotus_Camera *camera, f32 deltaX, f32 deltaY) {
    if (!camera) return; // error: null pointer!

    camera->yaw += deltaX * camera->sensitivity;
    camera->pitch += deltaY * camera->sensitivity;

    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;
}

