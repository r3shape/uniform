// hello_2D.c
// Now fully driven by libx ecsx!

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

    R3_Window* window = r3_core->platform.create_window("Hello Triangle", 800, 600);
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

    u32 entity0 = ecsx->create_entity_with(2, (u8[]){
        R3_SPRITE2D,
        R3_TRANSFORM2D
    });

    R3_Sprite2D sprite0; ecsx->get_component(R3_SPRITE2D, entity0, &sprite0);
    *sprite0.texture = texture;
    r3_core->graphics.destroy_vertex_data(sprite0.vertex);
    *sprite0.vertex = r3_2D->shape2D.triangle2D((Vec2){32.0, 32.0}, (Vec3){0, 1, 0});

    R3_Transform2D trans0; ecsx->get_component(R3_TRANSFORM2D, entity0, &trans0);
    *trans0.location = (Vec2){400, 300};

    u32 entity1 = ecsx->create_entity_with(2, (u8[]){
        R3_SPRITE2D,
        R3_TRANSFORM2D
    });
    
    R3_Sprite2D sprite1; ecsx->get_component(R3_SPRITE2D, entity1, &sprite1);
    *sprite1.texture = texture;
    *sprite1.color = (Vec3){1.0, 1.0, 1.0};
    
    R3_Transform2D trans1; ecsx->get_component(R3_TRANSFORM2D, entity1, &trans1);
    *trans1.location = (Vec2){400, 300};

    r3_core->graphics.init_pipeline(
        R3_TRIANGLE_MODE, &shader,
        mathx->mat.ortho(0, window->size[0], 0, window->size[1], 0, 1)
    ); r3_core->graphics.init_camera(
        mathx->vec.vec3(0, 0, 1),
        mathx->vec.vec3(0, 0,-1),
        mathx->vec.vec3(0, 1, 0)
    );

    r3_core->graphics.camera.speed = 3;
    
    while (running) {
        r3_core->platform.poll_events();
        r3_core->platform.poll_inputs();
        
        r3_core->graphics.toggle_wireframe(r3_core->input.key_is_down(R3_KEY_F1));
        
        sprite0.color->x = LIBX_CLAMP(sprite0.color->x + (0.01 * (r3_core->input.key_is_down(R3_KEY_PLUS) - r3_core->input.key_is_down(R3_KEY_MINUS))), 0.0, 1.0);
        
        trans0.velocity->x = (r3_core->input.key_is_down(R3_KEY_D) - r3_core->input.key_is_down(R3_KEY_A));
        trans0.velocity->y = (r3_core->input.key_is_down(R3_KEY_W) - r3_core->input.key_is_down(R3_KEY_S));
        
        r3_core->graphics.translate_camera(
            (r3_core->input.key_is_down(R3_KEY_RIGHT) - r3_core->input.key_is_down(R3_KEY_LEFT)),
            (r3_core->input.key_is_down(R3_KEY_UP) - r3_core->input.key_is_down(R3_KEY_DOWN)),
            0
        );

        ecsx->run_systems(R3_TRANSFORM2D);
        ecsx->run_systems(R3_SPRITE2D);

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