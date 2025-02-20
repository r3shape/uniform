#include "../include/utility/r3_hashmap.h"
#include "../include/memory/r3_memory.h"

#include <stdio.h>

_R3_PRIVATE int r3_string_hash(const char* buffer) {
    int res = 0;
    size_t size = strlen(buffer);
    for(int i = 0; i < size; i++) {
        res+=(int)buffer[i]*31;
    }; return res;
}

_R3_PRIVATE ubyte r3_probe_hashmap_f(R3_Hashmap* m, int* kHash, const char* key) {
    if (!m || !kHash || !key) return R3_FALSE;
    ubyte match = R3_FALSE;
    R3_Key_Value* kvp = m->map[*kHash];
    
    for (int i = *kHash+1; i < m->max; i++) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp) {
                match = R3_TRUE;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp && !strcmp(key, kvp->k)) {
                *kHash = i;
                match = R3_TRUE;
                break;
            } else continue;
        }
    } return match;
}

_R3_PRIVATE ubyte r3_probe_hashmap_r(R3_Hashmap* m, int* kHash, const char* key) {
    if (!m || !kHash || !key) return R3_FALSE;
    ubyte match = R3_FALSE;
    R3_Key_Value* kvp = m->map[*kHash];
    
    for (int i = *kHash-1; i > 0; i--) {
        kvp = m->map[i];
        if (key == NULL) {
            if (!kvp) {
                match = R3_TRUE;
                *kHash = i;
                break;
            } else continue;
        } else {
            if (kvp && !strcmp(key, kvp->k)) {
                match = 1;
                *kHash = R3_TRUE;
                break;
            } else continue;
        }
    } return match;
}

R3_Hashmap* r3_create_hashmap(int max) {
    R3_Hashmap* m = (R3_Hashmap*)r3_memory_api->alloc(sizeof(R3_Hashmap), 16);
    if (!m) {
        // error: out of memory!
        return NULL;
    }

    m->max = max;
    m->count = 0;

    // TODO: r3_memory_api->calloc() needed!
    m->map = (R3_Key_Value**)calloc(max, sizeof(R3_Key_Value *));
    if (!m->map) {
        // error: out of memory!
        r3_memory_api->free(m);
        return NULL;
    }

    return m;
}

void r3_destroy_hashmap(R3_Hashmap* m) {
    for (int i = 0; i < m->max; i++) {
        if (m->map[i]) {
            r3_memory_api->free(m->map[i]);
        }
    }
    // TODO: r3_memory_api->calloc() needed for the following call:
    // r3_memory_api->free(m->map);
    free(m->map);   // for now system free is used since the address isnt aligned / no header info
    r3_memory_api->free(m);
}

void* r3_get_hashmap(R3_Hashmap* m, const char* key) {
    if (!key) { return NULL; }

    int kHash = r3_string_hash(key) % m->max;
    R3_Key_Value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        ubyte match = R3_FALSE;

        // forward probing
        match = r3_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = r3_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            // error: probing error!
            return NULL;
        }
        
        kvp = m->map[kHash];
    }; return (kvp) ? kvp->v : NULL;
}

ubyte r3_set_hashmap(R3_Hashmap* m, const char* key, void* value) {
    if (!key || !value || m->count+1 > m->max) { return R3_FALSE; }

    int kHash = r3_string_hash(key) % m->max;
    R3_Key_Value* kvp = m->map[kHash];

    // resolve collisions with open addressing + linear probing
    if (kvp) {
        if (!strcmp(key, kvp->k)) {
            m->map[kHash]->v = value;
            return R3_FALSE;        
        }

        ubyte set = R3_FALSE;
        
        // forward probing
        set = r3_probe_hashmap_f(m, &kHash, NULL);

        // reverse probing
        if (!set) set = r3_probe_hashmap_r(m, &kHash, NULL);

        if (!set) {
            // error: probing error!
            return R3_FALSE;
        }
    }

    m->map[kHash] = (R3_Key_Value*)r3_memory_api->alloc(sizeof(R3_Key_Value), 16);
    m->map[kHash]->k = strdup(key);
    m->map[kHash]->v = value;
    m->count++;

    return R3_TRUE;
}

ubyte r3_rem_hashmap(R3_Hashmap* m, const char* key) {
    if (!key) { return R3_FALSE; }

    int kHash = r3_string_hash(key) % m->max;
    R3_Key_Value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        ubyte match = R3_FALSE;

        // forward probing
        match = r3_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = r3_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            // error: probing error!
            return R3_FALSE;
        }
        
        kvp = m->map[kHash];
    };
    
    r3_memory_api->free(m->map[kHash]);
    m->map[kHash] = NULL;
    return R3_TRUE;
}
