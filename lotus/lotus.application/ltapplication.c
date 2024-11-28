#include "ltapplication.h"

#include "../lotus.core/memory/ltmemory.h"
#include "../lotus.core/platform/ltlogger.h"
#include "../lotus.core/platform/ltplatform.h"
#include "../lotus.core/platform/event/ltevent.h"
#include "../lotus.core/platform/input/ltinput.h"
#include "../lotus.core/graphics/renderer/ltrenderer.h"

typedef struct tagAppState {
    b8 init;
    b8 running;
    b8 suspended;
    f64 lastTime;
    u16 winsize[2];
    LTapplication* appInstance;
    LTrenderState* renderState;
    LTplatformState* platformState;
} LTappInternal;
static LTrenderState _renderState;
static LTplatformState _platformState;
static LTappInternal applicationState;

LTapplication* ltGetApplication(void) {
    if (!applicationState.init || !applicationState.appInstance) return NULL;
    return applicationState.appInstance;
}

LTrenderState* ltGetRenderState(void) {
    if (!applicationState.init || !applicationState.renderState) return NULL;
    return applicationState.renderState;
}

LTplatformState* ltGetPlatformState(void) {
    if (!applicationState.init || !applicationState.platformState) return NULL;
    return applicationState.platformState;
}

b8 ltRunApplication(void) {
    b8 res = LOTUS_TRUE;
    while (applicationState.running) {
        if (!ltPlatformPump(applicationState.platformState)) {
            applicationState.running = LOTUS_FALSE;
            res = LOTUS_FALSE;
        }

        if (!applicationState.suspended) {
            if (!applicationState.appInstance->appUpdate(0, applicationState.appInstance)) {
                ltSetLogLevel(LOTUS_LOG_FATAL);
                ltLogFatal("Failed to run application update function! shutting down.\n");
                applicationState.running = LOTUS_FALSE;
                break;
            }
            
            if (!applicationState.appInstance->appRender(0, applicationState.appInstance)) {
                ltSetLogLevel(LOTUS_LOG_FATAL);
                ltLogFatal("Failed to run application render function! shutting down.\n");
                applicationState.running = LOTUS_FALSE;
                break;
            }
        }
    }

    applicationState.running = LOTUS_FALSE;
    ltPlatformExit(applicationState.platformState);
    return res;
}

b8 ltInitApplication(LTapplication* app) {
    if (applicationState.init) return LOTUS_FALSE;
    ltMemoryInit();

    if (
        !ltEventInit()                                                                      ||
        !ltInputInit()                                                                      ||
        !ltPlatformInit(&_platformState, app->config.title, app->config.winpos[0], app->config.winpos[1], app->config.winsize[0], app->config.winsize[1]) ||
        !ltRendererInit(&_renderState, 1280, 720)                                     // ||
        ) {
            ltSetLogLevel(LOTUS_LOG_FATAL);
            ltLogError("Failed to initialize Lotus Application.\n");
            applicationState.init = LOTUS_FALSE;
            return LOTUS_FALSE;
    } else {
        ltSetLogLevel(LOTUS_LOG_INFO);
        char* memStats = ltGetMemoryUsageStr();
        ltLogInfo("%s\n", memStats);
        _ltMemFree(memStats, LOTUS_FALSE);

        applicationState.lastTime = 0.0f;
        applicationState.init = LOTUS_TRUE;
        applicationState.running = LOTUS_TRUE;
        applicationState.suspended = LOTUS_FALSE;
        applicationState.winsize[0] = app->config.winsize[0];
        applicationState.winsize[1] = app->config.winsize[1];
        
        applicationState.appInstance = app;
        applicationState.renderState = &_renderState;
        applicationState.platformState = &_platformState;

        if (!applicationState.appInstance->appInit(applicationState.appInstance)) {
            ltSetLogLevel(LOTUS_LOG_FATAL);
            ltLogError("Failed to initialize Lotus Application.\n");
            return LOTUS_FALSE;
        }

        return LOTUS_TRUE;
    }
}

