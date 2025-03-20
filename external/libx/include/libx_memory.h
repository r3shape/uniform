#pragma once

#include "libx_def.h"

/* ---------------- LINEAR ALLOCATOR ---------------- */
typedef struct Linear_Allocator {
    void* data;
    u64 offset;
    u64 max;
} Linear_Allocator;
/* ---------------- LINEAR ALLOCATOR ---------------- */


/* ---------------- ARENA ALLOCATOR ---------------- */
typedef struct Arena_Allocator {
    struct Arena_Allocator* last;
    struct Arena_Allocator* next;
    void* data;
    u64 offset;
    u64 max;
} Arena_Allocator;
/* ---------------- ARENA ALLOCATOR ---------------- */


/* ---------------- STACK ALLOCATOR ---------------- */
/* ---------------- STACK ALLOCATOR ---------------- */


/* ---------------- RING ALLOCATOR ---------------- */
/* ---------------- RING ALLOCATOR ---------------- */


typedef struct _libx_memory_api {
    void (*dealloc)(void* ptr);
    void* (*alloc)(u64 size, u64 align);
    void* (*realloc)(void* ptr, u64 size, u64 align);

    Linear_Allocator* (*create_linear_allocator)(u64 max, u64 align);
    void* (*linear_alloc)(Linear_Allocator* allocator, u64 size, u64 align);
    void (*linear_reset)(Linear_Allocator* allocator);
    void (*destroy_linear_allocator)(Linear_Allocator* allocator);

    Arena_Allocator* (*create_arena_allocator)(Arena_Allocator* allocator, u64 max, u64 align);
    void* (*arena_alloc)(Arena_Allocator* allocator, u64 size, u64 align);
    void (*arena_reset)(Arena_Allocator* allocator);
    void (*destroy_arena_allocator)(Arena_Allocator* allocator);
    void (*collapse_arena_allocator)(Arena_Allocator* allocator);
} _libx_memory_api;
extern _libx_memory_api* memx;

LIBX_API u8 libx_init_memory(void);
LIBX_API void libx_cleanup_memory(void);
