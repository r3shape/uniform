#include "../../include/lotus2D.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 uLocation;\n"
    "layout(location = 1) in vec3 uColor;\n"
    "uniform mat4 uModel;\n"
    "uniform mat4 uView;\n"
    "uniform mat4 uProj;\n"
    "out vec3 vertexColor;\n"
    "void main() {\n"
    "   gl_Position = uProj * uView * uModel * vec4(uLocation, 1.0f);\n"
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

int main () {
    b8 running = lt2DInit();
    LTscene* scene = ltGetScene2D();

    LTshaderProgram shader = ltglShaderProgram(vShader, fShader);
    
    LTentity e0 = scene->entityManager.makeEntity();
    
    scene->componentManager.addComponent(e0, LOTUS_MESH_COMPONENT);
    scene->componentManager.addComponent(e0, LOTUS_TRANSFORM_COMPONENT);

    ltRectangleMesh2D(e0, 32, 32, 1, 0, 0);

    // handle engine-events via callback
    ltRegisterEvent(LOTUS_EVENT_RESIZE, 0, resizeCallback);

    // camera-data handled manually until implementation :)
    LTvec3 eye = {0.0f, 0.0f, 1.0f};
    LTvec3 center = {0.0f, 0.0f, 0.0f};
    LTvec3 up = {0.0f, 1.0f, 0.0f};
    LTmat4 mView = ltLookAt(eye, center, up);

    f32 mspeed = 5;
    LTtransformComponent transform;
    LTcomponent comp = scene->componentManager.getComponent(e0, LOTUS_TRANSFORM_COMPONENT);
    if (comp.type == LOTUS_TRANSFORM_COMPONENT) {
        transform = comp.data.transform;
    }

    while (running) {
        ltClearColor();
        ltPumpEvents();

        // handle input-events via-state
        if (ltIsKeyDown(LOTUS_KEY_ESCAPE)) running = 0;

        if (ltIsKeyDown(LOTUS_KEY_D)) transform.location->x += mspeed;
        if (ltIsKeyDown(LOTUS_KEY_A)) transform.location->x -= mspeed;
        if (ltIsKeyDown(LOTUS_KEY_W)) transform.location->y += mspeed;
        if (ltIsKeyDown(LOTUS_KEY_S)) transform.location->y -= mspeed;

        // handle transforms manually until implementation :)
        *transform.model = ltMulMat4(ltIdentity(), ltTransMat4(transform.location->x, transform.location->y, 0.0));

        // this uniform will be set with ltDrawMesh2D() once camera-data is implemented :)
        ltglSetUniform(shader, LOTUS_UNIFORM_MAT4, "uView", &mView);

        ltSetShader(&shader);
        ltDrawMesh2D(e0);

        ltInputUpdate(0);
        ltSwapBuffers();
    }; lt2DExit();
    return 0;
}

