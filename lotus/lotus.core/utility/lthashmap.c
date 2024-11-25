#include "lthashmap.h"

#include "../memory/ltmemory.h"
#include "../platform/ltlogger.h"

int ltStringHash(const char* buffer) {
    int res = 0;
    size_t size = strlen(buffer);
    for(int i = 0; i < size; i++) {
        res+=(int)buffer[i]*31;
    }; return res;
}

LThashmap* ltMakeHashmap(int max) {
    LThashmap* m = (LThashmap*)ltMemAlloc(sizeof(LThashmap), LOTUS_MEMTAG_HASHMAP);
    if (!m) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate hashmap");
        return NULL;
    }

    m->max = max;
    m->count = 0;

    m->map = (LTkeyValue**)calloc(max, sizeof(LTkeyValue*));
    if (!m->map) {
        ltMemFree(m, sizeof(LThashmap), LOTUS_MEMTAG_HASHMAP);
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate hashmap array");
        return NULL;
    }

    return m;
}

void ltDestroyHashmap(LThashmap* m) {
    for (int i = 0; i < m->max; i++) {
        if (m->map[i]) {
            ltMemFree(m->map[i]->v, sizeof(m->map[i]->v), LOTUS_MEMTAG_HASHMAP);
            ltMemFree(m->map[i], sizeof(m->map[i]), LOTUS_MEMTAG_HASHMAP);
        }
    }
    ltMemFree(m->map, m->count * sizeof(LTkeyValue), LOTUS_MEMTAG_HASHMAP);
    ltMemFree(m, sizeof(LThashmap), LOTUS_MEMTAG_HASHMAP);
}


bool ltProbeHashmapF(LThashmap* m, int* kHash, const char* key) {
    bool match = 0;
    LTkeyValue* kvp = m->map[*kHash];
    
    for (int i = *kHash+1; i < m->max; i++) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp && !strcmp(key, kvp->k)) {
                *kHash = i;
                match = 1;
                break;
            } else continue;
        }
    } return match;
}

bool ltProbeHashmapR(LThashmap* m, int* kHash, const char* key) {
    bool match = 0;
    LTkeyValue* kvp = m->map[*kHash];
    
    for (int i = *kHash-1; i > 0; i--) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp && !strcmp(key, kvp->k)) {
                match = 1;
                *kHash = i;
                break;
            } else continue;
        }
    } return match;
}


void* ltGetHashmap(LThashmap* m, const char* key) {
    if (!key) { return NULL; }

    int kHash = ltStringHash(key) % m->max;
    LTkeyValue* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        bool match = 0;

        // forward probing
        match = ltProbeHashmapF(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = ltProbeHashmapR(m, &kHash, key);
        }
        
        if (!match) {
            ltSetLogLevel(LOTUS_LOG_ERROR);
            ltLogError("probing error | key [%s] is not set", key);
            return NULL;
        }
        
        kvp = m->map[kHash];
    }; return kvp->v;
}

LTerrorType ltSetHashmap(LThashmap* m, const char* key, void* value) {
    if (!key || !value || m->count+1 > m->max) { return LOTUS_ERR_PROCESS; }

    int kHash = ltStringHash(key) % m->max;
    LTkeyValue* kvp = m->map[kHash];

    // resolve collisions with open addressing + linear probing
    if (kvp) {
        if (!strcmp(key, kvp->k)) {
            m->map[kHash]->v = value;
            return LOTUS_ERR_NONE;        
        }

        bool set = 0;
        
        // forward probing
        set = ltProbeHashmapF(m, &kHash, NULL);

        // reverse probing
        if (!set) set = ltProbeHashmapR(m, &kHash, NULL);

        if (!set) {
            ltSetLogLevel(LOTUS_LOG_ERROR);
            ltLogError("probing error | key[%s]", key);
            return LOTUS_ERR_MALLOC;
        }
    }

    m->map[kHash] = (LTkeyValue*)ltMemAlloc(sizeof(LTkeyValue), LOTUS_MEMTAG_HASHMAP);
    m->map[kHash]->k = strdup(key);
    m->map[kHash]->v = value;
    m->count++;
    return LOTUS_ERR_NONE;
}

LTerrorType ltRemHashmap(LThashmap* m, const char* key) {
    if (!key) { return LOTUS_ERR_PROCESS; }

    int kHash = ltStringHash(key) % m->max;
    LTkeyValue* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        bool match = 0;

        // forward probing
        match = ltProbeHashmapF(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = ltProbeHashmapR(m, &kHash, key);
        }
        
        if (!match) {
            ltSetLogLevel(LOTUS_LOG_ERROR);
            ltLogError("probing error | key [%s] is not set", key);
            return LOTUS_ERR_MALLOC;
        }
        
        kvp = m->map[kHash];
    };
    
    ltMemFree(m->map[kHash]->v, sizeof(m->map[kHash]->v), LOTUS_MEMTAG_HASHMAP);
    ltMemFree(m->map[kHash], sizeof(m->map[kHash]), LOTUS_MEMTAG_HASHMAP);
    m->map[kHash] = NULL;
    return LOTUS_ERR_NONE;
}
