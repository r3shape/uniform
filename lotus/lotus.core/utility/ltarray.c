#include "ltarray.h"
#include "../memory/ltmemory.h"
#include "../platform/ltlogger.h"


void* ltMakeDArray(u64 max, u64 stride) {

    u64 total_size = (LOTUS_ARRAY_FIELDS * sizeof(u64)) + (max * stride);
    void* arr = ltMemAlloc(total_size, LOTUS_MEMTAG_ARRAY);

    u64* header = (u64*)arr;
    header[LOTUS_ARRFIELD_MAX] = max;
    header[LOTUS_ARRFIELD_LENGTH] = 0;
    header[LOTUS_ARRFIELD_STRIDE] = stride;

    return (void*)(header + LOTUS_ARRAY_FIELDS);
}


void* ltResizeDArray(u64 max, void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    u64 stride = header[LOTUS_ARRFIELD_STRIDE];
    u64 total_size = (LOTUS_ARRAY_FIELDS * sizeof(u64)) + (max * stride);


    void* temp = realloc(header, total_size);
    if (!temp) {
        return NULL;
    }

    header = (u64*)temp;
    header[LOTUS_ARRFIELD_MAX] = max;
    if (header[LOTUS_ARRFIELD_LENGTH] > max) {
        header[LOTUS_ARRFIELD_LENGTH] = max;
    }

    return (void*)(header + LOTUS_ARRAY_FIELDS);
}


void ltDestroyDArray(void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    u64 max = header[LOTUS_ARRFIELD_MAX];
    u64 stride = header[LOTUS_ARRFIELD_STRIDE];
    ltMemFree(header, (LOTUS_ARRAY_FIELDS * sizeof(u64)) + (max * stride), LOTUS_MEMTAG_ARRAY);
}


u64 ltGetDArrayField(u64 field, void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    return header[field];
}


void ltSetDArrayField(u64 field, u64 value, void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    header[field] = value;
}


int ltPushDArray(void* element, void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    u64 length = header[LOTUS_ARRFIELD_LENGTH];
    u64 max = header[LOTUS_ARRFIELD_MAX];
    u64 stride = header[LOTUS_ARRFIELD_STRIDE];

    if (length >= max) {
        return -1;
    }

    void* dest = (char*)arr + (length * stride);
    for (u64 i = 0; i < stride; i++) {
        ((char*)dest)[i] = ((char*)element)[i];
    }

    header[LOTUS_ARRFIELD_LENGTH] += 1;
    return 0;
}


int ltPopDArray(void* element, void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    u64 length = header[LOTUS_ARRFIELD_LENGTH];
    u64 stride = header[LOTUS_ARRFIELD_STRIDE];

    if (length == 0) {
        return -1;
    }


    void* src = (char*)arr + ((length - 1) * stride);
    for (u64 i = 0; i < stride; i++) {
        ((char*)element)[i] = ((char*)src)[i];
    }

    header[LOTUS_ARRFIELD_LENGTH] -= 1;
    return 0;
}

int ltPopAtDArray(u64 index, void* element, void* arr) {
    u64* header = (u64*)arr - LOTUS_ARRAY_FIELDS;
    u64 length = header[LOTUS_ARRFIELD_LENGTH];
    u64 stride = header[LOTUS_ARRFIELD_STRIDE];

    if (length == 0 || index >= length) {
        return -1;
    }

    void* src = (char*)arr + (index * stride);

    for (u64 i = 0; i < stride; i++) {
        ((char*)element)[i] = ((char*)src)[i];
    }

    // shift elements
    if (index < length - 1) {
        memmove(src, (char*)src + stride, (length - index - 1) * stride);
    }

    header[LOTUS_ARRFIELD_LENGTH] -= 1;
    return 0;
}


