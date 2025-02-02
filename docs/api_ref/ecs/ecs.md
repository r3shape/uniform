# **Lotus ECS API Reference 🪷**

Welcome to the **Lotus ECS API Reference**! 🪷  
The **Lotus Entity-Component-System (ECS)** module provides a robust architecture for managing game entities and their behaviors in a scalable, performance-oriented manner. This module enables you to separate your game logic into discrete, reusable components, making it easier to manage complex interactions.

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)

```c
#define LOTUS_ENTITY_MAX        ((1U << 16) - 1)
```
> This macro defines the maximum number of entities that the ECS system can handle. It uses bit-shifting to set the upper bound to 65535 (i.e., `2^16 - 1`), allowing for a large number of entities.

```c
#define LOTUS_COMPONENT_MAX     (1U << 5)
```
>This macro sets the maximum number of components that can be registered in the ECS system. Here, the limit is 32 components (i.e., `2^5`).

```c
#define LOTUS_PREFAB_MAX        (1U << 6)
```
> This macro defines the maximum number of prefabs that the ECS system can store. It’s set to 64 prefabs (`2^6`).

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)

#### **`Lotus_Entity`** 🪷

```c
typedef struct Lotus_Entity {
    ubyte2 id;      // The unique identifier for this entity.
    ubyte4 mask;    // A bitmask representing the components attached to this entity.
} Lotus_Entity;
```

**Description**:  
- **id**: A unique identifier for each entity. This ID is used throughout the system to reference the entity in various operations.
- **mask**: A bitmask that holds flags representing the components attached to the entity. Each bit corresponds to a different component.

---

#### **`Lotus_Entity_Manager`** 🪷

```c
typedef struct Lotus_Entity_Manager {
    ubyte2 entity_count;       // The total number of entities currently managed.
    ubyte2 free_count;         // The number of free (unused) entities.
    ubyte2 free_pool[LOTUS_ENTITY_MAX]; // The pool of available entity IDs.
    Lotus_Entity entities[LOTUS_ENTITY_MAX]; // The array of all entities in the ECS system.
} Lotus_Entity_Manager;
```

**Description**:  
This structure manages all entities in the system, keeping track of the number of active entities, the free entities available for reuse, and the array of entities themselves.

---

#### **`Lotus_Component`** 🪷

```c
typedef struct Lotus_Component {
    ubyte4 stride;  // The size (in bytes) of each individual component instance.
    ubyte2 count;   // The number of components of this type.
    void* data;     // A pointer to the data associated with the component instances.
} Lotus_Component;
```

**Description**:  
This structure represents a component in the ECS system. The `stride` is the size of a single component instance, while the `data` pointer is used to store the actual component data for all instances.

---

#### **`Lotus_Component_Manager`** 🪷

```c
typedef struct Lotus_Component_Manager {
    ubyte2 count; // The number of components currently registered.
    Lotus_Component components[LOTUS_COMPONENT_MAX];  // The array of registered components.
    Lotus_Component_System systems[LOTUS_COMPONENT_MAX];  // The systems associated with each component type.
} Lotus_Component_Manager;
```

**Description**:  
This structure manages all registered components. It stores the components and any associated component systems that define the behavior or logic applied to the components.

---

#### **`Lotus_Prefab`** 🪷

```c
typedef struct Lotus_Prefab {
    ubyte2 count;  // The number of components associated with this prefab.
    ubyte2 extends;  // A flag indicating if the prefab can be extended.
    ubyte ids[LOTUS_PREFAB_MAX]; // An array of component IDs that make up the prefab.
} Lotus_Prefab;
```

**Description**:  
This structure represents a prefab, which is a template or blueprint entity that consists of a set of components. Prefabs are used to quickly create entities with predefined sets of components.

---

#### **`Lotus_Prefab_Manager`** 🪷

```c
typedef struct Lotus_Prefab_Manager {
    ubyte2 count;      // The number of prefabs currently stored.
    Lotus_Prefab prefabs[LOTUS_PREFAB_MAX]; // The array of prefabs.
} Lotus_Prefab_Manager;
```

**Description**:  
This structure manages all prefabs in the ECS system. It keeps track of the number of prefabs and stores the array of prefabs.

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

### **Entity Functions:**
- [create_entity](#create_entity) | [destroy_entity](#destroy_entity)

### **Component Functions:**
- [register_component](#register_component)  | [unregister_component](#unregister_component)  
- [add_component](#add_component) | [get_component](#get_component) | [rem_component](#rem_component) | [has_component](#has_component)  

### **System Functions:**
- [register_system](#register_system) | [unregister_system](#unregister_system) | [run_system](#run_system)  

### **Prefab Functions:**
- [create_prefab](#create_prefab) | [create_prefab_ex](#create_prefab_ex) | [destroy_prefab](#destroy_prefab) | [instance_prefab](#instance_prefab)  

---

<a id="create_entity"></a>
#### **`create_entity()`** 🪷

**Description**:  
Creates a new entity in the ECS system and returns a unique entity ID.

**Parameters**:  
None.

**Return Type**:  
`ubyte2` - The ID of the newly created entity.

**Detailed Explanation**:  
This function initializes a new entity and returns its ID. Entities are the basic building blocks of the ECS architecture and can have components attached to them later. Use this function to dynamically add new entities to your game world.

---
<a id="destroy_entity"></a>
#### **`destroy_entity()`** 🪷

**Description**:  
Destroys an existing entity, freeing its resources.

**Parameters**:  
- `entity_id` (`ubyte2`): The ID of the entity to destroy.

**Return Type**:  
None.

**Detailed Explanation**:  
This function removes the specified entity from the ECS system and frees any resources associated with it. Once destroyed, the entity is no longer valid, and any components attached to it are also cleaned up.

---
<a id="register_component"></a>
#### **`register_component()`** 🪷

**Description**:  
Registers a new component type in the ECS system.

**Parameters**:  
- `stride` (`ubyte4`): The size (in bytes) of a single component instance.
- `component_id` (`ubyte`): The unique ID assigned to the component.

**Return Type**:  
None.

**Detailed Explanation**:  
This function registers a new component type, which can then be used by entities in the ECS system. The `stride` parameter specifies the size of the component's data, which is used for memory allocation. Components are essential for adding data to entities.

---

<a id="unregister_component"></a>
#### **`unregister_component()`** 🪷

**Description**:  
Unregisters a component type from the ECS system, making it unavailable for future use.

**Parameters**:  
- `component_id` (`ubyte`): The unique ID assigned to the component to unregister.

**Return Type**:  
None.

**Detailed Explanation**:  
This function removes a component type from the ECS system, freeing its associated resources. Once unregistered, components of that type cannot be added to entities. This is useful when you want to clean up component types that are no longer needed.

---
<a id="add_component"></a>
#### **`add_component()`** 🪷

**Description**:  
Adds a component of a specific type to an entity.

**Parameters**:  
- `entity_id` (`ubyte2`): The ID of the entity to which the component will be added.
- `component_id` (`ubyte`): The ID of the component to add.

**Return Type**:  
None.

**Detailed Explanation**:  
This function attaches a component of the specified type to an entity. The entity can then utilize the data and behavior associated with the component. Components are added to entities during runtime as the game world evolves.

---
<a id="get_component"></a>
#### **`get_component()`** 🪷

**Description**:  
Retrieves a component of a specific type from an entity.

**Parameters**:  
- `entity_id` (`ubyte2`): The ID of the entity from which to retrieve the component.
- `component_id` (`ubyte`): The ID of the component to retrieve.

**Return Type**:  
`void*` - A pointer to the requested component of the entity.

**Detailed Explanation**:  
This function retrieves the specified component from the entity. The returned pointer can be used to modify or access the component’s data. If the entity does not have the requested component, the behavior is undefined, so it’s important to use the `has_component()` function before calling this.

---
<a id="rem_component"></a>
#### **`rem_component()`** 🪷

**Description**:  
Removes a component of a specific type from an entity.

**Parameters**:  
- `entity_id` (`ubyte2`): The ID of the entity from which to remove the component.
- `component_id` (`ubyte`): The ID of the component to remove.

**Return Type**:  
None.

**Detailed Explanation**:  
This function removes the specified component from the entity. After removal, the entity no longer holds that component, and its data is no longer accessible unless re-added.

---
<a id="has_component"></a>
#### **`has_component()`** 🪷

**Description**:  
Checks if an entity has a specific component attached.

**Parameters**:  
- `entity_id` (`ubyte2`): The ID of the entity to check.
- `component_id` (`ubyte`): The ID of the component to check.

**Return Type**:  
`ubyte` - Returns `1` if the entity has the component, `0` otherwise.

**Detailed Explanation**:  
This function checks if the specified entity has the given component. It's a quick way to validate that an entity contains the necessary data before attempting to retrieve or modify it.

---
<a id="register_system"></a>
#### **`register_system()`** 🪷

**Description**:  
Registers a system to operate on entities with specific components.

**Parameters**:  
- `component_id` (`ubyte`): The component ID that the system will process.
- `system` (`void*`): A pointer to the system function or handler that processes entities with the specified component.

**Return Type**:  
None.

**Detailed Explanation**:  
This function registers a system, which operates on entities that have the specified component. The system is responsible for running logic, such as movement or rendering, on those entities during the game loop. Systems allow for efficient processing of multiple entities at once based on component data.

---
<a id="unregister_system"></a>
#### **`unregister_system()`** 🪷

**Description**:  
Unregisters a previously registered system.

**Parameters**:  
- `component_id` (`ubyte`): The component ID associated with the system to unregister.

**Return Type**:  
None.

**Detailed Explanation**:  
This function removes the system from the ECS, preventing it from processing entities with the specified component. Use this when you no longer want a system to operate on entities with a particular component.

---
<a id="run_system"></a>
#### **`run_system()`** 🪷

**Description**:  
Executes a registered system, processing entities with the relevant components.

**Parameters**:  
- `component_id` (`ubyte`): The ID of the component to run the system on.

**Return Type**:  
None.

**Detailed Explanation**:  
This function runs the specified system, which operates on all entities that have the given component. Systems are executed during each game loop, allowing for real-time updates to entity states, such as movement, rendering, and other gameplay logic.

---
<a id="create_prefab"></a>
#### **`create_prefab()`** 🪷

**Description**:  
Creates a new prefab from a list of component IDs.

**Parameters**:  
- `component_ids` (`ubyte*`): An array of component IDs to include in the prefab.
- `component_count` (`ubyte`): The number of components in the prefab.

**Return Type**:  
`ubyte2` - The ID of the newly created prefab.

**Detailed Explanation**:  
This function creates a prefab, which is a template entity composed of multiple components. Prefabs allow you to define common sets of components and reuse them across multiple entities, streamlining the creation process.

---
<a id="create_prefab_ex"></a>
#### **`create_prefab_ex()`** 🪷

**Description**:  
Creates a new prefab with the option to specify a parent entity.

**Parameters**:  
- `component_ids` (`ubyte*`): An array of component IDs to include in the prefab.
- `component_count` (`ubyte`): The number of components in the prefab.
- `parent_id` (`ubyte2`): The ID of the parent entity to associate with the prefab.

**Return Type**:  
`ubyte2` - The ID of the newly created prefab.

**Detailed Explanation**:  
This function works like `create_prefab()`, but it also allows you to associate the prefab with a parent entity. This can be useful for creating hierarchical relationships between entities and prefabs.

---
<a id="destroy_prefab"></a>
#### **`destroy_prefab()`** 🪷

**Description**:  
Destroys a previously created prefab.

**Parameters**:  
- `prefab_id` (`ubyte2`): The ID of the prefab to destroy.

**Return Type**:  
None.

**Detailed Explanation**:  
This function removes the specified prefab from the ECS system. Once destroyed, the prefab cannot be instantiated again, and any entities created from it will no longer be associated with it.

---
<a id="instance_prefab"></a>
#### **`instance_prefab()`** 🪷

**Description**:  
Creates an instance of a prefab, generating a new entity from the prefab's components.

**Parameters**:  
- `prefab_id` (`ubyte2`): The ID of the prefab to instantiate.

**Return Type**:  
`ubyte2` - The ID of the newly created entity.

**Detailed Explanation**:  
This function creates a new entity from the given prefab, attaching all the components defined by the prefab to the new entity. The resulting entity will have the same components and data as the prefab, allowing for easy reuse and instance creation.

---