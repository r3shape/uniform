#ifndef LTLOGGER_H
#define LTLOGGER_H

#include "../defines.h"
#include "lotus_platform.h"

typedef enum Lotus_Log_Level {
    LOTUS_LOG_FATAL,
    LOTUS_LOG_ERROR,
    LOTUS_LOG_WARNING,
    LOTUS_LOG_INFO,
    LOTUS_LOG_DEBUG,
    LOTUS_LOG_TRACE
} Lotus_Log_Level;

static Lotus_Log_Level currentLogLevel = LOTUS_LOG_INFO;

// Log level colors: fatal, error, warning, info, debug, trace
static const ubyte logLevelColors[] = {0, 1, 2, 3, 4, 5};

static Lotus_Log_Level lotus_get_log_level() { return currentLogLevel; }
static void lotus_set_log_level(Lotus_Log_Level level) { currentLogLevel = level; }

static void lotus_log_message(Lotus_Log_Level level, const char *message, ...) {
    if (level > currentLogLevel) return;

    va_list args;
    va_start(args, message);

    char buffer[1024];
    if (vsnprintf(buffer, sizeof(buffer), message, args) >= sizeof(buffer)) {
        printf("Log message truncated!\n");
    }
    va_end(args);

    printf("%s\n", buffer);
}

#define LOTUS_LOG_IF_ENABLED(LEVEL, MESSAGE, ...) \
    lotus_log_message(LEVEL, "|" #LEVEL "| :: " MESSAGE, ##__VA_ARGS__)

#define lotus_log_fatal(MESSAGE, ...) \
    LOTUS_LOG_IF_ENABLED(LOTUS_LOG_FATAL, MESSAGE, ##__VA_ARGS__)

#define lotus_log_error(MESSAGE, ...) \
    LOTUS_LOG_IF_ENABLED(LOTUS_LOG_ERROR, MESSAGE, ##__VA_ARGS__)

#define lotus_log_warning(MESSAGE, ...) \
    LOTUS_LOG_IF_ENABLED(LOTUS_LOG_WARNING, MESSAGE, ##__VA_ARGS__)

#define lotus_log_info(MESSAGE, ...) \
    LOTUS_LOG_IF_ENABLED(LOTUS_LOG_INFO, MESSAGE, ##__VA_ARGS__)

#define lotus_log_debug(MESSAGE, ...) \
    LOTUS_LOG_IF_ENABLED(LOTUS_LOG_DEBUG, MESSAGE, ##__VA_ARGS__)

#define lotus_log_trace(MESSAGE, ...) \
    LOTUS_LOG_IF_ENABLED(LOTUS_LOG_TRACE, MESSAGE, ##__VA_ARGS__)

#endif