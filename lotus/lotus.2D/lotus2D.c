#include "../include/lotus2D.h"

#include "../lotus.core/utility/ltmath.h"
#include "../lotus.core/platform/ltlogger.h"

b8 lt2dInit(void) {
    b8 ltcInit = lotusInit();
    if (!ltcInit) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to initialize Lotus Core!\n");
        return LOTUS_FALSE;
    }

    LTrenderState* renderState = ltGetRenderState();
    LTplatformState* platformState = ltGetPlatformState();
    renderState->mProj = ltOrtho(0, platformState->window.size.x, 0, platformState->window.size.y, 0.1, 10.0);

    return LOTUS_TRUE;
}

void lt2dExit(void) {
    lotusExit();
}

