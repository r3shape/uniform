## **Lotus File I/O API Reference 🪷**

Welcome to the **Lotus File I/O API Reference**! 🪷  
The **Lotus File I/O API** provides functions for managing files, including reading, writing, copying, deleting, and appending data to files. These functions are essential for handling persistent data within the engine or game, such as configuration files, save files, or logs.

---

## **Functions 🪷**

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

- [is_file](#is_file) | [read_file](#read_file) | [delete_file](#delete_file)  
- [get_file_size](#get_file_size) | [copy_file](#copy_file) | [append_file](#append_file)  
- [write_file](#write_file) | [process_file](#process_file)  

---

<a id="is_file"></a>
#### **`is_file()`** 🪷

**Description**:  
Checks if a file exists at the given path.

**Parameters**:  
- `path` (`const char*`): The path to the file.

**Return Type**:  
`ubyte` - Returns `1` if the file exists, `0` otherwise.

---

<a id="read_file"></a>
#### **`read_file()`** 🪷

**Description**:  
Reads the contents of a file into a dynamically allocated string.

**Parameters**:  
- `path` (`const char*`): The path to the file.

**Return Type**:  
`char*` - A pointer to the file's content as a null-terminated string, or `NULL` on failure. The caller is responsible for freeing the allocated memory.

---

<a id="delete_file"></a>
#### **`delete_file()`** 🪷

**Description**:  
Deletes a file at the specified path.

**Parameters**:  
- `path` (`const char*`): The path to the file.

**Return Type**:  
`ubyte` - Returns `1` on success, `0` on failure.

---

<a id="get_file_size"></a>
#### **`get_file_size()`** 🪷

**Description**:  
Retrieves the size of a file in bytes.

**Parameters**:  
- `path` (`const char*`): The path to the file.

**Return Type**:  
`long` - The size of the file in bytes, or `-1` on failure.

---

<a id="copy_file"></a>
#### **`copy_file()`** 🪷

**Description**:  
Copies a file from the source path to the destination path.

**Parameters**:  
- `dest` (`const char*`): The destination path where the file will be copied.
- `src` (`const char*`): The source path of the file to copy.

**Return Type**:  
`ubyte` - Returns `1` on success, `0` on failure.

---

<a id="append_file"></a>
#### **`append_file()`** 🪷

**Description**:  
Appends data to the end of a file.

**Parameters**:  
- `data` (`const char*`): The data to append to the file.
- `path` (`const char*`): The path to the file.
- `newline` (`ubyte`): If `1`, appends a newline after the data; otherwise, no newline is added.

**Return Type**:  
`ubyte` - Returns `1` on success, `0` on failure.

---

<a id="write_file"></a>
#### **`write_file()`** 🪷

**Description**:  
Writes data to a file, optionally preserving existing content.

**Parameters**:  
- `data` (`const char*`): The data to write to the file.
- `path` (`const char*`): The path to the file.
- `preserve` (`ubyte`): If `1`, appends data to the file; if `0`, overwrites the file.

**Return Type**:  
`ubyte` - Returns `1` on success, `0` on failure.

---

<a id="process_file"></a>
#### **`process_file()`** 🪷

**Description**:  
Processes each line of a file using a user-defined callback function.

**Parameters**:  
- `processLine` (`void (*)(const char* line)`) : A callback function that processes each line of the file. The function should accept a single argument: the line as a null-terminated string.
- `path` (`const char*`): The path to the file.

**Return Type**:  
`ubyte` - Returns `1` on success, `0` on failure.

---