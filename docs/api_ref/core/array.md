## **Lotus Array API Reference 🪷**

Welcome to the **Lotus Array API Reference**! 🪷  
The **Lotus Array API** provides functions for managing and manipulating dynamic arrays within the **Lotus** engine. These arrays can be resized, have elements pushed or popped, and have additional operations like insertion and printing.

Each array is managed with a custom structure that holds metadata like size, capacity, and stride to optimize memory usage and performance.

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)

```c
#define LOTUS_ARRAY_HEADER_SIZE (4 * sizeof(ubyte4))
#define LOTUS_ARRAY_GET_HEADER(array) ((ubyte4*)((ubyte*)array - LOTUS_ARRAY_HEADER_SIZE))
#define LOTUS_ARRAY_GET_HEADER_FIELD(array, field) LOTUS_ARRAY_GET_HEADER(array)[field]
#define LOTUS_ARRAY_SET_HEADER_FIELD(array, field, value) LOTUS_ARRAY_GET_HEADER(array)[field] = value
```

**Description**:  
These macros manipulate the header of the array. The header contains metadata for array operations, including size, stride, length, and capacity.

- **LOTUS_ARRAY_HEADER_SIZE**: The size of the header (in bytes).
- **LOTUS_ARRAY_GET_HEADER**: Retrieves the array's header.
- **LOTUS_ARRAY_GET_HEADER_FIELD**: Retrieves a specific field in the array's header.
- **LOTUS_ARRAY_SET_HEADER_FIELD**: Sets a specific field in the array's header.

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)

---

<a id="Lotus_Array_Field"></a>
#### **`Lotus_Array_Field`** 🪷
```c
typedef enum Lotus_Array_Field {
    LOTUS_ARRAY_SIZE_FIELD = 0,
    LOTUS_ARRAY_STRIDE_FIELD,
    LOTUS_ARRAY_LENGTH_FIELD,
    LOTUS_ARRAY_CAPACITY_FIELD,
    LOTUS_ARRAY_FIELD_COUNT
} Lotus_Array_Field;
```

**Description**:  
Defines the various fields in the array's header used for storing array metadata. 

- **LOTUS_ARRAY_SIZE_FIELD**: The total size of the array (including header).
- **LOTUS_ARRAY_STRIDE_FIELD**: The size of a single element in the array.
- **LOTUS_ARRAY_LENGTH_FIELD**: The number of elements currently in the array.
- **LOTUS_ARRAY_CAPACITY_FIELD**: The total number of elements the array can hold before resizing.

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

- [create_array](#create_array) | [destroy_array](#destroy_array)  
- [print_array](#print_array) | [pop_array](#pop_array)  
- [push_array](#push_array) | [resize_array](#resize_array)  
- [insert_array](#insert_array)  

---

<a id="create_array"></a>
#### **`create_array()`** 🪷

**Description**:  
Creates a new dynamic array with a specified stride and capacity.

**Parameters**:  
- `stride` (`ubyte4`): The size of each element in the array.
- `capacity` (`ubyte4`): The maximum number of elements the array can hold.

**Return Type**:  
`void*` - A pointer to the newly created array.

---

<a id="destroy_array"></a>
#### **`destroy_array()`** 🪷

**Description**:  
Destroys a dynamic array and frees its associated memory.

**Parameters**:  
- `array` (`void*`): A pointer to the array to destroy.

**Return Type**:  
None.

---

<a id="print_array"></a>
#### **`print_array()`** 🪷

**Description**:  
Prints the contents of a dynamic array to the console for debugging purposes.

**Parameters**:  
- `tag` (`char*`): A label to prefix the printed output.
- `array` (`void*`): A pointer to the array to print.

**Return Type**:  
None.

---

<a id="pop_array"></a>
#### **`pop_array()`** 🪷

**Description**:  
Pops the last element from the array and stores it in a specified memory location.

**Parameters**:  
- `array` (`void*`): A pointer to the array.
- `out_value` (`void*`): A pointer to store the popped value.

**Return Type**:  
None.

---

<a id="push_array"></a>
#### **`push_array()`** 🪷

**Description**:  
Pushes a new element to the end of the array.

**Parameters**:  
- `array` (`void*`): A pointer to the array.
- `in_value` (`void*`): A pointer to the value to push onto the array.

**Return Type**:  
`void*` - A pointer to the array after the element is pushed.

---

<a id="resize_array"></a>
#### **`resize_array()`** 🪷

**Description**:  
Resizes a dynamic array to a new capacity.

**Parameters**:  
- `array` (`void*`): A pointer to the array to resize.
- `new_capacity` (`ubyte4`): The new capacity of the array.

**Return Type**:  
`void*` - A pointer to the resized array.

---

<a id="insert_array"></a>
#### **`insert_array()`** 🪷

**Description**:  
Inserts a new element at a specified index in the array.

**Parameters**:  
- `array` (`void*`): A pointer to the array.
- `index` (`ubyte4`): The index at which to insert the element.
- `in_value` (`void*`): A pointer to the value to insert.

**Return Type**:  
`void*` - A pointer to the array after the element is inserted.

---