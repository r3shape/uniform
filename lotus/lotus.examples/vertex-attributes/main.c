#include "../../include/lotus.h"

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

void main() {
    i32 running = lotusInit();

    f32 vertices[] = {
        // uLocation      uColor
        -0.5, -0.5, 0.0,  1.0, 0.0, 0.0,
         0.5, -0.5, 0.0,  0.0, 1.0, 0.0,
         0.0,  0.5, 0.0,  0.0, 0.0, 1.0
    };

    LTshaderProgram shader = ltglShaderProgram(vShader, fShader);
    LTvertexData triangle = ltglVertexData(2, 3, 0, vertices, NULL);

    while (running) {
        ltClearColor();
        ltPumpEvents();

        // handle input-events via-state
        if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) running = 0;
        if (ltIsKeyDown(LOTUS_KEY_R)) ltSetClearColor(1.0, 0.0, 0.0, 1.0);
        if (ltIsKeyDown(LOTUS_KEY_G)) ltSetClearColor(0.0, 1.0, 0.0, 1.0);
        if (ltIsKeyDown(LOTUS_KEY_B)) ltSetClearColor(0.0, 0.0, 1.0, 1.0);

        ltSetShader(&shader);
        ltDraw(&triangle);

        ltInputUpdate(0);
        ltSwapBuffers();
    }; lotusExit();

}
