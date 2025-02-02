# Lotus Core Module 🪷

**Lotus Core** is the foundational module of the engine, providing essential features such as memory management, platform abstraction, and utility functions. It is designed to initialize and manage the underlying layers, which include the **Memory Layer**, **Platform Layer** (with its **Input** and **Event** sublayers), **Utility Layer**, and **Graphics Layer** (which also contains the **Camera Submodule**). These layers work together to support the engine's core capabilities.

## Key Functions 🪷
Before accessing any other part of the engine, these core functions must be called:
- **`lotus_init_core()`**: Initializes the core system, setting up the necessary layers (Memory, Platform, etc.).
- **`lotus_shutdown_core()`**: Shuts down the core system and all of its layers, except for the **Graphics Layer**.

### Core Module Initialization and Shutdown 🪷
**Lotus Core** is responsible for initializing all layers except the **Graphics Layer** and shutting down all layers except **Graphics**. It is the responsibility of the developer to call `lotus_init_core()` before any other layer or module is used and `lotus_shutdown_core()` after all other layers or modules have been shut down.

Note:
- **Platform Layer**: Handles window creation and event handling, which should occur before the **Graphics Layer** can be initialized.
- **Graphics Layer**: While the **Platform Layer** handles window creation and OpenGL context initialization.

---

### Layers🪷

**Lotus Core** consists of several layers, each responsible for distinct parts of the engine's functionality. Below is a brief overview of each:

#### Memory Layer 🌺
- **Purpose**: Manages dynamic memory allocation, deallocation, and alignment for the engine.
- **Key Responsibilities**:
    - Provides custom implementations for `malloc` and `free` to ensure memory is allocated with the appropriate alignment.
    - Implements utilities for managing memory blocks efficiently.
<br>

#### Platform Layer 🪷
- **Purpose**: Provides platform-specific functionality such as input handling, event management, and window management.
- **Key Responsibilities**:
    - **Input Submodule**: Handles keyboard, mouse, and gamepad input.
    - **Event Submodule**: Manages events like window resizing, keypresses, etc.
    - **Platform Abstraction**: Manages OS-specific tasks like file handling and basic OS-level interactions.
<br>

#### Utility Layer 🪷
- **Purpose**: Contains utility functions and data structures that assist with common programming tasks.
- **Key Responsibilities**:
    - Includes utility libraries for math (e.g., vectors, matrices), dynamic arrays, hash maps, and linked lists.
    - Provides essential tools for game developers to handle common tasks like math operations, array resizing, and file I/O.
<br>

#### Graphics Layer 🪷
- **Purpose**: Interfaces with OpenGL to provide rendering capabilities.
- **Key Responsibilities**:
    - Manages the OpenGL context, shaders, and rendering state.
    - Implements functions for rendering 2D and 3D objects.
    - **Camera Submodule**: Provides a generic camera system that supports both **2D** and **3D** configurations.
    - The **Graphics Layer** cannot function without a valid OpenGL context created by the **Platform Layer**.
<br>

---

### Summary 🪷
**Lotus Core** is the backbone of Lotus, providing essential layers that all other layers depend on. Understanding how to initialize and shut down these layers, as well as how to configure the module is crucial for an effective hacking session.

---

For more detailed information on each layer, and their functions and components, check out the [Lotus Core API Reference](../../api_ref/core/core.md)