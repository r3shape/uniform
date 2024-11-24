#ifndef LTPLATFORM_H
#define LTPLATFORM_H

#include "../defines.h"

typedef struct tagPlatformState {
    void* internal;
    void* windowPtr;
} LTplatformState;

f64 ltPlatformGetTime(void);
void ltPlatformSleep(u64 ms);
b8 ltPlatformPump(LTplatformState* state);
void ltPlatformExit(LTplatformState* state);
void ltPlatformSwapBuffers(LTplatformState* state);
b8 ltPlatformInit(LTplatformState* state, const char* application, i32 x, i32 y, i32 w, i32 H);

void* ltMemAlloc(u64 size, b8 aligned);
void* ltMemZero(void* block, u64 size);
void ltMemFree(void* block, b8 aligned);
void* ltMemSet(void* block, i32 value, u64 size);
void* ltMemCopy(void* dest, const void* source, u64 size);

void ltConsoleWrite(const char* message, u8 color);
void ltConsoleWriteError(const char* message, u8 color);

#endif