#include "../../include/lotus2D.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 uLocation;\n"
    "layout(location = 1) in vec3 uColor;\n"
    "layout(location = 2) in vec2 uTCoord;\n"
    "out vec2 texCoord;\n"
    "out vec3 vertexColor;\n"
    "void main() {\n"
    "   gl_Position = vec4(uLocation, 1.0f);\n"
    "   vertexColor = uColor;\n"
    "   texCoord = uTCoord;\n"
    "}\0"
};

char fShader[] = {
    "#version 460 core\n"
    "in vec2 texCoord;\n"
    "in vec3 vertexColor;\n"
    "out vec4 fragColor;\n"
    "uniform sampler2D uTexture;\n"
    "void main() {\n"
    "   fragColor = texture(uTexture, texCoord) * vec4(vertexColor, 1.0f);\n"
    "}\0"
};

b8 resizeCallback(LTeventData data, u16 eventCode, void* sender, void* listener) {
    if (eventCode != LOTUS_EVENT_RESIZE) return 0;
    ltSetViewport(0, 0, data.data.u16[0], data.data.u16[1]);
}

void main() {
    i32 running = lotusInit();

    LT2Dsprite sprite = lt2dMakeSprite(
        (LTvec2){0.32, 0.32},
        (LTvec2){100.0, 100.0},
        (LTvec3){1.0f, 1.0f, 1.0f},
        "textures/container.jpg"
    );
    LTshaderProgram shader = ltglShaderProgram(vShader, fShader);

    // handle engine-events via callback
    ltRegisterEvent(LOTUS_EVENT_RESIZE, 0, resizeCallback);

    while (running) {
        ltClearColor();
        ltPumpEvents();

        // handle input-events via-state
        if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (ltIsKeyDown(LOTUS_KEY_C)) sprite.primitive = lt2dMakeCircle(.25, 32, 1, 1, 1);
        if (ltIsKeyDown(LOTUS_KEY_T)) sprite.primitive = lt2dMakeTriangle(.5, .5, 1, 1, 1);
        if (ltIsKeyDown(LOTUS_KEY_R)) sprite.primitive = lt2dMakeRectangle(.5, .5, 1, 1, 1);

        ltSetShader(&shader);
        lt2dDrawSprite(&sprite);

        ltInputUpdate(0);
        ltSwapBuffers();
    }; lotusExit();
}
