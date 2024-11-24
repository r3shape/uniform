#ifndef LTLOGGER_H
#define LTLOGGER_H

#include "../defines.h"
#include "ltplatform.h"

typedef enum LTlogLevel {
    LOTUS_LOG_FATAL,
    LOTUS_LOG_ERROR,
    LOTUS_LOG_WARNING,
    LOTUS_LOG_INFO,
    LOTUS_LOG_DEBUG,
    LOTUS_LOG_TRACE
} LTlogLevel;

static LTlogLevel currentLogLevel = LOTUS_LOG_INFO;

// Log level colors: fatal, error, warning, info, debug, trace
static const u8 logLevelColors[] = {0, 1, 2, 3, 4, 5};

static LTlogLevel ltGetLogLevel() { return currentLogLevel; }
static void ltSetLogLevel(LTlogLevel level) { currentLogLevel = level; }

static void ltLogMessage(LTlogLevel level, const char *message, ...) {
    if (level > currentLogLevel) return;

    va_list args;
    va_start(args, message);

    char buffer[1024];
    if (vsnprintf(buffer, sizeof(buffer), message, args) >= sizeof(buffer)) {
        ltConsoleWrite("Log message truncated!", logLevelColors[LOTUS_LOG_ERROR]);
    }
    va_end(args);

    u8 color = logLevelColors[level];
    ltConsoleWrite(buffer, color);
}

#define ltLogIfEnabled(LEVEL, MESSAGE, ...) \
    ltLogMessage(LEVEL, "|" #LEVEL "| :: " MESSAGE, ##__VA_ARGS__)

#define ltLogFatal(MESSAGE, ...) \
    ltLogIfEnabled(LOTUS_LOG_FATAL, MESSAGE, ##__VA_ARGS__)

#define ltLogError(MESSAGE, ...) \
    ltLogIfEnabled(LOTUS_LOG_ERROR, MESSAGE, ##__VA_ARGS__)

#define ltLogWarning(MESSAGE, ...) \
    ltLogIfEnabled(LOTUS_LOG_WARNING, MESSAGE, ##__VA_ARGS__)

#define ltLogInfo(MESSAGE, ...) \
    ltLogIfEnabled(LOTUS_LOG_INFO, MESSAGE, ##__VA_ARGS__)

#define ltLogDebug(MESSAGE, ...) \
    ltLogIfEnabled(LOTUS_LOG_DEBUG, MESSAGE, ##__VA_ARGS__)

#define ltLogTrace(MESSAGE, ...) \
    ltLogIfEnabled(LOTUS_LOG_TRACE, MESSAGE, ##__VA_ARGS__)


#endif