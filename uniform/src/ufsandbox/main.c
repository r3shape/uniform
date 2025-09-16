#include <uniform.h>
#include <stdio.h>

UFApplication app;

UFResource shader;
UFResource u_sprite_color;

Vec3 rgb = VEC3(123, 420, 69);

/* HOOKS */
none keyDownHook(UFEventCode code, UFEvent event) {
    switch(event.key) {
        case(UF_KEYBOARD_W): {
            printf("DEVICE(%llu) PRESSED W -- MOVE UP\n", event.device);
        } return;
        case(UF_KEYBOARD_A): {
            printf("DEVICE(%llu) PRESSED A -- MOVE LEFT\n", event.device);
        } return;
        case(UF_KEYBOARD_S): {
            printf("DEVICE(%llu) PRESSED S -- MOVE DOWN\n", event.device);
        } return;
        case(UF_KEYBOARD_D): {
            printf("DEVICE(%llu) PRESSED D -- MOVE RIGHT\n", event.device);
        } return;
        default: return;
    }
}

none appInit(none) {
    app.evinPtr->newHook(UF_EVENT_KEY_PRESSED, keyDownHook);

    shader = app.gpuPtr->newProgram((UFGPUProgramDesc) {
        .type=UF_GPU_VERTEX_PROGRAM,
        .vertex.vertexPath="external/assets/shaders/default/shader.vert",
        .vertex.fragmentPath="external/assets/shaders/default/shader.frag"
    });

    u_sprite_color = app.gpuPtr->setUniform(shader, (UFGPUUniformDesc){
        .type=UF_GPU_UNIFORM_VEC3,
        .data=&rgb,
        .name="u_sprite_color"
    });
}

none appExit(none) {
    app.gpuPtr->delProgram(shader);
}

none appUpdate(none) {
    app.gpuPtr->clearColorBuffer(VEC4(25, 25, 125, 225));

    rgb.data[0] -= 0.1;
    app.gpuPtr->sendUniform(shader, u_sprite_color);
}

none appRender(none) {}

UFApplication* ufExport(none) {
    app = (UFApplication) {
        .ver = 1, .api = 1, .ctx = 1,
        .windowPos=(Vec2){ .data={100, 100} },
        .windowSize=(Vec2){ .data={1280, 720} },
        .init_hook = appInit, .exit_hook = appExit,
        .update_hook = appUpdate, .render_hook = appRender
    };
    return &app;
}
