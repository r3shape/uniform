#ifndef __SANEMEMORY_H__
#define __SANEMEMORY_H__

#include <include/SSDK/globals.h>

typedef struct Blob {
    ptr data;
    u64 size;
} Blob;

typedef enum AllocatorType {
    SANE_ALLOC_DEFAULT,
    SANE_ALLOC_ARENA,
    SANE_ALLOC_TYPES
} AllocatorType;

typedef struct Allocator {
    AllocatorType type;
    union context {
        struct arena {
            ptr buffer;
            u64 offset;
            u64 commit;
            u64 max;
        } arena;
    } context;
} Allocator;

typedef struct SaneMemory {
    SSDK_FNPTR(ptr, alloc, u64 size, u64 align);
    SSDK_FNPTR(none, dealloc, ptr mem);
    SSDK_FNPTR(none, zero, ptr mem, u64 size);
    SSDK_FNPTR(ptr, realloc, ptr mem, u64 size, u64 align);
    
    SSDK_FNPTR(byte, blobAlloc, u64 align, Blob* blob);
    SSDK_FNPTR(byte, blobRealloc, u64 size, u64 align, Blob* blob);
    SSDK_FNPTR(byte, blobDealloc, Blob* blob);
    
    SSDK_FNPTR(byte, createAllocator, AllocatorType type, u64 max, u64 align, Allocator* allocator);
    SSDK_FNPTR(byte, destroyAllocator, Allocator* allocator);
    
    SSDK_FNPTR(ptr, arenaAlloc, u64 size, u64 align, Allocator* allocator);
    SSDK_FNPTR(none, arenaReset, Allocator* allocator);
    SaneModule module;
} SaneMemory;
extern SaneMemory* saneMemory;

SSDK_FUNC byte ssdkInitMemory(none);
SSDK_FUNC none ssdkExitMemory(none);

#endif  // __SANEMEMORY_H__