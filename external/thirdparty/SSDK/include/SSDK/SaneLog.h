#ifndef __SANELOG_H__
#define __SANELOG_H__

#include <include/SSDK/globals.h>

typedef enum LogLevel {
    SANE_LOG_BASE,
    SANE_LOG_INFO,
    SANE_LOG_WARN,
    SANE_LOG_SUCCESS,
    SANE_LOG_ERROR,
    SANE_LOG_LEVELS
} LogLevel;

typedef struct SaneLog {
    SaneModule module;
    SSDK_FNPTR(none, log, u8 level, cstr msg);
    SSDK_FNPTR(none, logFmt, u8 level, cstr msg, ...);
} SaneLog;
extern SaneLog* saneLog;

SSDK_FUNC byte ssdkInitLog(none);
SSDK_FUNC none ssdkExitLog(none);

#endif  // __SANELOG_H__