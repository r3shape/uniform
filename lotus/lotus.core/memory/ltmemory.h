#ifndef LTMEMORY_H
#define LTMEMORY_H

#include "../defines.h"

typedef enum tagMemoryTag {
    LOTUS_MEMTAG_UNKNOWN=0,
    LOTUS_MEMTAG_ARRAY,
    LOTUS_MEMTAG_STRING,
    LOTUS_MEMTAG_HASHMAP,
    LOTUS_MEMTAG_JOB,
    LOTUS_MEMTAG_BST,
    LOTUS_MEMTAG_EVENT,
    LOTUS_MEMTAG_SCENE,
    LOTUS_MEMTAG_ENTITY,
    LOTUS_MEMTAG_TEXTURE,
    LOTUS_MEMTAG_MATERIAL,
    LOTUS_MEMTAG_RENDERER,
    LOTUS_MEMTAG_TRANSFORM,
    LOTUS_MEMTAG_ENTITY_NODE,
    LOTUS_MEMTAG_RING_QUEUE,
    LOTUS_MEMTAG_APPLICATION,
    
    LOTUS_MEMTAG_MAX
} LTmemoryTag;

void ltMemoryInit(void);
void ltMemoryExit(void);
void ltGetMemoryTagStr(char* buffer, LTmemoryTag tag);

LOTUS_API_ENTRY char* ltGetMemoryUsageStr(void);
LOTUS_API_ENTRY void ltMemZero(void* block, u64 size);
LOTUS_API_ENTRY void* ltMemAlloc(u64 size, LTmemoryTag tag);
LOTUS_API_ENTRY void* ltMemSet(void* block, i32 value, u64 size);
LOTUS_API_ENTRY void ltMemFree(void* block, u64 size, LTmemoryTag tag);
LOTUS_API_ENTRY void* ltMemCopy(void* dest, const void* source, u64 size);


#endif