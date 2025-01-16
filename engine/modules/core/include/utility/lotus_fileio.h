#pragma once

#include "../lotus_core_defines.h"

/**
 * @brief Checks if a file exists at the given path.
 * 
 * @param path The path to the file.
 * @return 1 if the file exists, 0 otherwise.
 */
LOTUS_API ubyte lotus_is_file(const char* path);

/**
 * @brief Reads the contents of a file into a dynamically allocated string.
 * 
 * @param path The path to the file.
 * @return A pointer to the file's content as a null-terminated string, or NULL on failure. 
 *         The caller is responsible for freeing the allocated memory.
 */
LOTUS_API char* lotus_read_file(const char* path);

/**
 * @brief Deletes a file at the specified path.
 * 
 * @param path The path to the file.
 * @return 1 on success, 0 on failure.
 */
LOTUS_API ubyte lotus_delete_file(const char* path);

/**
 * @brief Retrieves the size of a file in bytes.
 * 
 * @param path The path to the file.
 * @return The size of the file in bytes, or -1 on failure.
 */
LOTUS_API long lotus_get_file_size(const char* path);

/**
 * @brief Copies a file from the source path to the destination path.
 * 
 * @param dest The destination path where the file will be copied.
 * @param src The source path of the file to copy.
 * @return 1 on success, 0 on failure.
 */
LOTUS_API ubyte lotus_copy_file(const char* dest, const char* src);

/**
 * @brief Appends data to the end of a file.
 * 
 * @param data The data to append to the file.
 * @param path The path to the file.
 * @param newline If 1, appends a newline after the data; otherwise, no newline is added.
 * @return 1 on success, 0 on failure.
 */
LOTUS_API ubyte lotus_append_file(const char* data, const char* path, ubyte newline);

/**
 * @brief Writes data to a file, optionally preserving existing content.
 * 
 * @param data The data to write to the file.
 * @param path The path to the file.
 * @param preserve If 1, appends data to the file; if 0, overwrites the file.
 * @return 1 on success, 0 on failure.
 */
LOTUS_API ubyte lotus_write_file(const char* data, const char* path, ubyte preserve);

/**
 * @brief Processes each line of a file using a user-defined callback function.
 * 
 * @param processLine A callback function that processes each line of the file. 
 *                    The function should accept a single argument: the line as a null-terminated string.
 * @param path The path to the file.
 * @return 1 on success, 0 on failure.
 */
LOTUS_API ubyte lotus_process_file(void (*processLine)(const char* line), const char* path);
