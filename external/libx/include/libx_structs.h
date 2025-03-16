#pragma once

#include "libx_def.h"

/* ---------------- ARRAY ---------------- */
typedef struct Array_Head {
    u32 count, max;
    u32 size, stride;
} Array_Head;

typedef enum Array_Head_Field {
    LIBX_ARRAY_SIZE_FIELD = 0,
    LIBX_ARRAY_STRIDE_FIELD,
    LIBX_ARRAY_COUNT_FIELD,
    LIBX_ARRAY_MAX_FIELD,
    LIBX_ARRAY_FIELDS,
} Array_Head_Field;
/* ---------------- ARRAY ---------------- */


/* ---------------- LINKED ARRAY ---------------- */
typedef struct Linked_Array {
    struct Linked_Array* last;
    struct Linked_Array* next;
    void* array;
    Array_Head meta;
} Linked_Array;
/* ---------------- LINKED ARRAY ---------------- */


/* ---------------- HASH ARRAY ---------------- */
typedef struct Key_Value {
    void* value;
    cstr key;
} Key_Value;

typedef struct Hash_Array {
    Key_Value* map;
    Array_Head meta;
} Hash_Array;
/* ---------------- HASH ARRAY ---------------- */


typedef struct _libx_structs_api {
    void (*destroy_array)(void* array);

    /**
     * `push_array` is used to ensure a value is inserted at the next available slot in an array BASED ON THE CURRENT NUMBER OF ELEMENTS within the array.
     * Be careful using `push_array` after `n` calls to `put_array` as `push_array` will skip the first `n` slots in an array, and may over-write the inserts.
     */
    void (*push_array)(void* array, void* invalue);

    /**
     * `pop_array` is used to ensure a value is returned from the last occupied slot in an array BASED ON THE CURRENT NUMBER OF ELEMENTS within the array.
     * Be careful using `pop_array` after `n` calls to `put_array` as `pop_array` will start at index `n - 1` in an array, and fail to return inserted values.
     */
    void (*pop_array)(void* array, void* outvalue);
    
    void* (*create_array)(u32 stride, u32 max);
    void* (*resize_array)(void* array, u32 max);
    
    /**
     * `put_array` is used to manually write data to an array.
     * The data written with `put_array` is over-written with `n` calls to `push_array`, where `n` is equal to the `index` inserted to, ONLY IF the `index` inserted to is greater than the number of elements in the array.
     * Calling `push_array` after `n` calls to `put_array` ensures the first `n` slots of the array are never over-written by subsequent calls to `push_array`.
     */
    void (*put_array)(void* array, u32 index, void* invalue);
    
    /**
     * `pull_array` is used to return a value from a specific index in an array
     * Calling `pull_array` DOES shift/reorder elements in the array, instead the value at the removed index is set to 0, maintaining the integrity of the structure of data within the array.
     */
    void (*pull_array)(void* array, u32 index, void* outvalue);

    /**
     * `get_array_head` is used to return the metadata about an array that is stored in memory along with the array.
     */
    Array_Head (*get_array_head)(void* array);

    /**
     * `create_linked_array` dynamically allocates and returns a doubly-linked `Linked_Array` structure which is handy for linking blocks of memory containing *n* elements.
     * Any `Linked_Array` structure created with `create_linked_array` has its array field allocated via `create_array`, thus all other array API functions are applicable.  
     */
    Linked_Array* (*create_linked_array)(Linked_Array* array, u32 stride, u32 max);
    
    /**
     * `destroy_linked_array` destroys a specific link within a linked array structure whilst maintaining structural integrity.
     * Use `collapse_linked_array` to recursively destroy all links in a linked array structure from any link in the structure.
     */
    void (*destroy_linked_array)(Linked_Array* array);

    /**
     * `collapse_linked_array` recursively collapses an entire linked array structure from any link in the structure.
     * Use `destroy_linked_array` to destroy a specific link whilst maintaining structural integrity.
     */
    void (*collapse_linked_array)(Linked_Array* array);

    Hash_Array* (*create_hash_array)(u32 max);
    u8 (*put_hash_array)(Hash_Array* array, cstr key, void* value);
    void* (*get_hash_array)(Hash_Array* array, cstr key);
    u8 (*pull_hash_array)(Hash_Array* array, cstr key, Key_Value* out);
    void (*destroy_hash_array)(Hash_Array* array);
} _libx_structs_api;
extern _libx_structs_api* structs_api;

LIBX_API u8 libx_init_structs(void);
LIBX_API void libx_cleanup_structs(void);
