#ifndef __R3_ALLOC_H__
#define __R3_ALLOC_H__

#include <r3kit/include/defines.h>

R3_API void r3_mem_dealloc(ptr mem);
R3_API ptr r3_mem_alloc(u64 size, u8 align);
R3_API ptr r3_mem_realloc(u64 size, u8 align, ptr mem);

R3_API u8 r3_mem_assign(ptr value, ptr mem);
R3_API u8 r3_mem_set(u64 size, u8 value, ptr mem);
R3_API u8 r3_mem_read(u64 size, ptr value, ptr mem);
R3_API u8 r3_mem_write(u64 size, ptr value, ptr mem);

#endif	// __R3_ALLOC_H__
