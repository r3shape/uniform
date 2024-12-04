#include "../../include/lotus2D.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 uLocation;\n"
    "layout(location = 1) in vec3 uColor;\n"
    "layout(location = 2) in vec2 uTCoord;\n"
    "out vec2 texCoord;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 uModel;\n"
    "uniform mat4 uView;\n"
    "uniform mat4 uProj;\n"
    "void main() {\n"
    "   gl_Position = uProj * uView * uModel * vec4(uLocation, 1.0f);\n"
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
    i32 running = lt2dInit();

    LT2Dsprite sprite = lt2dMakeSprite(
        (LTvec2){32.0, 32.0},
        (LTvec2){0.0, 0.0},
        (LTvec3){1.0f, 1.0f, 1.0f},
        "textures/container.jpg"
    );
    LTshaderProgram shader = ltglShaderProgram(vShader, fShader);

    // handle engine-events via callback
    ltRegisterEvent(LOTUS_EVENT_RESIZE, 0, resizeCallback);

    LTvec3 eye = {0.0f, 0.0f, 1.0f};
    LTvec3 center = {0.0f, 0.0f, 0.0f};
    LTvec3 up = {0.0f, 1.0f, 0.0f};
    LTmat4 mView = ltLookAt(eye, center, up);

    while (running) {
        ltClearColor();
        ltPumpEvents();

        // handle input-events via-state
        if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (ltIsKeyDown(LOTUS_KEY_C)) sprite.primitive = lt2dMakeCircle(32, 32, 1, 1, 1);
        if (ltIsKeyDown(LOTUS_KEY_T)) sprite.primitive = lt2dMakeTriangle(32.0, 32.0, 1, 1, 1);
        if (ltIsKeyDown(LOTUS_KEY_R)) sprite.primitive = lt2dMakeRectangle(32.0, 32.0, 1, 1, 1);

        sprite.matrix = ltMulMat4(ltIdentity(), ltTransMat4(sprite.location.x + sprite.size.x/2, sprite.location.y + sprite.size.y/2, 0.0f));

        ltglSetUniform(shader, LOTUS_UNIFORM_MAT4, "uView", &mView);
        
        ltSetShader(&shader);
        lt2dDrawSprite(&sprite);

        ltInputUpdate(0);
        ltSwapBuffers();
    }; lt2dExit();
}
