#pragma once

#include "ltapplication.h"
#include "../lotus.core/platform/ltlogger.h"

int main(void) {
    LTapplication app;
    if (!ltConfigureApp(&app)) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to configure application instance!\n");
        return 1;
    }
    
    if (!app.appInit || !app.appUpdate || !app.appRender) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Application function pointers missing!\n");
        return 1;
    }

    if (!ltInitApplication(&app)) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to make application!\n");
        return 1;
    }

    if (!ltRunApplication()) {
        ltSetLogLevel(LOTUS_LOG_FATAL);
        ltLogFatal("Failed to shutdown gracefully!\n");
        return 1;
    }

    return 0;
}
