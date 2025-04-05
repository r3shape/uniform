#define R3_MODULES R3_CORE|R3_2D
#include <r3/r3.core/include/r3.core.h>

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

int main() {
    r3_init_core(R3_MODULES);

    R3_Window* window = r3_core->platform.create_window("Hello 2D", 800, 600);
    r3_core->platform.create_gl_context();
    r3_core->graphics.init_gl(&r3_core->graphics);
    
    r3_core->events.register_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.register_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.register_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    
    R3_Shader shader = r3_core->graphics.create_shader(
        filex->read("assets/shaders/default/shader.vert", 0),
        filex->read("assets/shaders/default/shader.frag", 0)
    );

    R3_Texture texture = r3_core->graphics.create_texture2D("assets/textures/logo.png", R3_RGBA_FORMAT);

    if (r3_core->graphics.init_pipeline(
        R3_TRIANGLE_MODE, &shader,
        mathx->mat.ortho(0, window->size[0], 0, window->size[1], 0, 1)
    )) printf("render pipeline initialized\n");
    else printf("render pipeline failed to be initialized!\n");

    if (r3_core->graphics.init_camera(
        mathx->vec.vec3(0, 0, 1),
        mathx->vec.vec3(0, 0,-1),
        mathx->vec.vec3(0, 1, 0)
    )) printf("camera initialized!\n");
    else printf("camera failed to be initialized!\n");
    r3_core->graphics.camera.speed = 3;

    Mat4 u_model = mathx->mat.identity4();
    Vec2 location = mathx->vec.vec2(400, 300);
    u_model = mathx->mat.mult4(u_model, mathx->mat.trans4(location.x, location.y, 0));

    R3_Vertex_Data triangle = r3_2D->shape2D.triangle2D((Vec2){32, 32}, (Vec3){1, 0, 0});

    r3_core->platform.show_cursor();
    while (running) {
        r3_core->platform.poll_events();
        r3_core->platform.poll_inputs();
        
        if (r3_core->input.key_is_down(R3_KEY_F1)) r3_core->graphics.toggle_wireframe(1);
        else r3_core->graphics.toggle_wireframe(0);

        r3_core->graphics.push_pipeline(&triangle, &u_model, NULL, &texture, R3_TRIANGLE_MODE, R3_RENDER_ARRAYS);

        r3_core->graphics.flush_pipeline();
        r3_core->graphics.update_camera();
        r3_core->platform.swap_buffers();
    }
    
    r3_core->events.unregister_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.unregister_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.unregister_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    
    r3_core->platform.destroy_gl_context();
    r3_core->platform.destroy_window();

    r3_cleanup_core();
    return 0;
}
