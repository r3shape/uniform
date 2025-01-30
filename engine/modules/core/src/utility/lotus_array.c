#include "../include/utility/lotus_array.h"

#include <stdio.h>

void lotus_print_array(ubyte* tag, void* array) {
    if (!array) return;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 size = header[0];
    ubyte4 stride = header[1];
    ubyte4 length = header[2];
    ubyte4 capacity = header[3];

    printf("|(Array): %s\n", (tag) ? tag : "Lotus Array");
    printf("|(size): %d bytes (stride): %d bytes (length): %d elems (capacity): %d elems\n", size, stride, length, capacity);
}

void* lotus_create_array(ubyte4 stride, ubyte4 capacity) {
    void* array = lotus_memory_api->alloc(LOTUS_ARRAY_HEADER_SIZE + (capacity * stride), 16);
    if (!array) return NULL;    // error: out of memory!

    ubyte4* header = (ubyte4*)((ubyte*)array);
    header[0] = LOTUS_ARRAY_HEADER_SIZE + (capacity * stride);
    header[1] = stride;
    header[2] = 0;
    header[3] = capacity;

    void* content = (void*)((ubyte*)array + LOTUS_ARRAY_HEADER_SIZE);
    return content;
}

void lotus_destroy_array(void* array) {
    if (!array) return;
    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    lotus_memory_api->free(header);
}

void* lotus_resize_array(void* array, ubyte4 new_capacity) {
    if (!array) return NULL;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 stride = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_STRIDE_FIELD);

    ubyte4 total_size = LOTUS_ARRAY_HEADER_SIZE + (new_capacity * stride);

    void* temp = lotus_memory_api->realloc(header, total_size, 16);
    if (!temp) return NULL;     // error: failed to re-allocate array!

    header = (ubyte4*)((ubyte*)temp);
    void* content = (void*)((ubyte*)temp + LOTUS_ARRAY_HEADER_SIZE);

    LOTUS_ARRAY_SET_HEADER_FIELD(content, LOTUS_ARRAY_SIZE_FIELD, LOTUS_ARRAY_HEADER_SIZE + (new_capacity * stride));
    LOTUS_ARRAY_SET_HEADER_FIELD(content, LOTUS_ARRAY_LENGTH_FIELD, (header[2] > new_capacity) ? new_capacity : header[2]);
    LOTUS_ARRAY_SET_HEADER_FIELD(content, LOTUS_ARRAY_CAPACITY_FIELD, new_capacity);

    return content;
}

void lotus_pop_array(void* array, void* out_value) {
    if (!array || !out_value) return;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 stride = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_STRIDE_FIELD);
    ubyte4 length = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_LENGTH_FIELD);

    if (length == 0) return;    // error: array empty!

    void* slot = (ubyte*)array + ((length - 1) * stride);
    memcpy(out_value, slot, stride);
    header[LOTUS_ARRAY_LENGTH_FIELD]--;
}

void* lotus_push_array(void* array, void* in_value) {
    if (!array || !in_value) return NULL;   // error: null ptr!

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 stride = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_STRIDE_FIELD);
    ubyte4 length = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_LENGTH_FIELD);
    ubyte4 capacity = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_CAPACITY_FIELD);

    if (length >= capacity) {
        array = lotus_resize_array(array, capacity * 2);
        header = (ubyte4*)((ubyte*)array - LOTUS_ARRAY_HEADER_SIZE);
    }

    void* slot = (ubyte*)array + (length * stride);
    memcpy(slot, in_value, stride);
    header[LOTUS_ARRAY_LENGTH_FIELD]++;

    return array;
}

void* lotus_insert_array(void* array, ubyte4 index, void* in_value) {
    if (!array || !in_value) return NULL;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 stride = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_STRIDE_FIELD);
    ubyte4 length = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_LENGTH_FIELD);
    ubyte4 capacity = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_CAPACITY_FIELD);

    if (index > length) return NULL;    // error: index out of bounds!

    if (length >= capacity) {
        array = lotus_resize_array(array, capacity * 2);
        header = (ubyte4*)((ubyte*)array - LOTUS_ARRAY_HEADER_SIZE);
    }

    void* slot = (ubyte*)array + (index * stride);
    if (index < length) memmove((ubyte*)slot + stride, slot, (length - index) * stride);

    memcpy(slot, in_value, stride);
    header[LOTUS_ARRAY_LENGTH_FIELD]++;

    return array;
}
