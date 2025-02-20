#include "../../include/memory/r3_memory.h"

#include <stdio.h>

R3_Memory_API* r3_memory_api = NULL;

/* MEMORY API IMPLEMENTATION */
void *_memory_alloc_impl(ubyte8 size, ubyte8 align) {
    if (!size || !align) return NULL;   // error: value error!

    /**
     * Ensure alignment value is a power of two.
     */
    if (!((align & (align - 1)) == 0)) return NULL;   // error: align must be a power of two!
    
    void *alignedPtr = NULL;

    /**
     * Allocate extra bytes for memory header and alignment
     */
    void *offPtr = malloc(MEMORY_PAD_SIZE(align) + size);
    if (offPtr) {
        /**
         * Align the `user` portion of memory to the target size.
         */
        alignedPtr = (void*)ALIGN_ADDRESS((ubyte8)((ubyte2 *)offPtr + 1), align);
        
        /**
         * Calculate the offset, and store it in front of the aligned ptr.
         */
        *((ubyte2 *)alignedPtr - 1) =
            (ubyte2)((ubyte8)alignedPtr - (ubyte8)offPtr);
    }
    return alignedPtr;
}

void *_memory_realloc_impl(void* ptr, ubyte8 size, ubyte8 align) {
    if (!size || !align) return NULL;   // error: value error!

    // if pointer is NULL, act like malloc.
    if (!ptr) return _memory_alloc_impl(size, align);

    /**
     * Ensure alignment value is a power of two.
     */
    if (!((align & (align - 1)) == 0)) return NULL;   // error: align must be a power of two!
    
    void *alignedPtr = NULL;

    /**
     * Walk back to original `offPtr`
     * Allocate extra bytes for memory header and alignment
     */
    void* offPtr = realloc((void *)((ubyte8)ptr - *((ubyte2 *)ptr - 1)), MEMORY_PAD_SIZE(align) + size);
    if (offPtr) {

        /**
         * Align the `user` portion of memory to the target size
         */
        alignedPtr = (void *)ALIGN_ADDRESS((ubyte8)((ubyte2 *)offPtr - 1), align);

        /**
         * Calculate and store ptr offset in front of the aligned ptr;
         */
        *((ubyte2 *)alignedPtr - 1) =
            (ubyte2)((ubyte8)alignedPtr - (ubyte8)offPtr);
    }
    return alignedPtr;
}

void _memory_free_impl(void *ptr) {
    /**
     * Walk back to offPtr, and free from there.
     */
    free((void *)((ubyte8)ptr - *((ubyte2 *)ptr - 1)));
}

ubyte _memory_init_region_impl(R3_Memory_Region *region, ubyte8 stride, ubyte8 capacity, ubyte8 align) {
    if (!region) return R3_FALSE;  // err: nul ptr!
    if (!((align & (align - 1)) == 0)) return R3_FALSE;    // error: align must be power of two!

    region->last = NULL;
    region->next = NULL;
    region->capacity = capacity;
    region->stride = stride;
    region->count = 0;

    region->data = (ubyte8 *)_memory_alloc_impl(stride * capacity, align);
    if (!region->data) return R3_FALSE;    // error: out of memory!

    return R3_TRUE;
}

ubyte _memory_spawn_region_impl(R3_Memory_Region *start, ubyte8 stride, ubyte8 capacity, ubyte8 align) {
    if (!start) return R3_FALSE;   // error: null ptr!

    start->next = (R3_Memory_Region *)_memory_alloc_impl(sizeof(R3_Memory_Region), align);
    if (!start->next) return R3_FALSE; // error: out of memory!

    if (!_memory_init_region_impl(start->next, stride, capacity, align)) {
        _memory_free_impl(start->next);
        start->next = NULL;
        return R3_FALSE;   // error: out of memory!
    }
    
    start->next->last = start;

    return R3_TRUE;
}

R3_Memory_Region *_memory_get_region_impl(R3_Memory_Region *start, sbyte8 step) {
    if (!start) return NULL;            // error: null ptr!
    if (step == 0) return start;        // error: return start region!

    R3_Memory_Region *region = start;
    if (step > 0) {
        for (ubyte8 i = 0; i < step; i++) {
            if (region->next == NULL) return NULL;  // error: index error / region not found!
            region = region->next;
        }
    } else {
        for (ubyte8 i = 0; i > step; i--) {
            if (region->last == NULL) return NULL;  // error: index error / region not found!
            region = region->last;
        }
    }

    return region;
}

ubyte _memory_link_region_impl(R3_Memory_Region *start, R3_Memory_Region *region, sbyte8 step) {
    if (!start || !region) return R3_FALSE;   // error: null ptr!
    
    R3_Memory_Region *target = _memory_get_region_impl(start, step);
    if (!target) return R3_FALSE;  // error: index error / region not found!

    // TODO: link all regions linked to parameter region
    if (target->next != NULL) target->next->last = region;
    region->next = target->next;
    region->last = target;
    target->next = region;

    return R3_TRUE;
}

ubyte _memory_unlink_region_impl(R3_Memory_Region *region) {
    if (!region) return R3_FALSE;  // error: null ptr!

    if (region->last != NULL) region->last = (region->next != NULL) ? region->next : NULL;
    if (region->next != NULL) region->next = (region->last != NULL) ? region->last : NULL;

    region->last = NULL;
    region->next = NULL;

    return R3_TRUE;
}

void _memory_free_region_impl(R3_Memory_Region *region) {
    if (!region) return;    // error: null ptr!
    
    // "re-wire" adjacent regions to bypass the current
    if (region->last != NULL) region->last->next = (region->next != NULL) ? region->next : NULL;
    if (region->next != NULL) region->next->last = (region->last != NULL) ? region->last : NULL;

    region->last = NULL;
    region->next = NULL;
    region->capacity = 0;
    region->stride = 0;
    region->count = 0;
    
    _memory_free_impl((void *)region->data);
}

void _memory_free_all_regions_impl(R3_Memory_Region *start) {
    if (!start) return; // error: null ptr!
    
    R3_Memory_Region *current = start;
    R3_Memory_Region *nextRegion = NULL;
    while (current != NULL) {
        nextRegion = current->next;
        _memory_free_region_impl(current);
        current = nextRegion;
    }
}

ubyte _memory_set_region_data_impl(R3_Memory_Region *region, void *ptr) {
    if (!region) return R3_FALSE;  // error: null ptr!
    
    // TODO: handle d-array resizing (r3_realloc() needed)
    if (region->count + 1 > region->capacity) return R3_FALSE; // error: data array exhausted!
    
    region->data[region->count] = (ubyte8)ptr;
    region->count++;
    return R3_TRUE;
}

ubyte8 _memory_get_region_data_impl(R3_Memory_Region *region, ubyte8 index) {
    if (!region) return R3_FALSE;  // error: null ptr!
    if (index >= region->count) return R3_FALSE; // error: index out of bounds!
    
    return region->data[index];
}

/* MEMORY API IMPLEMENTATION */

ubyte r3_init_memory(void) {
    r3_memory_api = (R3_Memory_API*)malloc(sizeof(R3_Memory_API));
    if (!r3_memory_api) return R3_FALSE; // error: out of memory!

    r3_memory_api->alloc = _memory_alloc_impl;
    r3_memory_api->realloc = _memory_realloc_impl;
    r3_memory_api->free = _memory_free_impl;

    r3_memory_api->init_region = _memory_init_region_impl;
    r3_memory_api->spawn_region = _memory_spawn_region_impl;
    r3_memory_api->get_region = _memory_get_region_impl;
    r3_memory_api->link_region = _memory_link_region_impl;
    r3_memory_api->unlink_region = _memory_unlink_region_impl;
    r3_memory_api->free_region = _memory_free_region_impl;
    r3_memory_api->free_all_regions = _memory_free_all_regions_impl;
    
    r3_memory_api->get_region_data = _memory_get_region_data_impl;
    r3_memory_api->set_region_data = _memory_set_region_data_impl;
    
    return R3_TRUE;
}

void r3_shutdown_memory(void) {
    free(r3_memory_api);
}
