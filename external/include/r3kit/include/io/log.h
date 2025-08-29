#ifndef __R3_LOG_H__
#define __R3_LOG_H__

#include <r3kit/include/defines.h>

typedef enum Log_Level {
	INFO_LOG,
	WARN_LOG,
	TRACE_LOG,
	ERROR_LOG,
	SUCCESS_LOG,
	LOG_LEVELS
} Log_Level;

R3_API none r3_log_toggle(Log_Level level);
R3_API none r3_log_stdout(Log_Level level, char* message);
R3_API none r3_log_stdoutf(Log_Level level, char* message, ...);

#endif	// __R3_LOG_H__