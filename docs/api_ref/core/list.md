## **Lotus Linked List API Reference 🪷**

Welcome to the **Lotus Linked List API Reference**! 🪷  
The **Lotus Linked List API** provides functions for managing a doubly linked list of data nodes. It allows you to create, remove, and query nodes, as well as retrieve and set metadata about the list (such as its length and stride).

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)

```c
#define LOTUS_LIST_HEADER_SIZE  sizeof(Lotus_List_Header)
#define LOTUS_LIST_GET_HEADER(list) ((Lotus_List_Header*)list)
#define LOTUS_LIST_GET_HEADER_FIELD(list, field) LOTUS_LIST_GET_HEADER(list)[field]
#define LOTUS_LIST_SET_HEADER_FIELD(list, field, value) LOTUS_LIST_GET_HEADER(list)[field] = value
```

**Description**:  
These macros are used to manipulate and retrieve the header information of a linked list. The header contains metadata like size, stride, and the length of the list.

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)

<a id="Lotus_List_Header"></a>
#### **`Lotus_List_Header`** 🪷
```c
typedef struct Lotus_List_Header {
    ubyte4 size;
    ubyte4 stride;
    ubyte4 length;
} Lotus_List_Header;
```

**Description**:  
This structure holds metadata for a linked list, including its size, stride (the size of each element), and length (the number of nodes in the list).

- **size**: The total size of the list, including its header.
- **stride**: The size of each node in the list (in bytes).
- **length**: The number of nodes in the list.

---

<a id="Lotus_List_Node"></a>
#### **`Lotus_List_Node`** 🪷
```c
typedef struct Lotus_List_Node {
    struct Lotus_List_Node* prev;
    struct Lotus_List_Node* next;
    void* data;
} Lotus_List_Node;
```

**Description**:  
Represents a node in the linked list. Each node contains a pointer to its previous and next nodes, as well as a pointer to the data it stores.

- **prev**: A pointer to the previous node in the list.
- **next**: A pointer to the next node in the list.
- **data**: A pointer to the data stored in the node.

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

- [create_list](#create_list) | [destroy_list](#destroy_list)  
- [get_list_header](#get_list_header) | [get_list_header_field](#get_list_header_field)  
- [set_list_header_field](#set_list_header_field) | [create_list_node](#create_list_node)  
- [remove_list_node](#remove_list_node) | [query_list_node](#query_list_node)  

---

<a id="create_list"></a>
#### **`create_list()`** 🪷

**Description**:  
Creates a new linked list with a specified stride (size of each node).

**Parameters**:  
- `dataStride` (`ubyte4`): The size of each node in the list.

**Return Type**:  
`Lotus_List_Node*` - A pointer to the start of the newly created list.

---

<a id="destroy_list"></a>
#### **`destroy_list()`** 🪷

**Description**:  
Destroys the linked list and frees its associated memory.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.

**Return Type**:  
None.

---

<a id="get_list_header"></a>
#### **`get_list_header()`** 🪷

**Description**:  
Retrieves the header of the linked list, which contains metadata about the list such as size, stride, and length.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.

**Return Type**:  
`Lotus_List_Header` - The list's header structure.

---

<a id="get_list_header_field"></a>
#### **`get_list_header_field()`** 🪷

**Description**:  
Retrieves a specific field from the linked list's header.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.
- `field` (`Lotus_List_Header_Field`): The field of the header to retrieve (e.g., size, stride, or length).

**Return Type**:  
`ubyte4` - The value of the requested field in the header.

---

<a id="set_list_header_field"></a>
#### **`set_list_header_field()`** 🪷

**Description**:  
Sets a specific field in the linked list's header.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.
- `field` (`Lotus_List_Header_Field`): The field of the header to modify.
- `value` (`ubyte4`): The value to set for the field.

**Return Type**:  
`ubyte` - `1` if successful, `0` if the operation failed.

---

<a id="create_list_node"></a>
#### **`create_list_node()`** 🪷

**Description**:  
Creates a new node and adds it to the linked list.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.

**Return Type**:  
`ubyte` - `1` if successful, `0` if the operation failed.

---

<a id="remove_list_node"></a>
#### **`remove_list_node()`** 🪷

**Description**:  
Removes a node from the linked list.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.

**Return Type**:  
`ubyte` - `1` if successful, `0` if the operation failed.

---

<a id="query_list_node"></a>
#### **`query_list_node()`** 🪷

**Description**:  
Queries a node at a specified index in the linked list.

**Parameters**:  
- `startNode` (`Lotus_List_Node*`): A pointer to the first node in the list.
- `index` (`ubyte4`): The index of the node to query.

**Return Type**:  
`Lotus_List_Node*` - A pointer to the queried node.

---