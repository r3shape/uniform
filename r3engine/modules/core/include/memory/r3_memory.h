#ifndef R3_MEMORY_H
#define R3_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include "../r3_core_defines.h"

#define MEMORY_PAD_SIZE(a) (sizeof(ubyte2) + ((a) - 1))
#define ALIGN_ADDRESS(p, a) (((p) + ((a) - 1)) & ~((a) - 1))

typedef struct R3_Memory_Region {
    struct R3_Memory_Region *next;
    struct R3_Memory_Region *last;
    ubyte8 capacity;
    ubyte8 stride;
    ubyte8 count;
    ubyte8* data;
} R3_Memory_Region;

typedef struct R3_Memory_API {
    void *(*alloc)(ubyte8 size, ubyte8 align);
    void *(*realloc)(void *ptr, ubyte8 size, ubyte8 align);
    void (*free)(void *ptr);

    ubyte (*init_region)(R3_Memory_Region *region, ubyte8 stride, ubyte8 capacity, ubyte8 align);
    ubyte (*spawn_region)(R3_Memory_Region *region, ubyte8 stride, ubyte8 capacity, ubyte8 align);
    R3_Memory_Region *(*get_region)(R3_Memory_Region *start, sbyte8 step);
    ubyte (*link_region)(R3_Memory_Region *start, R3_Memory_Region *region, sbyte8 step);
    ubyte (*unlink_region)(R3_Memory_Region *region);
    void (*free_region)(R3_Memory_Region *region);
    void (*free_all_regions)(R3_Memory_Region *start);

    ubyte (*set_region_data)(R3_Memory_Region *region, void* ptr);
    ubyte8 (*get_region_data)(R3_Memory_Region *region, ubyte8 index);
} R3_Memory_API;
extern R3_Memory_API* r3_memory_api;

R3_API ubyte r3_init_memory(void);
R3_API void r3_shutdown_memory(void);

#endif
