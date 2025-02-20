#include "../include/utility/r3_array.h"

#include <stdio.h>

void r3_print_array(char* tag, void* array) {
    if (!array) return;

    ubyte4* header = R3_ARRAY_GET_HEADER(array);
    ubyte4 size = header[0];
    ubyte4 stride = header[1];
    ubyte4 length = header[2];
    ubyte4 capacity = header[3];

    printf("|(Array): %s\n", (tag) ? tag : "Lotus Array");
    printf("|(size): %d bytes (stride): %d bytes (length): %d elems (capacity): %d elems\n", size, stride, length, capacity);
}

void* r3_create_array(ubyte4 stride, ubyte4 capacity) {
    void* array = r3_memory_api->alloc(R3_ARRAY_HEADER_SIZE + (capacity * stride), 16);
    if (!array) return NULL;    // error: out of memory!

    ubyte4* header = (ubyte4*)((ubyte*)array);
    header[0] = R3_ARRAY_HEADER_SIZE + (capacity * stride);
    header[1] = stride;
    header[2] = 0;
    header[3] = capacity;

    void* content = (void*)((ubyte*)array + R3_ARRAY_HEADER_SIZE);
    return content;
}

void r3_destroy_array(void* array) {
    if (!array) return;
    ubyte4* header = R3_ARRAY_GET_HEADER(array);
    r3_memory_api->free(header);
}

void* r3_resize_array(void* array, ubyte4 new_capacity) {
    if (!array) return NULL;

    ubyte4* header = R3_ARRAY_GET_HEADER(array);
    ubyte4 stride = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_STRIDE_FIELD);

    ubyte4 total_size = R3_ARRAY_HEADER_SIZE + (new_capacity * stride);

    void* temp = r3_memory_api->realloc(header, total_size, 16);
    if (!temp) return NULL;     // error: failed to re-allocate array!

    header = (ubyte4*)((ubyte*)temp);
    void* content = (void*)((ubyte*)temp + R3_ARRAY_HEADER_SIZE);

    R3_ARRAY_SET_HEADER_FIELD(content, R3_ARRAY_SIZE_FIELD, R3_ARRAY_HEADER_SIZE + (new_capacity * stride));
    R3_ARRAY_SET_HEADER_FIELD(content, R3_ARRAY_LENGTH_FIELD, (header[2] > new_capacity) ? new_capacity : header[2]);
    R3_ARRAY_SET_HEADER_FIELD(content, R3_ARRAY_CAPACITY_FIELD, new_capacity);

    return content;
}

void r3_pop_array(void* array, void* out_value) {
    if (!array || !out_value) return;

    ubyte4* header = R3_ARRAY_GET_HEADER(array);
    ubyte4 stride = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_STRIDE_FIELD);
    ubyte4 length = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_LENGTH_FIELD);

    if (length == 0) return;    // error: array empty!

    void* slot = (ubyte*)array + ((length - 1) * stride);
    memcpy(out_value, slot, stride);
    header[R3_ARRAY_LENGTH_FIELD]--;
}

void* r3_push_array(void* array, void* in_value) {
    if (!array || !in_value) return NULL;   // error: null ptr!

    ubyte4* header = R3_ARRAY_GET_HEADER(array);
    ubyte4 stride = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_STRIDE_FIELD);
    ubyte4 length = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_LENGTH_FIELD);
    ubyte4 capacity = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_CAPACITY_FIELD);

    if (length >= capacity) {
        array = r3_resize_array(array, capacity * 2);
        header = (ubyte4*)((ubyte*)array - R3_ARRAY_HEADER_SIZE);
    }

    void* slot = (ubyte*)array + (length * stride);
    memcpy(slot, in_value, stride);
    header[R3_ARRAY_LENGTH_FIELD]++;

    return array;
}

void* r3_insert_array(void* array, ubyte4 index, void* in_value) {
    if (!array || !in_value) return NULL;

    ubyte4* header = R3_ARRAY_GET_HEADER(array);
    ubyte4 stride = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_STRIDE_FIELD);
    ubyte4 length = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_LENGTH_FIELD);
    ubyte4 capacity = R3_ARRAY_GET_HEADER_FIELD(array, R3_ARRAY_CAPACITY_FIELD);

    if (index > length) return NULL;    // error: index out of bounds!

    if (length >= capacity) {
        array = r3_resize_array(array, capacity * 2);
        header = (ubyte4*)((ubyte*)array - R3_ARRAY_HEADER_SIZE);
    }

    void* slot = (ubyte*)array + (index * stride);
    if (index < length) memmove((ubyte*)slot + stride, slot, (length - index) * stride);

    memcpy(slot, in_value, stride);
    header[R3_ARRAY_LENGTH_FIELD]++;

    return array;
}
