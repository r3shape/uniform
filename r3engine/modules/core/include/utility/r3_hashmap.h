#pragma once

#include "../r3_core_defines.h"

typedef struct R3_Key_Value {
    const char* k;
    void* v;
} R3_Key_Value;

typedef struct R3_Hashmap {
    int max;
    int count;
    R3_Key_Value** map;
} R3_Hashmap;

/**
 * @brief Creates a new hashmap with a specified maximum number of entries.
 * 
 * @param max The maximum number of entries the hashmap can hold.
 * @return A pointer to the newly created hashmap, or NULL on failure.
 */
R3_API R3_Hashmap* r3_create_hashmap(int max);

/**
 * @brief Destroys a hashmap and frees all associated memory.
 * 
 * @param m A pointer to the hashmap to destroy.
 */
R3_API void r3_destroy_hashmap(R3_Hashmap* m);

/**
 * @brief Retrieves a value from the hashmap using the specified key.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to look up.
 * @return A pointer to the value associated with the key, or NULL if the key is not found.
 */
R3_API void* r3_get_hashmap(R3_Hashmap* m, const char* key);

/**
 * @brief Removes a key-value pair from the hashmap.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to remove.
 * @return `R3_TRUE` on success, or `R3_FALSE` if the operation fails.
 */
R3_API ubyte r3_rem_hashmap(R3_Hashmap* m, const char* key);

/**
 * @brief Sets a key-value pair in the hashmap. If the key already exists, its value is updated.
 * 
 * @param m A pointer to the hashmap.
 * @param key The key to set.
 * @param value A pointer to the value to associate with the key.
 * @return `R3_TRUE` on success, or `R3_FALSE` if the operation fails.
 */
R3_API ubyte r3_set_hashmap(R3_Hashmap* m, const char* key, void* value);
