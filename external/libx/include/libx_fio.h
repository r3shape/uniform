#pragma once

#include "libx_def.h"

typedef struct _libx_fileio_api {
    u8 (*exists)(cstr path);
    u64 (*size)(cstr path);
    u8 (*remove)(cstr path);
    str (*read)(cstr path, u64 size);
    u8 (*copy)(cstr src, cstr dest, u64 size);
    u8 (*copyb)(cstr src, cstr dest, u64 size);
    u8 (*readb)(cstr path, u64 size, void* out);
    u8 (*write)(cstr path, cstr lines, u8 preserve);
    u8 (*append)(cstr path, cstr lines, u8 newline);
    u8 (*writeb)(cstr path, u64 size, u8 preserve, void* ptr);
    u8 (*appendb)(cstr path, u64 size, u8 newline, void* ptr);
    u8 (*process)(cstr path, u64 lines, void (*callback)(cstr line));
} _libx_fileio_api;
extern _libx_fileio_api* fileio_api;

LIBX_API u8 libx_init_fileio(void);
LIBX_API void libx_cleanup_fileio(void);
