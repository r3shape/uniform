// hello_3D.c
// Now fully driven by corex ecsx!

#define KC_MODULES KC_CORE|KC_3D
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

u8 camera_callback(u16 event_code, KC_Event data) {
    if (event_code == KC_EVENT_MOUSE_MOVE) {
        koncept_core->graphics.rotate_camera(data.i16[0], -data.i16[1]);
        return COREX_TRUE;
    }
    return COREX_FALSE;
}

int main() {
    koncept_init_core(KC_MODULES);
        
    KC_Window* window = koncept_core->platform.create_window("Hello 3D", 800, 600);
    koncept_core->platform.create_gl_context();
    koncept_core->graphics.init_gl(&koncept_core->graphics);
    
    koncept_core->events.register_callback(KC_EVENT_QUIT, quit_callback1);
    koncept_core->events.register_callback(KC_EVENT_RESIZE, resize_callback);
    koncept_core->events.register_callback(KC_EVENT_KEY_PRESSED, quit_callback2);
    koncept_core->events.register_callback(KC_EVENT_MOUSE_MOVE, camera_callback);
    
    struct Light {
        Vec3 ambient;
        Vec3 diffuse;
        Vec3 specular;
        Vec3 location;
    } u_light = {
        .ambient = corex->mathx.vec.vec3(0.5, 0.5, 0.5),
        .diffuse = corex->mathx.vec.vec3(0.3, 0.3, 0.3),
        .specular = corex->mathx.vec.vec3(1.0, 1.0, 1.0),
        .location = corex->mathx.vec.vec3(-2, 0.0, -2*32)
    };
    
    KC_Shader shader = koncept_core->graphics.create_shader(
        corex->filex.read("external/assets/shaders/light/shader.vert", 0),
        corex->filex.read("external/assets/shaders/light/shader.frag", 0)
    );

    KC_Texture texture = koncept_core->graphics.create_texture2D("external/assets/textures/logo.png", KC_RGBA_FORMAT);
    
    u32 entity0 = corex->ecsx.create_entity_with(3, (u8[]){
        KC_TRANSFORM3D,
        KC_MESH3D,
        KC_MATERIAL3D
    });
    
    KC_Mesh3D mesh0; corex->ecsx.get_component(KC_MESH3D, entity0, &mesh0);
    *mesh0.texture = texture;

    KC_Transform3D trans0; corex->ecsx.get_component(KC_TRANSFORM3D, entity0, &trans0);
    *trans0.scale = (Vec3){10, 10, 10};
    
    u32 entity1 = corex->ecsx.create_entity_with(3, (u8[]){
        KC_TRANSFORM3D,
        KC_MESH3D,
        KC_SHADER3D
    });

    KC_Mesh3D mesh1; corex->ecsx.get_component(KC_MESH3D, entity1, &mesh1);
    *mesh1.texture = texture;
    
    KC_Transform3D trans1; corex->ecsx.get_component(KC_TRANSFORM3D, entity1, &trans1);
    *trans1.location = corex->mathx.vec.vec3(0, 0, -32);
    *trans1.scale = (Vec3){5, 5, 5};
    
    koncept_3D->set_shadekonceptD(entity1, 
        koncept_core->graphics.create_shader(
            corex->filex.read("external/assets/shaders/light/source.vert", 0),
            corex->filex.read("external/assets/shaders/light/source.frag", 0)
    ));
    
    koncept_core->graphics.set_uniform(&shader, &(KC_Uniform){.type = KC_UNIFORM_VEC3, .name = "u_light.ambient", .value = &u_light.ambient});
    koncept_core->graphics.set_uniform(&shader, &(KC_Uniform){.type = KC_UNIFORM_VEC3, .name = "u_light.diffuse", .value = &u_light.diffuse});
    koncept_core->graphics.set_uniform(&shader, &(KC_Uniform){.type = KC_UNIFORM_VEC3, .name = "u_light.specular", .value = &u_light.specular});
    koncept_core->graphics.set_uniform(&shader, &(KC_Uniform){.type = KC_UNIFORM_VEC3, .name = "u_light.location", .value = trans1.location});

    koncept_core->graphics.init_pipeline(
        KC_TRIANGLE_MODE, &shader,
        corex->mathx.mat.perspective(60.0, 800/600, 0.1, 1000)
    ); koncept_core->graphics.init_camera(
        corex->mathx.vec.vec3(0, 0, 3),
        corex->mathx.vec.vec3(0, 0, 1),
        corex->mathx.vec.vec3(0, 1, 0)
    );
    
    koncept_core->graphics.camera.speed = 0.2;
    koncept_core->graphics.camera.sensitivity = 0.05;

    koncept_core->platform.hide_cursor();
    koncept_core->platform.center_cursor();
    while (running) {
        koncept_core->platform.poll_events();
        koncept_core->platform.poll_inputs();
        
        mesh0.color->x = COREX_CLAMP(mesh0.color->x + (0.01 * (koncept_core->input.key_is_down(KC_KEY_PLUS) - koncept_core->input.key_is_down(KC_KEY_MINUS))), 0.0, 1.0);
        koncept_core->graphics.toggle_wireframe(koncept_core->input.key_is_down(KC_KEY_F1));

        trans1.location->x += 1.0 * (koncept_core->input.key_is_down(KC_KEY_RIGHT) - koncept_core->input.key_is_down(KC_KEY_LEFT));
        trans1.location->y += 1.0 * (koncept_core->input.key_is_down(KC_KEY_UP) - koncept_core->input.key_is_down(KC_KEY_DOWN));
        
        koncept_core->graphics.translate_camera(
            (koncept_core->input.key_is_down(KC_KEY_D) - koncept_core->input.key_is_down(KC_KEY_A)),
            (koncept_core->input.key_is_down(KC_KEY_SPACE) - koncept_core->input.key_is_down(KC_KEY_SHIFT)),
            (koncept_core->input.key_is_down(KC_KEY_W) - koncept_core->input.key_is_down(KC_KEY_S))
        );

        corex->ecsx.run_systems(KC_TRANSFORM3D);
        corex->ecsx.run_systems(KC_MATERIAL3D);
        corex->ecsx.run_systems(KC_MESH3D);

        koncept_core->graphics.update_camera();
        koncept_core->graphics.flush_pipeline();
        koncept_core->platform.swap_buffers();
    }
    
    koncept_core->events.unregister_callback(KC_EVENT_QUIT, quit_callback1);
    koncept_core->events.unregister_callback(KC_EVENT_RESIZE, resize_callback);
    koncept_core->events.unregister_callback(KC_EVENT_KEY_PRESSED, quit_callback2);
    koncept_core->events.unregister_callback(KC_EVENT_MOUSE_MOVE, camera_callback);
    
    koncept_core->platform.destroy_gl_context();
    koncept_core->platform.destroy_window();

    koncept_cleanup_core();
    return 0;
}