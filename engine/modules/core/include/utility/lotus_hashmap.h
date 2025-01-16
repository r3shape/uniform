#pragma once

#include "../lotus_core_defines.h"

typedef struct Lotus_Key_Value {
    const char* k;
    void* v;
} Lotus_Key_Value;

typedef struct Lotus_Hashmap {
    int max;
    int count;
    Lotus_Key_Value** map;
} Lotus_Hashmap;

/**
 * @brief Creates a new hashmap with a specified maximum number of entries.
 * 
 * @param max The maximum number of entries the hashmap can hold.
 * @return A pointer to the newly created hashmap, or NULL on failure.
 */
LOTUS_API Lotus_Hashmap* lotus_create_hashmap(int max);

/**
 * @brief Destroys a hashmap and frees all associated memory.
 * 
 * @param m A pointer to the hashmap to destroy.
 */
LOTUS_API void lotus_destroy_hashmap(Lotus_Hashmap* m);

/**
 * @brief Retrieves a value from the hashmap using the specified key.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to look up.
 * @return A pointer to the value associated with the key, or NULL if the key is not found.
 */
LOTUS_API void* lotus_get_hashmap(Lotus_Hashmap* m, const char* key);

/**
 * @brief Removes a key-value pair from the hashmap.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to remove.
 * @return `LOTUS_TRUE` on success, or `LOTUS_FALSE` if the operation fails.
 */
LOTUS_API ubyte lotus_rem_hashmap(Lotus_Hashmap* m, const char* key);

/**
 * @brief Sets a key-value pair in the hashmap. If the key already exists, its value is updated.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to set.
 * @param value A pointer to the value to associate with the key.
 * @return `LOTUS_TRUE` on success, or `LOTUS_FALSE` if the operation fails.
 */
LOTUS_API ubyte lotus_set_hashmap(Lotus_Hashmap* m, const char* key, void* value);
