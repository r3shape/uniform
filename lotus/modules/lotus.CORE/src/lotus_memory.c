#include "../include/platform/lotus_memory.h"

#include "../include/platform/lotus_logger.h"

Lotus_Allocator lotus_make_allocator(size_t heap_size) {
    Lotus_Allocator alloc;
    
    alloc.heap_start = malloc(heap_size);
    if (alloc.heap_start == (void*)0) {
        return (Lotus_Allocator){NULL};  // allocation failed
    }

    alloc.heap_size = heap_size;
    alloc.free_list = (Lotus_Memory_Meta*)alloc.heap_start;

    alloc.free_list->size = heap_size - sizeof(Lotus_Memory_Meta);
    alloc.free_list->is_free = LOTUS_TRUE;
    alloc.free_list->next = NULL;

    return alloc;
}

Lotus_Memory lotus_alloc(Lotus_Allocator* alloc, size_t size, size_t alignment) {
    Lotus_Memory_Meta* current = alloc->free_list;
    Lotus_Memory mem = { .ptr = NULL, .size = 0 };

    size = lotus_align(size, alignment);

    while (current) {
        if (current->is_free && current->size >= size) {
            // split block if theres extra space
            if (current->size > size + sizeof(Lotus_Memory_Meta)) {
                Lotus_Memory_Meta* new = (Lotus_Memory_Meta*)((char*)current + size + sizeof(Lotus_Memory_Meta));
                new->size = current->size - size - sizeof(Lotus_Memory_Meta);
                new->is_free = LOTUS_TRUE;
                new->next = current->next;

                current->size = size;
                current->next = new;
            }
            current->is_free = LOTUS_FALSE;
            mem.ptr = (char*)current + sizeof(Lotus_Memory_Meta);
            mem.size = current->size;
            return mem;
        }
        current = current->next;
    }
    // allocation failed
    return mem;
}

void lotus_free(Lotus_Allocator* alloc, Lotus_Memory mem) {
    Lotus_Memory_Meta* block = (Lotus_Memory_Meta*)((char*)mem.ptr - sizeof(Lotus_Memory_Meta));
    block->is_free = LOTUS_TRUE;

    // coalesce adjacent free blocks
    Lotus_Memory_Meta* current = alloc->free_list;

    while (current) {
        if (current->is_free && current->next && current->next->is_free) {
            current->size += current->next->size + sizeof(Lotus_Memory_Meta);
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}

void lotus_destroy_allocator(Lotus_Allocator* alloc) {
    free(alloc->heap_start);
    alloc->heap_start = NULL;
    alloc->heap_size = 0;
    alloc->free_list = NULL;
}
