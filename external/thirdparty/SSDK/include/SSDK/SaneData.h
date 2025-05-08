#ifndef __SANEDATA_H__
#define __SANEDATA_H__

#include <include/SSDK/globals.h>

/* ---------------- ARRAY ---------------- */
typedef struct ArrayHeader {
    u32 count, max;
    u32 size, stride;
} ArrayHeader;

typedef enum ArrayHeaderField {
    ARRAY_SIZE_FIELD = 0,
    ARRAY_STRIDE_FIELD,
    ARRAY_COUNT_FIELD,
    ARRAY_MAX_FIELD,
    ARRAY_FIELDS,
} ArrayHeaderField;
/* ---------------- ARRAY ---------------- */


/* ---------------- LINKED ARRAY ---------------- */
typedef struct LinkedArray {
    struct LinkedArray* last;
    struct LinkedArray* next;
    ptr array;
    ArrayHeader meta;
} LinkedArray;
/* ---------------- LINKED ARRAY ---------------- */


/* ---------------- HASH ARRAY ---------------- */
typedef struct KeyValue {
    ptr value;
    cstr key;
} KeyValue;

typedef struct HashArray {
    KeyValue* map;
    ArrayHeader meta;
} HashArray;
/* ---------------- HASH ARRAY ---------------- */

typedef struct SaneData {
    SaneModule module;

    struct {
        SSDK_FNPTR(ptr, create, u32 stride, u32 max);
        SSDK_FNPTR(ptr, resize, ptr array, u32 max);
        SSDK_FNPTR(void, destroy, ptr array);
        
        SSDK_FNPTR(void, push, ptr array, ptr invalue);
        SSDK_FNPTR(void, pop, ptr array, ptr outvalue);
        SSDK_FNPTR(void, put, ptr array, u32 index, ptr invalue);
        SSDK_FNPTR(void, pull, ptr array, u32 index, ptr outvalue);
        
        SSDK_FNPTR(ArrayHeader, getHeader, ptr array);
    } array;

    struct {
        SSDK_FNPTR(none, create, u32 stride, u32 max, LinkedArray* array);
        SSDK_FNPTR(none, destroy, LinkedArray* array);
        SSDK_FNPTR(none, collapse, LinkedArray* array);
    } linkedArray;

    struct {
        SSDK_FNPTR(none, create, u32 max, HashArray* array);
        SSDK_FNPTR(none, destroy, HashArray* array);
        
        SSDK_FNPTR(byte, put, cstr key, ptr value, HashArray* array);
        SSDK_FNPTR(byte, pull, cstr key, KeyValue* out, HashArray* array);
        
        SSDK_FNPTR(cstr*, getKeys, HashArray* array);
        SSDK_FNPTR(ptr*, getValues, HashArray* array);
        SSDK_FNPTR(ptr, get, cstr key, HashArray* array);
    } hashArray;

} SaneData;
extern SaneData* saneData;

SSDK_FUNC byte ssdkInitDS(none);
SSDK_FUNC none ssdkExitDS(none);

#endif  // __SANEDATA_H__