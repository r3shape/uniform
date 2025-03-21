#include <stdio.h>
#include "../engine/include/r3.h"

u8 running = 1;
u8 quit_callback2(u16 event_code, R3_Event data) {
    u16 key = data.u16[0];
    if (key == R3_KEY_ESCAPE) running = 0;
    return LIBX_TRUE;
}

u8 quit_callback1(u16 event_code, R3_Event data) {
    if (event_code == R3_EVENT_QUIT) running = 0;
    return LIBX_TRUE;
}

u8 resize_callback(u16 event_code, R3_Event data) {
    if (event_code == R3_EVENT_RESIZE) r3_core->graphics.gl.viewport(0, 0, data.u16[0], data.u16[1]);
    return LIBX_TRUE;
}

u8 camera_callback(u16 event_code, R3_Event data) {
    if (event_code == R3_EVENT_MOUSE_MOVE) {
        r3_core->graphics.rotate_camera(data.i16[0], -data.i16[1]);
        return LIBX_TRUE;
    }
    return LIBX_FALSE;
}

int main() {
    r3_init_core();
    
    R3_Window* window = r3_core->platform.create_window("Hello Triangle", 800, 600);
    r3_core->platform.create_gl_context();
    r3_core->graphics.init_gl(&r3_core->graphics);
    
    r3_core->events.register_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.register_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.register_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    r3_core->events.register_callback(R3_EVENT_MOUSE_MOVE, camera_callback);
        
    R3_Shader shader = r3_core->graphics.create_shader(
        filex->read("../engine/assets/shaders/default/shader.vert", 0),
        filex->read("../engine/assets/shaders/default/shader.frag", 0));
    R3_Shader shader2 = r3_core->graphics.create_shader(
        filex->read("../engine/assets/shaders/light/shader.vert", 0),
        filex->read("../engine/assets/shaders/light/shader.frag", 0));

    R3_Texture texture = r3_core->graphics.create_texture2D("../engine/assets/textures/logo.png", R3_RGBA_FORMAT);
    
    f32 speed = 0.02;
    f32 rotation = 0.0;
    Mat4 u_model = mathx->mat.identity4();
    Vec3 location = mathx->vec.vec3(0, 0, 0);
    R3_Vertex_Data vertex_data = r3_core->graphics.create_vertex_data(
        (f32[]){
           -0.5, -0.5, 0.5,    1.0, 0.0, 0.0,    0.0, 0.0,
            0.5, -0.5, 0.5,    0.0, 1.0, 0.0,    1.0, 0.0,
            0.0,  0.5, 0.5,    0.0, 0.0, 1.0,    0.5, 1.0
        }, 3, NULL, 0, R3_LOCATION_ATTR | R3_COLOR_ATTR | R3_TCOORD_ATTR
    );
    
    Mat4 u_model2 = mathx->mat.identity4();

    if (r3_core->graphics.init_pipeline(
        R3_TRIANGLE_MODE, &shader,
        mathx->mat.perspective(90.0, 800/600, 0, 1000)
    )) printf("render pipeline initialized\n");
    else printf("render pipeline failed to be initialized!\n");
    
    if (r3_core->graphics.init_camera(
        mathx->vec.vec3(0, 0, 3),
        mathx->vec.vec3(0, 0,-1),
        mathx->vec.vec3(0, 1, 0)
    )) printf("camera initialized!\n");
    else printf("camera failed to be initialized!\n");
    r3_core->graphics.camera.sensitivity = 0.05;

    while (running) {
        r3_core->platform.poll_events();
        r3_core->platform.poll_inputs();
        
        if (r3_core->input.key_is_down(R3_KEY_A)) r3_core->graphics.translate_camera(-1, 0, 0);
        if (r3_core->input.key_is_down(R3_KEY_D)) r3_core->graphics.translate_camera( 1, 0, 0);
        if (r3_core->input.key_is_down(R3_KEY_W)) r3_core->graphics.translate_camera( 0, 0, 1);
        if (r3_core->input.key_is_down(R3_KEY_S)) r3_core->graphics.translate_camera( 0, 0,-1);
        if (r3_core->input.key_is_down(R3_KEY_SPACE)) r3_core->graphics.translate_camera(0,  1, 0);
        if (r3_core->input.key_is_down(R3_KEY_SHIFT)) r3_core->graphics.translate_camera(0, -1, 0);
        
        u_model = mathx->mat.identity4();
        u_model = mathx->mat.mult4(u_model, mathx->mat.trans4(location.x, location.y, location.z));
        
        rotation += 1;
        rotation = fmod(rotation, 360);
        u_model = mathx->mat.mult4(u_model, mathx->mat.rotx4(rotation));
        u_model = mathx->mat.mult4(u_model, mathx->mat.roty4(rotation));
        u_model = mathx->mat.mult4(u_model, mathx->mat.rotz4(rotation));
        
        r3_core->graphics.push_pipeline(&vertex_data, &u_model, NULL, &texture, R3_TRIANGLE_MODE, R3_RENDER_ARRAYS);
        r3_core->graphics.push_pipeline(&vertex_data, &u_model2, &shader2, &texture, R3_TRIANGLE_MODE, R3_RENDER_ARRAYS);
        
        r3_core->graphics.update_camera();
        r3_core->graphics.flush_pipeline();
        r3_core->platform.swap_buffers();
    }
    
    r3_core->events.unregister_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.unregister_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.unregister_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    r3_core->events.unregister_callback(R3_EVENT_MOUSE_MOVE, camera_callback);
    
    r3_core->platform.destroy_gl_context();
    r3_core->platform.destroy_window(window);

    r3_cleanup_core();
    return 0;
}
