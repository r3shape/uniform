#pragma once

#include "../defines.h"

typedef enum Lotus_Array_Field {
    LOTUS_ARRAY_SIZE_FIELD = 0,
    LOTUS_ARRAY_STRIDE_FIELD,
    LOTUS_ARRAY_LENGTH_FIELD,
    LOTUS_ARRAY_CAPACITY_FIELD,
    LOTUS_ARRAY_FIELD_COUNT
} Lotus_Array_Field;

#define LOTUS_ARRAY_HEADER_SIZE (4 * sizeof(ubyte4))
#define LOTUS_ARRAY_GET_HEADER(array) ((ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE))
#define LOTUS_ARRAY_GET_HEADER_FIELD(array, field) LOTUS_ARRAY_GET_HEADER(array)[field]
#define LOTUS_ARRAY_SET_HEADER_FIELD(array, field, value) LOTUS_ARRAY_GET_HEADER(array)[field] = value

LOTUS_API_ENTRY void* lotus_make_array(ubyte4 stride, ubyte4 capacity);
LOTUS_API_ENTRY void lotus_destroy_array(void* array);

LOTUS_API_ENTRY void lotus_print_array(char* tag, void* array);

LOTUS_API_ENTRY void lotus_pop_array(void* array, void* out_value);
LOTUS_API_ENTRY void* lotus_push_array(void* array, void* in_value);
LOTUS_API_ENTRY void* lotus_resize_array(void* array, ubyte4 new_capacity);
LOTUS_API_ENTRY void* lotus_insert_array(void* array, ubyte4 index, void* in_value);
