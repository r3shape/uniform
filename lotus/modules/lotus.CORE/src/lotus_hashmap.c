#include "../include/utility/lotus_hashmap.h"

#include "../include/platform/lotus_logger.h"

_LOTUS_PRIVATE int lotus_string_hash(const char* buffer) {
    int res = 0;
    size_t size = strlen(buffer);
    for(int i = 0; i < size; i++) {
        res+=(int)buffer[i]*31;
    }; return res;
}

_LOTUS_PRIVATE bool lotus_probe_hashmap_f(Lotus_Hashmap* m, int* kHash, const char* key) {
    bool match = 0;
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

_LOTUS_PRIVATE bool lotus_probe_hashmap_r(Lotus_Hashmap* m, int* kHash, const char* key) {
    bool match = 0;
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

Lotus_Hashmap* lotus_make_hashmap(int max) {
    Lotus_Hashmap* m = (Lotus_Hashmap*)malloc(sizeof(Lotus_Hashmap));
    if (!m) {
        lotus_set_log_level(LOTUS_LOG_ERROR);
        lotus_log_error("failed to allocate hashmap");
        return NULL;
    }

    m->max = max;
    m->count = 0;

    m->map = (Lotus_Key_Value**)calloc(max, sizeof(Lotus_Key_Value*));
    if (!m->map) {
        free(m);
        lotus_set_log_level(LOTUS_LOG_ERROR);
        lotus_log_error("failed to allocate hashmap array");
        return NULL;
    }

    return m;
}

void lotus_destroy_hashmap(Lotus_Hashmap* m) {
    for (int i = 0; i < m->max; i++) {
        if (m->map[i]) {
            free(m->map[i]->v);
            free(m->map[i]);
        }
    }
    free(m->map);
    free(m);
}

void* lotus_get_hashmap(Lotus_Hashmap* m, const char* key) {
    if (!key) { return NULL; }

    int kHash = lotus_string_hash(key) % m->max;
    Lotus_Key_Value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        bool match = 0;

        // forward probing
        match = lotus_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = lotus_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            lotus_set_log_level(LOTUS_LOG_ERROR);
            lotus_log_error("probing error | key [%s] is not set", key);
            return NULL;
        }
        
        kvp = m->map[kHash];
    }; return kvp->v;
}

Lotus_Error_Type lotus_set_hashmap(Lotus_Hashmap* m, const char* key, void* value) {
    if (!key || !value || m->count+1 > m->max) { return LOTUS_ERR_TYPE; }

    int kHash = lotus_string_hash(key) % m->max;
    Lotus_Key_Value* kvp = m->map[kHash];

    // resolve collisions with open addressing + linear probing
    if (kvp) {
        if (!strcmp(key, kvp->k)) {
            m->map[kHash]->v = value;
            return LOTUS_ERR_NONE;        
        }

        bool set = 0;
        
        // forward probing
        set = lotus_probe_hashmap_f(m, &kHash, NULL);

        // reverse probing
        if (!set) set = lotus_probe_hashmap_r(m, &kHash, NULL);

        if (!set) {
            lotus_set_log_level(LOTUS_LOG_ERROR);
            lotus_log_error("probing error | key[%s]", key);
            return LOTUS_ERR_FUNC;
        }
    }

    m->map[kHash] = (Lotus_Key_Value*)malloc(sizeof(Lotus_Key_Value));
    m->map[kHash]->k = strdup(key);
    m->map[kHash]->v = value;
    m->count++;
    return LOTUS_ERR_NONE;
}

Lotus_Error_Type lotus_rem_hashmap(Lotus_Hashmap* m, const char* key) {
    if (!key) { return LOTUS_ERR_TYPE; }

    int kHash = lotus_string_hash(key) % m->max;
    Lotus_Key_Value* kvp = m->map[kHash];

    if (kvp && strcmp(key, kvp->k)) {
        bool match = 0;

        // forward probing
        match = lotus_probe_hashmap_f(m, &kHash, key);

        // reverse probing
        if (!match) {
            match = lotus_probe_hashmap_r(m, &kHash, key);
        }
        
        if (!match) {
            lotus_set_log_level(LOTUS_LOG_ERROR);
            lotus_log_error("probing error | key [%s] is not set", key);
            return LOTUS_ERR_FUNC;
        }
        
        kvp = m->map[kHash];
    };
    
    free(m->map[kHash]->v);
    free(m->map[kHash]);
    m->map[kHash] = NULL;
    return LOTUS_ERR_NONE;
}
