#pragma once

#include "../r3_core_defines.h"
#include "../memory/r3_memory.h"

typedef enum R3_Array_Field {
    R3_ARRAY_SIZE_FIELD = 0,
    R3_ARRAY_STRIDE_FIELD,
    R3_ARRAY_LENGTH_FIELD,
    R3_ARRAY_CAPACITY_FIELD,
    R3_ARRAY_FIELD_COUNT
} R3_Array_Field;

#define R3_ARRAY_HEADER_SIZE (4 * sizeof(ubyte4))
#define R3_ARRAY_GET_HEADER(array) ((ubyte4*)((ubyte*)array - R3_ARRAY_HEADER_SIZE))
#define R3_ARRAY_GET_HEADER_FIELD(array, field) R3_ARRAY_GET_HEADER(array)[field]
#define R3_ARRAY_SET_HEADER_FIELD(array, field, value) R3_ARRAY_GET_HEADER(array)[field] = value

R3_API void* r3_create_array(ubyte4 stride, ubyte4 capacity);
R3_API void r3_destroy_array(void* array);

R3_API void r3_print_array(char* tag, void* array);

R3_API void r3_pop_array(void* array, void* out_value);
R3_API void* r3_push_array(void* array, void* in_value);
R3_API void* r3_resize_array(void* array, ubyte4 new_capacity);
R3_API void* r3_insert_array(void* array, ubyte4 index, void* in_value);
