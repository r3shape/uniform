#include "../../include/lotus2D.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 uLocation;\n"
    "layout(location = 1) in vec3 uColor;\n"
    "out vec3 vertexColor;\n"
    "void main() {\n"
    "   gl_Position = vec4(uLocation, 1.0f);\n"
    "   vertexColor = uColor;\n"
    "}\0"
};

char fShader[] = {
    "#version 460 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "   fragColor = vec4(vertexColor, 1.0f);\n"
    "}\0"
};

b8 resizeCallback(LTeventData data, u16 eventCode, void* sender, void* listener) {
    if (eventCode != LOTUS_EVENT_RESIZE) return 0;
    ltSetViewport(0, 0, data.data.u16[0], data.data.u16[1]);
}

void main() {
    i32 running = lotusInit();

    LTshaderProgram shader = ltglShaderProgram(vShader, fShader);
    LT2Dprimitive circle = lt2dMakeCircle(0.25f, 32, 0.0f, 0.0f, 1.0f); // Blue circle
    LT2Dprimitive triangle = lt2dMakeTriangle(0.5, 0.5, 1.0f, 0.0f, 0.0f);  // Red triangle
    LT2Dprimitive rectangle = lt2dMakeRectangle(0.5, 0.5, 0.0f, 1.0f, 0.0f); // Green rectangle

    // "dynamic" draw target
    LT2Dprimitive* target = &circle;

    // handle engine-events via callback
    ltRegisterEvent(LOTUS_EVENT_RESIZE, 0, resizeCallback);

    while (running) {
        ltClearColor();
        ltPumpEvents();

        // handle input-events via-state
        if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (ltIsKeyDown(LOTUS_KEY_R)) {
            target = &circle;
            ltSetClearColor(1.0, 0.0, 0.0, 1.0);
        }

        if (ltIsKeyDown(LOTUS_KEY_G)) {
            target = &triangle;
            ltSetClearColor(0.0, 1.0, 0.0, 1.0);
        }
        
        if (ltIsKeyDown(LOTUS_KEY_B)) {
            target = &rectangle;
            ltSetClearColor(0.0, 0.0, 1.0, 1.0);
        }

        ltSetShader(&shader);
        if (target != NULL) ltDraw(&target->vertexData);

        ltInputUpdate(0);
        ltSwapBuffers();
    }; lotusExit();
}
