#ifndef __R3_ARENA_H__
#define __R3_ARENA_H__

#include <r3kit/include/defines.h>

typedef struct Arena_Allocator {
    ptr buffer;
    u64 offset;
    u64 size;
} Arena_Allocator;

R3_API u8 r3_arena_reset(Arena_Allocator* alloc);
R3_API ptr r3_arena_alloc(u64 size, Arena_Allocator* alloc);
R3_API u8 r3_arena_dealloc(u64 size, ptr value, Arena_Allocator* alloc);

#endif // __R3_ARENA_H__