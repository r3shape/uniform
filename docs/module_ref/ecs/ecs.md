# Lotus ECS Module Reference 🪷

Welcome to the **Lotus ECS Module Reference**! 🪷  
The **Entity-Component-System (ECS)** module is a key part of **Lotus**, providing a data-driven architecture for managing game objects and their behavior. ECS allows for efficient object management by breaking down complex game objects into individual components, which can be manipulated and processed independently. This architecture enables scalability, performance, and flexibility in your game design.

## Key Concepts of ECS 🪷

In **Lotus ECS**, the following core concepts are used to build and manage entities in your game:

- **Entity**: Represents a game object, identified by a unique ID. Entities hold references to the components attached to them via a bitmask.
- **Component**: Represents data associated with an entity. Components contain the raw data but no logic (e.g., position, health, velocity).
- **System**: Defines the logic that operates on entities that have specific components. Systems are run over entities that meet the criteria for the components they require.
- **Prefab**: A predefined set of components that can be reused to create entities with the same component structure.

The **Lotus ECS** module allows you to create entities, attach components to them, register systems to operate on those components, and manage prefabs for quick instantiation of entity setups.

---

### ECS Module Overview 🪷

The **Lotus ECS Module** consists of several core components that enable the creation and management of entities, components, systems, and prefabs. Here’s a quick overview of each:

#### Entity Management 🪷
- **Purpose**: Manages entities in the game world, including their creation, destruction, and component attachment.
- **Key Structures**:
    - `Lotus_Entity`: Represents a single entity, containing an ID and a bitmask for its attached components.
    - `Lotus_Entity_Manager`: Manages a collection of entities and tracks free entities for reuse.
- **API Reference**: Functions related to entity management will be detailed in the **API Reference** section.

#### Component Management 🪷
- **Purpose**: Handles the creation, destruction, and access to components attached to entities.
- **Key Structures**:
    - `Lotus_Component`: Represents a single component attached to an entity, holding data and a stride.
    - `Lotus_Component_Manager`: Manages all components and their systems.
- **API Reference**: Functions related to component management will be detailed in the **API Reference** section.

#### System Management 🪷
- **Purpose**: Allows developers to define systems that operate on entities based on their components. Systems perform the game logic like updating positions or applying damage.
- **Key Structures**:
    - `Lotus_Component_System`: A function pointer type that defines a system's logic, which is run for each entity possessing a specific component.
    - `Lotus_Component_Manager`: Registers systems and manages their execution.
- **API Reference**: System registration and execution functions are covered in the **API Reference** section.

#### Prefab Management 🪷
- **Purpose**: Enables the creation and instantiation of prefab objects—preconfigured collections of components that can be reused across entities.
- **Key Structures**:
    - `Lotus_Prefab`: Represents a prefab, including its component IDs.
    - `Lotus_Prefab_Manager`: Manages all prefabs and provides functions to create, destroy, and instance prefabs.
- **API Reference**: Prefab-related functions are detailed in the **API Reference** section.

---

### Summary 🪷

The **Lotus ECS** module provides a powerful, flexible, and efficient way to manage entities, components, and systems within your game. By leveraging this ECS architecture, you can build scalable and high-performance experiences that are easy to maintain and extend.

---

For more detailed information on each function, check out the [**Lotus ECS API Reference**](../../api_ref/ecs/ecs.md).