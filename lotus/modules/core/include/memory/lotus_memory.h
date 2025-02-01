#ifndef LOTUS_MEMORY_H
#define LOTUS_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include "../lotus_core_defines.h"

#define MEMORY_PAD_SIZE(a) (sizeof(ubyte2) + ((a) - 1))
#define ALIGN_ADDRESS(p, a) (((p) + ((a) - 1)) & ~((a) - 1))

typedef struct Lotus_Memory_Region {
    struct Lotus_Memory_Region *next;
    struct Lotus_Memory_Region *last;
    ubyte8 capacity;
    ubyte8 stride;
    ubyte8 count;
    ubyte8* data;
} Lotus_Memory_Region;

typedef struct Lotus_Memory_API {
    void *(*alloc)(ubyte8 size, ubyte8 align);
    void *(*realloc)(void *ptr, ubyte8 size, ubyte8 align);
    void (*free)(void *ptr);

    ubyte (*init_region)(Lotus_Memory_Region *region, ubyte8 stride, ubyte8 capacity, ubyte8 align);
    ubyte (*spawn_region)(Lotus_Memory_Region *region, ubyte8 stride, ubyte8 capacity, ubyte8 align);
    Lotus_Memory_Region *(*get_region)(Lotus_Memory_Region *start, sbyte8 step);
    ubyte (*link_region)(Lotus_Memory_Region *start, Lotus_Memory_Region *region, sbyte8 step);
    ubyte (*unlink_region)(Lotus_Memory_Region *region);
    void (*free_region)(Lotus_Memory_Region *region);
    void (*free_all_regions)(Lotus_Memory_Region *start);

    ubyte (*set_region_data)(Lotus_Memory_Region *region, void* ptr);
    ubyte8 (*get_region_data)(Lotus_Memory_Region *region, ubyte8 index);
} Lotus_Memory_API;
extern Lotus_Memory_API* lotus_memory_api;

LOTUS_API ubyte lotus_init_memory(void);
LOTUS_API void lotus_shutdown_memory(void);

#endif
