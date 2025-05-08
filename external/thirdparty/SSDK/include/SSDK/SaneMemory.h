#ifndef __SANEMEMORY_H__
#define __SANEMEMORY_H__

#include <include/SSDK/globals.h>

typedef struct SaneBlob {
    ptr data;
    u64 size;
} SaneBlob;

typedef enum SaneAllocatorType {
    SANE_ALLOC_DEFAULT,
    SANE_ALLOC_ARENA,
    SANE_ALLOC_TYPES
} SaneAllocatorType;

typedef struct SaneAllocator {
    SaneAllocatorType type;
    union context {
        struct arena {
            ptr buffer;
            u64 offset;
            u64 commit;
            u64 max;
        } arena;
    } context;
} SaneAllocator;

typedef struct SaneMemory {
    SSDK_FNPTR(ptr, alloc, u64 size, u64 align);
    SSDK_FNPTR(none, dealloc, ptr mem);
    SSDK_FNPTR(none, zero, ptr mem, u64 size);
    SSDK_FNPTR(ptr, realloc, ptr mem, u64 size, u64 align);
    
    SSDK_FNPTR(byte, blob_alloc, u64 align, SaneBlob* blob);
    SSDK_FNPTR(byte, blob_realloc, u64 size, u64 align, SaneBlob* blob);
    SSDK_FNPTR(byte, blob_dealloc, SaneBlob* blob);
    
    SSDK_FNPTR(byte, create_allocator, SaneAllocatorType type, u64 max, u64 align, SaneAllocator* allocator);
    SSDK_FNPTR(byte, destroy_allocator, SaneAllocator* allocator);
    
    SSDK_FNPTR(ptr, arena_alloc, u64 size, u64 align, SaneAllocator* allocator);
    SSDK_FNPTR(none, arena_reset, SaneAllocator* allocator);
    SaneModule module;
} SaneMemory;
extern SaneMemory* saneMemory;

SSDK_FUNC byte ssdkInitMemory(none);
SSDK_FUNC none ssdkExitMemory(none);

#endif  // __SANEMEMORY_H__