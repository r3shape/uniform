#ifndef LTPLATFORM_H
#define LTPLATFORM_H

#include "../defines.h"
#include "../lotustypes.h"

LTdynamicLib ltPlatformLoadLib(char* name);
b8 ltPlatformUnloadLib(LTdynamicLib* lib);

f64 ltPlatformGetTime(void);
void ltPlatformSleep(u64 ms);
b8 ltPlatformPump(LTplatformState* state);
void ltPlatformExit(LTplatformState* state);
void ltPlatformSwapBuffers(LTplatformState* state);
b8 ltPlatformInit(LTplatformState* state, char* application, i32 x, i32 y, i32 w, i32 H);

void* _ltMemAlloc(u64 size, b8 aligned);
void* _ltMemZero(void* block, u64 size);
void _ltMemFree(void* block, b8 aligned);
void* _ltMemSet(void* block, i32 value, u64 size);
void* _ltMemCopy(void* dest, const void* source, u64 size);

void ltConsoleWrite(const char* message, u8 color);
void ltConsoleWriteError(const char* message, u8 color);

#endif