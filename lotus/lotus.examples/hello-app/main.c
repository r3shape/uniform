#include "../../include/lotus.h"
#include "../../lotus.application/ltappentry.h"

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
    "out vec4 fragColor;\n"
    "in vec3 vertexColor;\n"
    "void main() {\n"
    "   fragColor = vec4(vertexColor, 1.0f);\n"
    "}\0"
};

LTvertexData triangle;
LTshaderProgram shader;

b8 appInit (struct LTapplication* app) {
    f32 vertices[] = {
        // uLocation      uColor
        -0.5, -0.5, 0.0,  1.0, 0.0, 0.0,
         0.5, -0.5, 0.0,  0.0, 1.0, 0.0,
         0.0,  0.5, 0.0,  0.0, 0.0, 1.0
    };

    shader = ltglShaderProgram(vShader, fShader);
    triangle = ltglVertexData(2, 3, 0, vertices, NULL);

    return LOTUS_TRUE;
}

b8 appUpdate (f32 deltaTime, struct LTapplication* app) {
    if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) return LOTUS_FALSE;
    if (ltIsKeyDown(LOTUS_KEY_R)) ltSetClearColor(1.0, 0.0, 0.0, 1.0);
    if (ltIsKeyDown(LOTUS_KEY_G)) ltSetClearColor(0.0, 1.0, 0.0, 1.0);
    if (ltIsKeyDown(LOTUS_KEY_B)) ltSetClearColor(0.0, 0.0, 1.0, 1.0);
    
    ltInputUpdate(deltaTime);

    return LOTUS_TRUE;
}

b8 appRender (f32 deltaTime, struct LTapplication* app) {
    ltClearColor();
    
    ltSetShader(&shader);
    ltDraw(&triangle);
    
    ltSwapBuffers();

    return LOTUS_TRUE;
}

b8 ltConfigureApp(LTapplication* app) {
    app->config.winpos[0] = 100;
    app->config.winpos[1] = 100;
    app->config.winsize[0] = 1280;
    app->config.winsize[1] = 720;
    app->config.title = "Lotus Application";
    app->appInit = appInit;
    app->appRender = appRender;
    app->appUpdate = appUpdate;
    app->state = NULL;
    return LOTUS_TRUE;
}
