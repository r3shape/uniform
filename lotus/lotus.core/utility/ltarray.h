#ifndef LTARRAY_H
#define LTARRAY_H

#include "../defines.h"

/* D-Array Memory Layout
*   max         (u64)
*   length      (u64)
*   stride      (u64)
*   elements    (void*)
*/

#define LOTUS_ARRAY_MAX_DEFAULT 1
#define LOTUS_ARRAY_RESIZE_DEFAULT 2

typedef enum LTarrayFields {
    LOTUS_ARRFIELD_MAX,
    LOTUS_ARRFIELD_LENGTH,
    LOTUS_ARRFIELD_STRIDE,
    LOTUS_ARRAY_FIELDS
} LTarrayFields;

LOTUS_API_ENTRY void ltDestroyDArray(void* arr);
LOTUS_API_ENTRY void* ltMakeDArray(u64 max, u64 stride);

LOTUS_API_ENTRY u64 ltGetDArrayField(u64 field, void* arr);
LOTUS_API_ENTRY void ltSetDArrayField(u64 field, u64 value, void* arr);

LOTUS_API_ENTRY void* ltResizeDArray(u64 max, void* arr);
LOTUS_API_ENTRY int ltPopDArray(void* element, void* arr);
LOTUS_API_ENTRY int ltPushDArray(void* element, void* arr);
LOTUS_API_ENTRY int ltPopAtDArray(u64 index, void* element, void* arr);

#endif