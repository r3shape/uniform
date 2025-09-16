#ifndef __R3_ARRAY_H__
#define __R3_ARRAY_H__

#include <r3kit/include/defines.h>

typedef struct ArrayHeader {
	u64 stride;
	u64 count;
	u64 size;
	u64 max;
} ArrayHeader;

// bare 'wrapper' structure for type saftey
typedef struct Array {
	ptr data;
} Array;

// TODO: r3_arr_put: at index, shift right 1 and write a value

R3_API u64 r3_arr_max(Array* in);
R3_API u64 r3_arr_count(Array* in);
R3_API u64 r3_arr_stride(Array* in);
R3_API ArrayHeader r3_arr_header(Array* in);

R3_API u8 r3_arr_dealloc(Array* in);
R3_API u8 r3_arr_realloc(u64 max, Array* in);
R3_API u8 r3_arr_alloc(u64 max, u64 stride, Array* out);

R3_API u8 r3_arr_shift_left(u64 index, u64 shift, Array *in);
R3_API u8 r3_arr_shift_right(u64 index, u64 shift, Array *in);

R3_API u8 r3_arr_pop(ptr value, Array* in);
R3_API u8 r3_arr_push(ptr value, Array* in);
R3_API u8 r3_arr_pull(u64 index, ptr value, Array* in);

R3_API u8 r3_arr_read(u64 index, ptr value, Array* in);
R3_API u8 r3_arr_write(u64 index, ptr value, Array* in);
R3_API u8 r3_arr_assign(u64 index, ptr value, Array* in);

/* HASH ARRAY */
R3_API u8 r3_arr_hashed_dealloc(Array* in);
R3_API u8 r3_arr_hashed_alloc(u64 max, u64 stride, Array* out);

R3_API u8 r3_arr_hashed_pull(const char* key, ptr value, Array* in);

R3_API u8 r3_arr_hashed_read(const char* key, ptr value, Array* in);
R3_API u8 r3_arr_hashed_write(const char* key, ptr value, Array* in);
R3_API u8 r3_arr_hashed_assign(const char* key, ptr value, Array* in);

#endif // __R3_ARRAY_H__
