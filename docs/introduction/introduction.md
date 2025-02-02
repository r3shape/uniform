# **Introduction to Lotus 🪷**

**Lotus** is a game engine that is built around **modularity**, where the core systems of the engine are separated into layers and modules. This modularity allows you to choose the exact systems you need for your project, minimizing unnecessary bloat. Whether you’re working on a 2D game, a 3D game, or an experimental project, you can pick and choose the modules that best fit your needs.

---

<br>

## **Key Features of Lotus 🪷**

- **Modular Architecture**: Each layer and module in **Lotus** is self-contained, making it easy to extend and customize the engine for your specific needs.
- **Entity-Component-System (ECS)**: The **ECS** module enables efficient, data-driven game object management and behavior systems.
- **2D and 3D Game Development**: **Lotus** provides tools for both 2D and 3D game development, including rendering, input handling, physics, and more.
- **Cross-Platform Support**: **Lotus** is designed to work across multiple platforms, with a focus on flexibility and ease of deployment.
- **Extensibility**: The **Next Module** introduces experimental and cutting-edge features, like the **Lotus Plug** module, which allows you to integrate custom functionality at the engine level at runtime.

---

<br>

## **How is Lotus Designed? 🪷**

At the heart of **Lotus** lies the **Core Module**, which provides essential systems such as **memory management**, **platform abstraction**, **event handling**, **input systems**, and **graphics rendering**. On top of the **Core Module**, additional modules like **ECS**, **2D**, **3D**, and **Next** build out the engine’s functionality.

Here's a breakdown of its primary modules:

### 1. **Lotus Core**
The **Core Module** is the foundation of **Lotus**. It manages low-level systems such as **memory** (via custom allocators), **platform abstraction** (handling OS-specific tasks), and **input/event management**. The **graphics layer** is also part of the core, but it requires window creation through platform-specific APIs before it can be fully initialized.

Key systems in the **Core Module**:
- **Memory Management**: Custom allocators for efficient memory use.
- **Platform Layer**: Manages cross-platform functionality like window creation and event handling.
- **Graphics**: Handles the rendering pipeline using OpenGL, with abstractions for 2D and 3D rendering.
- **Event & Input**: Abstracts user input and system events.

### 2. **Lotus ECS**
The **ECS** module provides a data-driven approach to managing game entities. By separating data (components) from behavior (systems), **ECS** improves performance and scalability, especially in large games with numerous entities. 

ECS benefits:
- **Performance**: Efficient memory usage with cache-friendly access patterns.
- **Scalability**: Easy to add new components and systems without modifying existing ones.

### 3. **Lotus 2D and 3D**
The **Lotus 2D** and **Lotus 3D** modules build on top of the **ECS** architecture, providing systems and tools for rendering, handling meshes, transformations, cameras, and physics in both 2D and 3D environments.

- **2D Module**: Provides tools for 2D game development, including sprite rendering, 2D physics, and input handling.
- **3D Module**: Expands on **Lotus 2D** with 3D rendering support, camera manipulation, and 3D transformations.

### 4. **Lotus Next**
The **Next Module** is a parent module containing cutting-edge features, such as **Lotus Plug**. These modules enable deep integration into the engine itself, allowing you to extend **Lotus** with custom plugins for game-specific functionality.

- **Lotus Plug**: The **Lotus Plug** module allows users to define engine-level features via plugins.

---

<br>

## **How Does Initialization Work? 🪷**

When you initialize **Lotus**, you start by calling the appropriate `lotus_init_2D()` or `lotus_init_3D()` function, which automatically initializes the necessary engine layers in the correct order. Specifically, these functions handle:
- Initializing core systems like **memory management**, **platform handling**, **input**, and **event management**.
- Once the core systems are set up, you can safely begin interacting with the **graphics** system to create windows and set up the rendering pipeline.

The order of initialization matters because the **graphics layer** depends on a window and OpenGL context being created through platform-specific APIs, which is why these are handled in sequence.

---

## **Getting Started 🪷**
>You can visit the **[Getting Started](../getting_started/getting_started.md)** section or proceed with the following instructions.
- **Start with Examples**: To get up and running with **Lotus**, check out the **[Examples](../examples/examples.md)**. These small, self-contained examples help you understand basic engine usage.
- **Dive Deeper**: After understanding the basics, explore the **[Module Reference](../module_ref/module_ref.md)** and **[API Reference](../api_ref/api_ref.md)** for more advanced functionality and a deeper dive into the engine's capabilities.
- **Expand with Plugins**: If you’re interested in adding new features to the engine, check out the **[Lotus Next Module Reference](../module_ref/next/next.md)** and the **[Lotus Plug API](../api_ref/next/next.md)** for more information on using and creating plugins.

---

<br>

## Lotus is still in early development, so expect things to evolve as new features are added. This documentation is a foundation for ongoing work, and I encourage you to explore and contribute as the engine matures.