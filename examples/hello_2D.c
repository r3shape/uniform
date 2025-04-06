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

    f32 speed = 2;
    Mat4 u_model = mathx->mat.identity4();
    Vec2 location = mathx->vec.vec2(400, 300);
    u_model = mathx->mat.mult4(u_model, mathx->mat.trans4(location.x, location.y, 0));

    Mat4 u_model2 = mathx->mat.identity4();
    u_model2 = mathx->mat.mult4(u_model2, mathx->mat.trans4(location.x, location.y, 0));

    R3_Vertex_Data triangle = r3_2D->shape2D.triangle2D((Vec2){32.0, 32.0}, (Vec3){1, 1, 1});

    u32 entity = ecsx->create_entity();
    ecsx->add_component(R3_SPRITE, entity);

    R3_Sprite sprite; ecsx->get_component(R3_SPRITE, entity, &sprite);
    sprite.texture = &texture;

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
        
        if (r3_core->input.key_is_down(R3_KEY_F1)) r3_core->graphics.toggle_wireframe(1);
        else r3_core->graphics.toggle_wireframe(0);

        if (r3_core->input.key_is_down(R3_KEY_A)) location.x -= speed;
        if (r3_core->input.key_is_down(R3_KEY_D)) location.x += speed;
        if (r3_core->input.key_is_down(R3_KEY_W)) location.y += speed;
        if (r3_core->input.key_is_down(R3_KEY_S)) location.y -= speed;
        
        sprite.color->x = LIBX_CLAMP(sprite.color->x + (0.01 * (r3_core->input.key_is_down(R3_KEY_PLUS) - r3_core->input.key_is_down(R3_KEY_MINUS))), 0.0, 1.0);

        if (r3_core->input.key_is_down(R3_KEY_UP))      r3_core->graphics.translate_camera( 0, 1, 0);
        if (r3_core->input.key_is_down(R3_KEY_DOWN))    r3_core->graphics.translate_camera( 0,-1, 0);
        if (r3_core->input.key_is_down(R3_KEY_LEFT))    r3_core->graphics.translate_camera(-1, 0, 0);
        if (r3_core->input.key_is_down(R3_KEY_RIGHT))   r3_core->graphics.translate_camera( 1, 0, 0);

        u_model = mathx->mat.identity4();
        u_model = mathx->mat.mult4(u_model, mathx->mat.trans4(location.x, location.y, 0));
        
        u_model2 = mathx->mat.identity4();
        u_model2 = mathx->mat.mult4(u_model2, mathx->mat.trans4(400, 300, 0));

        r3_core->graphics.push_pipeline(&(R3_Render_Call){
            .vertex = sprite.vertex, .model = &u_model,
            .shader = NULL, .texture = sprite.texture,
            .mode = R3_TRIANGLE_MODE, .type = R3_RENDER_ELEMENTS,
            .uniform_count = 1, .uniforms = (R3_Uniform*[]){
                &(R3_Uniform){
                    .name = "u_sprite_color",
                    .type = R3_UNIFORM_VEC3,
                    .value = sprite.color
                }
            }
        });
        
        r3_core->graphics.push_pipeline(&(R3_Render_Call){
            .vertex = &triangle, .model = &u_model2,
            .shader = NULL, .texture = &texture,
            .mode = R3_TRIANGLE_MODE, .type = R3_RENDER_ARRAYS,
            .uniform_count = 1, .uniforms = (R3_Uniform*[]){
                &(R3_Uniform){
                    .name = "u_sprite_color",
                    .type = R3_UNIFORM_VEC3,
                    .value = &(Vec3){0.0, 1.0, 0.0}
                }
            }
        });

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
