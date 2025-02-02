# **Lotus Memory API Reference 🪷**  

## **Introduction**  
The **Lotus Memory API** provides low-level memory management utilities, including aligned allocation, memory region management, and region linking. This API is crucial for efficient memory handling within **Lotus**, ensuring optimal alignment, structured memory allocation, and fast memory region traversal.  

Designed with performance in mind, the **Lotus Memory API** enables developers to manage memory efficiently through both individual allocations and structured **memory regions**. It supports **aligned memory allocation**, dynamic region-based allocation, and **region linking**, making it a powerful tool for optimizing game engine memory management.  

>## ***Note: `lotus_init_memory()` and `lotus_shutdown_memory()` MUST be called before and after all other module/layer initialization/shutdown calls.***

<br>

## **How Lotus Memory Works 🪷**  

The **Lotus Memory API** is divided into two main functionalities:  

1. **Standard Memory Management** – Functions for allocating, reallocating, and freeing memory with proper alignment.  
2. **Memory Region Management** – A structured approach to memory allocation using **Lotus Memory Regions**, enabling better control over memory pools.  

Memory **alignment** is a key focus of this API, ensuring that all allocations conform to proper memory boundaries for performance and compatibility reasons. The `ALIGN_ADDRESS` macro is used to align memory pointers, and `MEMORY_PAD_SIZE` ensures proper padding when allocating aligned memory.  

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**  

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)  

```c
#define MEMORY_PAD_SIZE(a) (sizeof(ubyte2) + ((a) - 1))
```
> This macro calculates the necessary padding for aligned memory allocations. It ensures that extra bytes are reserved for alignment adjustments when using malloc-based allocation.

```c
#define ALIGN_ADDRESS(p, a) (((p) + ((a) - 1)) & ~((a) - 1))
```
> This macro aligns a pointer `p` to the nearest address that satisfies the alignment `a`. It is a bitwise operation that ensures efficient memory access patterns.

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)  

#### **`Lotus_Memory_Region`** 🪷  

```c
typedef struct Lotus_Memory_Region {
    struct Lotus_Memory_Region *next;
    struct Lotus_Memory_Region *last;
    ubyte8 capacity;
    ubyte8 stride;
    ubyte8 count;
    ubyte8* data;
} Lotus_Memory_Region;
```

**Description**:  
A **Lotus Memory Region** represents a structured block of memory with metadata for managing its allocation and linking. This system enables efficient memory pooling and chunk-based allocation.

- **next**: Pointer to the next memory region in a linked list.  
- **last**: Pointer to the previous memory region in the list.  
- **capacity**: The total number of memory slots in the region.  
- **stride**: The size of each memory slot (defines memory chunk size).  
- **count**: The number of allocated memory slots in the region.  
- **data**: Pointer to the actual memory block used for storage.  

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)  

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

### **Memory Management Functions:**  
- [alloc](#alloc) | [realloc](#realloc) | [free](#free)  

### **Memory Region Functions:**  
- [init_region](#init_region) | [spawn_region](#spawn_region) | [get_region](#get_region) | [link_region](#link_region)  
- [unlink_region](#unlink_region) | [free_region](#free_region) | [free_all_regions](#free_all_regions)  
- [set_region_data](#set_region_data) | [get_region_data](#get_region_data)  

---

<a id="alloc"></a>
#### **`alloc()`** 🪷  

**Description**:  
Allocates memory with a specified alignment.

**Parameters**:  
- `size` (`ubyte8`): The number of bytes to allocate.  
- `align` (`ubyte8`): The required memory alignment.  

**Return Type**:  
`void*` - A pointer to the allocated memory block.  

---

<a id="realloc"></a>
#### **`realloc()`** 🪷  

**Description**:  
Reallocates memory, preserving alignment.

**Parameters**:  
- `ptr` (`void*`): The pointer to the existing memory block.  
- `size` (`ubyte8`): The new size for the memory block.  
- `align` (`ubyte8`): The required memory alignment.  

**Return Type**:  
`void*` - A pointer to the reallocated memory block.  

---

<a id="free"></a>
#### **`free()`** 🪷  

**Description**:  
Frees an allocated memory block.

**Parameters**:  
- `ptr` (`void*`): The pointer to the memory block to be freed.  

**Return Type**:  
None.  

---

<a id="init_region"></a>
#### **`init_region()`** 🪷  

**Description**:  
Initializes a memory region with a specified stride and capacity.

---

<a id="spawn_region"></a>
#### **`spawn_region()`** 🪷  

**Description**:  
Creates a new memory region dynamically.

---

<a id="get_region"></a>
#### **`get_region()`** 🪷  

**Description**:  
Retrieves a memory region relative to a starting region.

---

<a id="link_region"></a>
#### **`link_region()`** 🪷  

**Description**:  
Links a new memory region to an existing region.

---

<a id="unlink_region"></a>
#### **`unlink_region()`** 🪷  

**Description**:  
Unlinks a memory region from a linked list.

---

<a id="free_region"></a>
#### **`free_region()`** 🪷  

**Description**:  
Frees an allocated memory region.

---

<a id="free_all_regions"></a>
#### **`free_all_regions()`** 🪷  

**Description**:  
Frees all memory regions starting from a given region.

---

<a id="set_region_data"></a>
#### **`set_region_data()`** 🪷  

**Description**:  
Sets data in a memory region.

---

<a id="get_region_data"></a>
#### **`get_region_data()`** 🪷  

**Description**:  
Retrieves data from a memory region.

---