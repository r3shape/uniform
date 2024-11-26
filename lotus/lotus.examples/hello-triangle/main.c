#include "../../include/lotus.h"

b8 resizeCallback(LTeventData data, u16 eventCode, void* sender, void* listener) {
    if (eventCode != LOTUS_EVENT_RESIZE) return 0;
    ltSetViewport(0, 0, data.data.u16[0], data.data.u16[1]);
}

void main() {
    i32 running = lotusInit();

    f32 vertices[] = {
        -0.5, -0.5, 0.0,
         0.5, -0.5, 0.0,
         0.0,  0.5, 0.0
    };

    LTvertexData triangle = ltglVertexData(1, 3, 0, vertices, NULL);

    // handle engine-events via-callback
    ltRegisterEvent(LOTUS_EVENT_RESIZE, 0, resizeCallback);

    while (running) {
        ltClearColor();
        ltPumpEvents();

        // handle input-events via-state
        if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) running = 0;
        if (ltIsKeyDown(LOTUS_KEY_R)) ltSetClearColor(1.0, 0.0, 0.0, 1.0);
        if (ltIsKeyDown(LOTUS_KEY_G)) ltSetClearColor(0.0, 1.0, 0.0, 1.0);
        if (ltIsKeyDown(LOTUS_KEY_B)) ltSetClearColor(0.0, 0.0, 1.0, 1.0);

        ltDraw(&triangle);

        ltInputUpdate(0);
        ltSwapBuffers();
    }; lotusExit();

}
