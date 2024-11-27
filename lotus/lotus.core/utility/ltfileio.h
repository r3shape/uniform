#ifndef LTFILEIO_H
#define LTFILEIO_H

#include "../defines.h"

LOTUS_API_ENTRY b8 ltIsFile(const char* path);
LOTUS_API_ENTRY char* ltReadFile(const char* path);
LOTUS_API_ENTRY b8 ltDeleteFile(const char* path);
LOTUS_API_ENTRY long ltGetFileSize(const char* path);
LOTUS_API_ENTRY b8 ltCopyFile(const char* dest, const char* src);
LOTUS_API_ENTRY b8 ltAppendFile(const char* data, const char* path, b8 newline);
LOTUS_API_ENTRY b8 ltWriteFile(const char* data, const char* path, b8 preserve);
LOTUS_API_ENTRY b8 ltProcessFile(void (*processLine)(const char* line), const char* path);

#endif