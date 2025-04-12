// hello_2D.c
// Now fully driven by corex ecsx!

#define KC_MODULES KC_CORE|KC_2D
#include <koncept/core/koncept_core.h>

u8 running = 1;
u8 quit_callback2(u16 event_code, KC_Event data) {
    u16 key = data.u16[0];
    if (key == KC_KEY_ESCAPE) running = 0;
    return COREX_TRUE;
}

u8 quit_callback1(u16 event_code, KC_Event data) {
    if (event_code == KC_EVENT_QUIT) running = 0;
    return COREX_TRUE;
}

u8 resize_callback(u16 event_code, KC_Event data) {
    if (event_code == KC_EVENT_RESIZE) koncept_core->graphics.gl.viewport(0, 0, data.u16[0], data.u16[1]);
    return COREX_TRUE;
}

int main() {
    printf("KC Initialized: %d\n", koncept_init_core(KC_MODULES));
    KC_Window* window = koncept_core->platform.create_window("Hello Triangle", 800, 600);
    koncept_core->platform.create_gl_context();
    koncept_core->graphics.init_gl(&koncept_core->graphics);
    
    koncept_core->events.register_callback(KC_EVENT_QUIT, quit_callback1);
    koncept_core->events.register_callback(KC_EVENT_RESIZE, resize_callback);
    koncept_core->events.register_callback(KC_EVENT_KEY_PRESSED, quit_callback2);
    
    KC_Shader shader = koncept_core->graphics.create_shader(
        corex->filex.read("external/assets/shaders/default/shader.vert", 0),
        corex->filex.read("external/assets/shaders/default/shader.frag", 0)
    );
    
    KC_Texture texture = koncept_core->graphics.create_texture2D("external/assets/textures/logo.png", KC_RGBA_FORMAT);
    
    u32 entity0 = corex->ecsx.create_entity_with(2, (u8[]){
        KC_SPRITE2D,
        KC_TRANSFORM2D
    });

    KC_Sprite2D sprite0; corex->ecsx.get_component(KC_SPRITE2D, entity0, &sprite0);
    *sprite0.texture = texture;
    koncept_core->graphics.destroy_vertex_data(sprite0.vertex);
    *sprite0.vertex = koncept_2D->shape2D.triangle2D((Vec2){32.0, 32.0}, (Vec3){0, 1, 0});

    KC_Transform2D trans0; corex->ecsx.get_component(KC_TRANSFORM2D, entity0, &trans0);
    *trans0.location = (Vec2){400, 300};

    u32 entity1 = corex->ecsx.create_entity_with(2, (u8[]){
        KC_SPRITE2D,
        KC_TRANSFORM2D
    });
    
    KC_Sprite2D sprite1; corex->ecsx.get_component(KC_SPRITE2D, entity1, &sprite1);
    *sprite1.texture = texture;
    *sprite1.color = (Vec3){1.0, 1.0, 1.0};
    
    KC_Transform2D trans1; corex->ecsx.get_component(KC_TRANSFORM2D, entity1, &trans1);
    *trans1.location = (Vec2){400, 300};

    koncept_core->graphics.init_pipeline(
        KC_TRIANGLE_MODE, &shader,
        corex->mathx.mat.ortho(0, window->size[0], 0, window->size[1], 0, 1)
    ); koncept_core->graphics.init_camera(
        corex->mathx.vec.vec3(0, 0, 1),
        corex->mathx.vec.vec3(0, 0,-1),
        corex->mathx.vec.vec3(0, 1, 0)
    );

    koncept_core->graphics.camera.speed = 3;
    
    while (running) {
        koncept_core->platform.poll_events();
        koncept_core->platform.poll_inputs();
        
        koncept_core->graphics.toggle_wireframe(koncept_core->input.key_is_down(KC_KEY_F1));
        
        sprite0.color->x = COREX_CLAMP(sprite0.color->x + (0.01 * (koncept_core->input.key_is_down(KC_KEY_PLUS) - koncept_core->input.key_is_down(KC_KEY_MINUS))), 0.0, 1.0);
        
        trans0.velocity->x = (koncept_core->input.key_is_down(KC_KEY_D) - koncept_core->input.key_is_down(KC_KEY_A));
        trans0.velocity->y = (koncept_core->input.key_is_down(KC_KEY_W) - koncept_core->input.key_is_down(KC_KEY_S));
        
        koncept_core->graphics.translate_camera(
            (koncept_core->input.key_is_down(KC_KEY_RIGHT) - koncept_core->input.key_is_down(KC_KEY_LEFT)),
            (koncept_core->input.key_is_down(KC_KEY_UP) - koncept_core->input.key_is_down(KC_KEY_DOWN)),
            0
        );

        corex->ecsx.run_systems(KC_TRANSFORM2D);
        corex->ecsx.run_systems(KC_SPRITE2D);

        koncept_core->graphics.flush_pipeline();
        koncept_core->graphics.update_camera();
        koncept_core->platform.swap_buffers();
    }
    
    koncept_core->events.unregister_callback(KC_EVENT_QUIT, quit_callback1);
    koncept_core->events.unregister_callback(KC_EVENT_RESIZE, resize_callback);
    koncept_core->events.unregister_callback(KC_EVENT_KEY_PRESSED, quit_callback2);
    
    koncept_core->platform.destroy_gl_context();
    koncept_core->platform.destroy_window();

    koncept_cleanup_core();
    return 0;
}