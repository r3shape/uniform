// hello_3D.c
// Now fully driven by libx ecsx!

#define R3_MODULES R3_CORE|R3_3D
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

u8 camera_callback(u16 event_code, R3_Event data) {
    if (event_code == R3_EVENT_MOUSE_MOVE) {
        r3_core->graphics.rotate_camera(data.i16[0], -data.i16[1]);
        return LIBX_TRUE;
    }
    return LIBX_FALSE;
}

int main() {
    r3_init_core(R3_MODULES);
        
    R3_Window* window = r3_core->platform.create_window("Hello 3D", 800, 600);
    r3_core->platform.create_gl_context();
    r3_core->graphics.init_gl(&r3_core->graphics);
    
    r3_core->events.register_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.register_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.register_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    r3_core->events.register_callback(R3_EVENT_MOUSE_MOVE, camera_callback);
    
    struct Light {
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
        Vec3 location;
    } u_light = {
        .ambient = mathx->vec.vec3(0.5, 0.5, 0.5),
        .diffuse = mathx->vec.vec3(0.3, 0.3, 0.3),
        .specular = mathx->vec.vec3(1.0, 1.0, 1.0),
        .location = mathx->vec.vec3(-2, 0.0, -2*32)
    };
    
    R3_Shader shader = r3_core->graphics.create_shader(
        filex->read("assets/shaders/light/shader.vert", 0),
        filex->read("assets/shaders/light/shader.frag", 0)
    );

    R3_Texture texture = r3_core->graphics.create_texture2D("assets/textures/logo.png", R3_RGBA_FORMAT);
    
    u32 entity0 = ecsx->create_entity_with(3, (u8[]){
        R3_TRANSFORM3D,
        R3_MESH3D,
        R3_MATERIAL3D
    });
    
    R3_Mesh3D mesh0; ecsx->get_component(R3_MESH3D, entity0, &mesh0);
    *mesh0.texture = texture;

    R3_Transform3D trans0; ecsx->get_component(R3_TRANSFORM3D, entity0, &trans0);
    *trans0.scale = (Vec3){10, 10, 10};
    
    u32 entity1 = ecsx->create_entity_with(3, (u8[]){
        R3_TRANSFORM3D,
        R3_MESH3D,
        R3_SHADER3D
    });

    R3_Mesh3D mesh1; ecsx->get_component(R3_MESH3D, entity1, &mesh1);
    *mesh1.texture = texture;
    
    R3_Transform3D trans1; ecsx->get_component(R3_TRANSFORM3D, entity1, &trans1);
    *trans1.location = mathx->vec.vec3(0, 0, -32);
    *trans1.scale = (Vec3){5, 5, 5};
    
    r3_3D->set_shader3D(entity1, 
        r3_core->graphics.create_shader(
            filex->read("assets/shaders/light/source.vert", 0),
            filex->read("assets/shaders/light/source.frag", 0)
    ));
    
    r3_core->graphics.set_uniform(&shader, &(R3_Uniform){.type = R3_UNIFORM_VEC3, .name = "u_light.ambient", .value = &u_light.ambient});
    r3_core->graphics.set_uniform(&shader, &(R3_Uniform){.type = R3_UNIFORM_VEC3, .name = "u_light.diffuse", .value = &u_light.diffuse});
    r3_core->graphics.set_uniform(&shader, &(R3_Uniform){.type = R3_UNIFORM_VEC3, .name = "u_light.specular", .value = &u_light.specular});
    r3_core->graphics.set_uniform(&shader, &(R3_Uniform){.type = R3_UNIFORM_VEC3, .name = "u_light.location", .value = trans1.location});

    r3_core->graphics.init_pipeline(
        R3_TRIANGLE_MODE, &shader,
        mathx->mat.perspective(60.0, 800/600, 0.1, 1000)
    ); r3_core->graphics.init_camera(
        mathx->vec.vec3(0, 0, 3),
        mathx->vec.vec3(0, 0, 1),
        mathx->vec.vec3(0, 1, 0)
    );
    
    r3_core->graphics.camera.speed = 0.2;
    r3_core->graphics.camera.sensitivity = 0.05;

    r3_core->platform.hide_cursor();
    r3_core->platform.center_cursor();
    while (running) {
        r3_core->platform.poll_events();
        r3_core->platform.poll_inputs();
        
        mesh0.color->x = LIBX_CLAMP(mesh0.color->x + (0.01 * (r3_core->input.key_is_down(R3_KEY_PLUS) - r3_core->input.key_is_down(R3_KEY_MINUS))), 0.0, 1.0);
        r3_core->graphics.toggle_wireframe(r3_core->input.key_is_down(R3_KEY_F1));

        trans1.location->x += 1.0 * (r3_core->input.key_is_down(R3_KEY_RIGHT) - r3_core->input.key_is_down(R3_KEY_LEFT));
        trans1.location->y += 1.0 * (r3_core->input.key_is_down(R3_KEY_UP) - r3_core->input.key_is_down(R3_KEY_DOWN));
        
        r3_core->graphics.translate_camera(
            (r3_core->input.key_is_down(R3_KEY_D) - r3_core->input.key_is_down(R3_KEY_A)),
            (r3_core->input.key_is_down(R3_KEY_SPACE) - r3_core->input.key_is_down(R3_KEY_SHIFT)),
            (r3_core->input.key_is_down(R3_KEY_W) - r3_core->input.key_is_down(R3_KEY_S))
        );

        ecsx->run_systems(R3_TRANSFORM3D);
        ecsx->run_systems(R3_MATERIAL3D);
        ecsx->run_systems(R3_MESH3D);

        r3_core->graphics.update_camera();
        r3_core->graphics.flush_pipeline();
        r3_core->platform.swap_buffers();
    }
    
    r3_core->events.unregister_callback(R3_EVENT_QUIT, quit_callback1);
    r3_core->events.unregister_callback(R3_EVENT_RESIZE, resize_callback);
    r3_core->events.unregister_callback(R3_EVENT_KEY_PRESSED, quit_callback2);
    r3_core->events.unregister_callback(R3_EVENT_MOUSE_MOVE, camera_callback);
    
    r3_core->platform.destroy_gl_context();
    r3_core->platform.destroy_window();

    r3_cleanup_core();
    return 0;
}