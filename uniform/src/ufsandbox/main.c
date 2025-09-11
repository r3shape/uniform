#include <uniform.h>
#include <stdio.h>

UFApplication app;

UFResource shader;

none appInit(none) {
    shader = app.gpuPtr->newProgram((UFGPUProgramDesc) {
        .type=UF_GPU_VERTEX_PROGRAM,
        .vertex.vertexPath="external/assets/shaders/default/shader.vert",
        .vertex.fragmentPath="external/assets/shaders/default/shader.frag"
    });
}

none appExit(none) {}

none appUpdate(none) {}

none appRender(none) {
    app.gpuPtr->clearColorBuffer(VEC4(25, 25, 125, 225));
}

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
