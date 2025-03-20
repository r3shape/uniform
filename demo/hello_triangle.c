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

int main() {
    r3_init_core();
    
    R3_Window* window = r3_core->platform.create_window("Hello Triangle", 800, 600);
    r3_core->platform.create_gl_context();
    r3_core->graphics.init_gl(&r3_core->graphics);
    
    r3_core->events.register_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.register_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.register_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
        
    R3_Shader shader = r3_core->graphics.create_shader(
        filex->read("../engine/assets/shaders/default/default.vert", 0),
        filex->read("../engine/assets/shaders/default/default.frag", 0));

    R3_Texture2D texture = r3_core->graphics.create_texture2D("../engine/assets/textures/logo.png", R3_RGBA_FORMAT);
    
    f32 speed = 0.02;
    f32 rotation = 0.0;
    Mat4 u_model = mathx->mat.identity4();
    Vec2 location = mathx->vec.vec2(12, 8);
    R3_Vertex_Data vertex_data = r3_core->graphics.create_vertex_data(
        (f32[]){
            -0.5, -0.5, 0.5,    1.0, 0.0, 0.0,    0.0, 0.0,
             0.5, -0.5, 0.5,    0.0, 1.0, 0.0,    1.0, 0.0,
             0.0,  0.5, 0.5,    0.0, 0.0, 1.0,    0.5, 1.0
        }, 3, NULL, 0, R3_LOCATION_ATTR | R3_COLOR_ATTR | R3_TCOORD_ATTR
    );
    
    r3_core->graphics.render_begin(
        R3_TRIANGLE_MODE,
        mathx->vec.vec4(60/255.0, 120/255.0, 210/255.0, 255/255.0),
        mathx->mat.ortho(0, window->size[0], 0, window->size[1], 0, 100)
    );
    
    Mat4 u_view = mathx->mat.lookat(
        mathx->vec.vec3(0, 0, 1),
        mathx->vec.vec3(0, 0, 0),
        mathx->vec.vec3(0, 1, 0)
    );
    
    r3_core->graphics.set_uniform(&shader, "u_model", &u_model.m);
    r3_core->graphics.set_uniform(&shader, "u_view", &u_view.m);
    
    while (running) {
        r3_core->platform.poll_events();
        r3_core->platform.poll_inputs();
        r3_core->graphics.render_clear();
        
        if (r3_core->input.key_is_down(R3_KEY_A)) location.x -= speed;
        if (r3_core->input.key_is_down(R3_KEY_D)) location.x += speed;
        if (r3_core->input.key_is_down(R3_KEY_W)) location.y += speed;
        if (r3_core->input.key_is_down(R3_KEY_S)) location.y -= speed;
        
        u_model = mathx->mat.identity4();
        u_model = mathx->mat.mult4(u_model, mathx->mat.scale4(32, 32, 1));
        u_model = mathx->mat.mult4(u_model, mathx->mat.trans4(location.x, location.y, 0));
        
        rotation += 1;
        rotation = fmod(rotation, 360);
        u_model = mathx->mat.mult4(u_model, mathx->mat.rotz4(rotation));
        
        r3_core->graphics.send_uniform(&shader, R3_UNIFORM_MAT4, "u_model");
        r3_core->graphics.send_uniform(&shader, R3_UNIFORM_MAT4, "u_view");
        r3_core->graphics.gl.bind_texture(GL_TEXTURE_2D, texture.id);
        
        r3_core->graphics.render_call(R3_TRIANGLE_MODE, &shader, 3, 0, vertex_data.vao);
        r3_core->graphics.render_end();
        
        r3_core->platform.swap_buffers();
    }
    
    r3_core->events.unregister_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.unregister_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.unregister_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    
    r3_core->platform.destroy_gl_context();
    r3_core->platform.destroy_window(window);

    r3_cleanup_core();
    return 0;
}
