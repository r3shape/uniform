#include "../include/lotus2D.h"
#include "../lotus.core/utility/ltmath.h"
#include "../lotus.core/platform/ltlogger.h"

b8 lt2DInit(void) {
    if (!lotusInit()) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to initialize Lotus Core!\n");
        return LOTUS_FALSE;
    }
    
    if (!ltECSInit()) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to initialize Lotus ECS!\n");
        lotusExit();
        return LOTUS_FALSE;
    }

    LTrenderState* renderState = ltGetRenderState();
    LTplatformState* platformState = ltGetPlatformState();
    renderState->mProj = ltOrtho(0, platformState->window.size.x, 0, platformState->window.size.y, 0.1, 10.0);

    scene2D = ltMakeScene("Lotus Scene 2D");

    return LOTUS_TRUE;
}

void lt2DExit(void) {
    ltDestroyScene(scene2D);
    ltECSExit();
    lotusExit();
}

LTscene* ltGetScene2D(void) { return (scene2D != (void*)0) ? scene2D : (void*)0; }
