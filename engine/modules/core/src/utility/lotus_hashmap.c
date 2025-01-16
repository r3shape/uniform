#include "../include/utility/lotus_hashmap.h"
#include "../include/memory/lotus_memory.h"

#include <stdio.h>

_LOTUS_PRIVATE int lotus_string_hash(const char* buffer) {
    int res = 0;
    size_t size = strlen(buffer);
    for(int i = 0; i < size; i++) {
        res+=(int)buffer[i]*31;
    }; return res;
}

_LOTUS_PRIVATE ubyte lotus_probe_hashmap_f(Lotus_Hashmap* m, int* kHash, const char* key) {
    ubyte match = 0;
    Lotus_Key_Value* kvp = m->map[*kHash];
    
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

_LOTUS_PRIVATE ubyte lotus_probe_hashmap_r(Lotus_Hashmap* m, int* kHash, const char* key) {
    ubyte match = 0;
    Lotus_Key_Value* kvp = m->map[*kHash];
    
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

Lotus_Hashmap* lotus_create_hashmap(int max) {
    Lotus_Hashmap* m = (Lotus_Hashmap*)lotus_memory_api->alloc(sizeof(Lotus_Hashmap), 16);
    if (!m) {
        // error: out of memory!
        return NULL;
    }

    m->max = max;
    m->count = 0;

    // TODO: lotus_memory_api->calloc() needed!
    m->map = (Lotus_Key_Value**)calloc(max, sizeof(Lotus_Key_Value *));
    if (!m->map) {
        // error: out of memory!
        lotus_memory_api->free(m);
        return NULL;
    }

    return m;
}

void lotus_destroy_hashmap(Lotus_Hashmap* m) {
    for (int i = 0; i < m->max; i++) {
        if (m->map[i]) {
            lotus_memory_api->free(m->map[i]);
        }
    }
    // TODO: lotus_memory_api->calloc() needed for the following call:
    // lotus_memory_api->free(m->map);
    free(m->map);   // for now system free is used since the address isnt aligned / no header info
    lotus_memory_api->free(m);
}

void* lotus_get_hashmap(Lotus_Hashmap* m, const char* key) {
    if (!key) { return NULL; }

    int kHash = lotus_string_hash(key) % m->max;
    Lotus_Key_Value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        ubyte match = 0;

        // forward probing
        match = lotus_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = lotus_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            // error: probing error!
            return NULL;
        }
        
        kvp = m->map[kHash];
    }; return kvp->v;
}

ubyte lotus_set_hashmap(Lotus_Hashmap* m, const char* key, void* value) {
    if (!key || !value || m->count+1 > m->max) { return LOTUS_FALSE; }

    int kHash = lotus_string_hash(key) % m->max;
    Lotus_Key_Value* kvp = m->map[kHash];

    // resolve collisions with open addressing + linear probing
    if (kvp) {
        if (!strcmp(key, kvp->k)) {
            m->map[kHash]->v = value;
            return LOTUS_FALSE;        
        }

        ubyte set = 0;
        
        // forward probing
        set = lotus_probe_hashmap_f(m, &kHash, NULL);

        // reverse probing
        if (!set) set = lotus_probe_hashmap_r(m, &kHash, NULL);

        if (!set) {
            // error: probing error!
            return LOTUS_FALSE;
        }
    }

    m->map[kHash] = (Lotus_Key_Value*)lotus_memory_api->alloc(sizeof(Lotus_Key_Value), 16);
    m->map[kHash]->k = strdup(key);
    m->map[kHash]->v = value;
    m->count++;

    return LOTUS_TRUE;
}

ubyte lotus_rem_hashmap(Lotus_Hashmap* m, const char* key) {
    if (!key) { return LOTUS_FALSE; }

    int kHash = lotus_string_hash(key) % m->max;
    Lotus_Key_Value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        ubyte match = 0;

        // forward probing
        match = lotus_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = lotus_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            // error: probing error!
            return LOTUS_FALSE;
        }
        
        kvp = m->map[kHash];
    };
    
    lotus_memory_api->free(m->map[kHash]);
    m->map[kHash] = NULL;
    return LOTUS_TRUE;
}
