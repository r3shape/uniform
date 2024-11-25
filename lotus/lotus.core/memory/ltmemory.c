#include "ltmemory.h"

#include "../platform/ltlogger.h"
#include "../platform/ltplatform.h"

typedef struct tagMemoryStats {
    u64 totalAlloc;
    u64 taggedAlloc[LOTUS_MEMTAG_MAX];
} LTmemoryStats;
static LTmemoryStats ltMemStats;

void ltMemoryInit(void) {
    _ltMemZero(&ltMemStats, sizeof(LTmemoryStats));
}

void ltMemoryExit(void) {

}

void ltMemZero(void* block, u64 size) { _ltMemZero(block, size); }

void* ltMemAlloc(u64 size, LTmemoryTag tag) {
    if (tag == LOTUS_MEMTAG_UNKNOWN) {
        ltLogWarning("ltMemAlloc called using MEMTAG_UNKNOWN. Re-class this allocation.\n");
    }

    ltMemStats.totalAlloc += size;
    ltMemStats.taggedAlloc[tag] += size;

    // TODO: handle memory-alignment
    void* block = _ltMemAlloc(size, LOTUS_FALSE);
    _ltMemZero(block, size);
    return block;
}

void* ltMemSet(void* block, i32 value, u64 size) { return _ltMemSet(block, value, size); }

void ltMemFree(void* block, u64 size, LTmemoryTag tag) {
    if (tag == LOTUS_MEMTAG_UNKNOWN) {
        ltLogWarning("ltMemFree called using MEMTAG_UNKNOWN. Re-class this allocation.\n");
    }

    ltMemStats.totalAlloc -= size;
    ltMemStats.taggedAlloc[tag] -= size;
    
    // TODO: handle memory-alignment
    _ltMemFree(block, LOTUS_FALSE);
}

void* ltMemCopy(void* dest, const void* source, u64 size) { return _ltMemCopy(dest, source, size); }

void ltGetMemoryTagStr(char* buffer, LTmemoryTag tag) {
    switch (tag) {
        case LOTUS_MEMTAG_VALUE: {
            sprintf(buffer, "DYNAMIC VALUE");
            break;
        }
        case LOTUS_MEMTAG_ARRAY: {
            sprintf(buffer, "DYNAMIC ARRAY");
            break;
        }
        case LOTUS_MEMTAG_STRING: {
            sprintf(buffer, "STRING");
            break;
        }
        case LOTUS_MEMTAG_HASHMAP: {
            sprintf(buffer, "HASHMAP");
            break;
        }
        case LOTUS_MEMTAG_JOB: {
            sprintf(buffer, "JOB");
            break;
        }
        case LOTUS_MEMTAG_BST: {
            sprintf(buffer, "BST");
            break;
        }
        case LOTUS_MEMTAG_SCENE: {
            sprintf(buffer, "SCENE");
            break;
        }
        case LOTUS_MEMTAG_ENTITY: {
            sprintf(buffer, "ENTITY");
            break;
        }
        case LOTUS_MEMTAG_TEXTURE: {
            sprintf(buffer, "TEXTURE");
            break;
        }
        case LOTUS_MEMTAG_MATERIAL: {
            sprintf(buffer, "MATERIAL");
            break;
        }
        case LOTUS_MEMTAG_RENDERER: {
            sprintf(buffer, "RENDERER");
            break;
        }
        case LOTUS_MEMTAG_TRANSFORM: {
            sprintf(buffer, "TRANSFORM");
            break;
        }
        case LOTUS_MEMTAG_ENTITY_NODE: {
            sprintf(buffer, "ENTITY NODE");
            break;
        }
        case LOTUS_MEMTAG_RING_QUEUE: {
            sprintf(buffer, "RING QUEUE");
            break;
        }
        case LOTUS_MEMTAG_APPLICATION: {
            sprintf(buffer, "APPLICATION");
            break;
        }
        case LOTUS_MEMTAG_MAX: break;
        default:
            break;
    }
}

char* ltGetMemoryUsageStr(void) {
    const u64 kib = 1024;
    const u64 mib = 1024 * 1024;
    const u64 gib = 1024 * 1024 * 1024;
    char buffer[8000] = "Lotus Engine Memory Usage (tagged):\n";
    u64 bufferOffset = strlen(buffer);

    for (int i = 1; i < LOTUS_MEMTAG_MAX; ++i) {
        f32 amount = 1.0f;
        char unit[4] = "Xib";
        if (ltMemStats.taggedAlloc[i] >= gib) {
            unit[0] = 'G';
            amount = ltMemStats.taggedAlloc[i] / (f32)gib;
        } else if (ltMemStats.taggedAlloc[i] >= mib) {
            unit[0] = 'M';
            amount = ltMemStats.taggedAlloc[i] / (f32)mib;
        } else if (ltMemStats.taggedAlloc[i] >= kib) {
            unit[0] = 'K';
            amount = ltMemStats.taggedAlloc[i] / (f32)kib;
        } else {
            unit[0] = 'B'; unit[1] = '\0';
            amount = (f32)ltMemStats.taggedAlloc[i];
        }
        char tagBuffer[25];
        ltGetMemoryTagStr(tagBuffer, i);
        i32 written = snprintf(buffer + bufferOffset, 8000, "%s: %.2f%s\n", tagBuffer, amount, unit);
        bufferOffset += written;
    }
    return _strdup(buffer);
}
