#ifndef __SANEDATA_H__
#define __SANEDATA_H__

#include <include/SSDK/globals.h>

/* ---------------- ARRAY ---------------- */
typedef struct SaneArrayHeader {
    u32 count, max;
    u32 size, stride;
} SaneArrayHeader;

typedef enum Array_Head_Field {
    ARRAY_SIZE_FIELD = 0,
    ARRAY_STRIDE_FIELD,
    ARRAY_COUNT_FIELD,
    ARRAY_MAX_FIELD,
    ARRAY_FIELDS,
} Array_Head_Field;
/* ---------------- ARRAY ---------------- */


/* ---------------- LINKED ARRAY ---------------- */
typedef struct SaneLinkedArray {
    struct SaneLinkedArray* last;
    struct SaneLinkedArray* next;
    ptr array;
    SaneArrayHeader meta;
} SaneLinkedArray;
/* ---------------- LINKED ARRAY ---------------- */


/* ---------------- HASH ARRAY ---------------- */
typedef struct SaneKeyValue {
    ptr value;
    cstr key;
} SaneKeyValue;

typedef struct SaneHashArray {
    SaneKeyValue* map;
    SaneArrayHeader meta;
} SaneHashArray;
/* ---------------- HASH ARRAY ---------------- */

typedef struct SaneDS {
    SaneModule module;

    struct {
        SSDK_FNPTR(ptr, create, u32 stride, u32 max);
        SSDK_FNPTR(ptr, resize, ptr array, u32 max);
        SSDK_FNPTR(void, destroy, ptr array);
        
        SSDK_FNPTR(void, push, ptr array, ptr invalue);
        SSDK_FNPTR(void, pop, ptr array, ptr outvalue);
        SSDK_FNPTR(void, put, ptr array, u32 index, ptr invalue);
        SSDK_FNPTR(void, pull, ptr array, u32 index, ptr outvalue);
        
        SSDK_FNPTR(SaneArrayHeader, getHeader, ptr array);
    } array;

    struct {
        SSDK_FNPTR(none, create, u32 stride, u32 max, SaneLinkedArray* array);
        SSDK_FNPTR(none, destroy, SaneLinkedArray* array);
        SSDK_FNPTR(none, collapse, SaneLinkedArray* array);
    } linkedArray;

    struct {
        SSDK_FNPTR(none, create, u32 max, SaneHashArray* array);
        SSDK_FNPTR(none, destroy, SaneHashArray* array);
        
        SSDK_FNPTR(byte, put, cstr key, ptr value, SaneHashArray* array);
        SSDK_FNPTR(byte, pull, cstr key, SaneKeyValue* out, SaneHashArray* array);
        
        SSDK_FNPTR(cstr*, getKeys, SaneHashArray* array);
        SSDK_FNPTR(ptr*, getValues, SaneHashArray* array);
        SSDK_FNPTR(ptr, get, cstr key, SaneHashArray* array);
    } hashArray;

} SaneDS;
extern SaneDS* saneDS;

SSDK_FUNC byte ssdkInitDS(none);
SSDK_FUNC none ssdkExitDS(none);

#endif  // __SANEDATA_H__