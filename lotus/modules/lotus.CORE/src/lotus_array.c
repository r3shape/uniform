#include "../include/utility/lotus_array.h"

void lotus_print_array(char* tag, void* array) {
    if (!array) return;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 size = header[0];
    ubyte4 stride = header[1];
    ubyte4 length = header[2];
    ubyte4 capacity = header[3];

    printf("|(Array): %s\n", (tag) ? tag : "Lotus Array");
    printf("|(size): %d bytes (stride): %d bytes (length): %d elems (capacity): %d elems\n", size, stride, length, capacity);
}

void* lotus_make_array(ubyte4 stride, ubyte4 capacity) {
    void* array = malloc(LOTUS_ARRAY_HEADER_SIZE + (capacity * stride));
    if (!array) {
        printf("Failed to allocate array!\n");
        return NULL;
    }

    ubyte4* header = (ubyte4*)((char*)array);
    header[0] = LOTUS_ARRAY_HEADER_SIZE + (capacity * stride);
    header[1] = stride;
    header[2] = 0;
    header[3] = capacity;

    void* content = (void*)((char*)array + LOTUS_ARRAY_HEADER_SIZE);
    return content;
}

void lotus_destroy_array(void* array) {
    if (!array) return;
    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    free(header);
}

void* lotus_resize_array(void* array, ubyte4 new_capacity) {
    if (!array) return NULL;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 stride = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_STRIDE_FIELD);

    ubyte4 total_size = LOTUS_ARRAY_HEADER_SIZE + (new_capacity * stride);

    void* temp = realloc(header, total_size);
    if (!temp) {
        printf("Failed to re-allocate array!\n");
        return NULL;
    }

    header = (ubyte4*)((char*)temp);
    void* content = (void*)((char*)temp + LOTUS_ARRAY_HEADER_SIZE);

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

    if (length == 0) {
        printf("Array is empty!\n");
        return;
    }

    void* slot = (char*)array + ((length - 1) * stride);
    memcpy(out_value, slot, stride);
    header[LOTUS_ARRAY_LENGTH_FIELD]--;
}

void* lotus_push_array(void* array, void* in_value) {
    if (!array || !in_value) return NULL;

    ubyte4* header = LOTUS_ARRAY_GET_HEADER(array);
    ubyte4 stride = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_STRIDE_FIELD);
    ubyte4 length = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_LENGTH_FIELD);
    ubyte4 capacity = LOTUS_ARRAY_GET_HEADER_FIELD(array, LOTUS_ARRAY_CAPACITY_FIELD);

    if (length >= capacity) {
        array = lotus_resize_array(array, capacity * 2);
        header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    }

    void* slot = (char*)array + (length * stride);
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

    if (index > length) {
        printf("Index out of bounds!\n");
        return NULL;
    }

    if (length >= capacity) {
        printf("resizing on index: %d\n", index);
        array = lotus_resize_array(array, capacity * 2);
        header = (ubyte4*)((char*)array - LOTUS_ARRAY_HEADER_SIZE);
    }

    void* slot = (char*)array + (index * stride);
    if (index < length) memmove((char*)slot + stride, slot, (length - index) * stride);

    memcpy(slot, in_value, stride);
    header[LOTUS_ARRAY_LENGTH_FIELD]++;

    return array;
}
