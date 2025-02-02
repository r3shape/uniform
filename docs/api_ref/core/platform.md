# **Lotus Platform API Reference 🪷**  

## **Introduction**  
The **Lotus Platform API** abstracts platform-specific functionality, including **window management**, **event polling**, **dynamic library loading**, **timekeeping**, and **OpenGL context creation**. It provides a unified interface for interacting with different operating systems while keeping the **Lotus Engine** platform-agnostic.  

This API enables **window creation**, **input polling**, **cursor management**, and **OpenGL rendering**, making it the backbone of **Lotus**’ cross-platform support.

>## ***Note: `lotus_init_platform()` MUST be called after `lotus_init_memory()`, before all other module/layer initialization calls, and `lotus_shutdown_platform()` MUST be called after all other module/layer shutdown calls.***

<br>

## **Core Features 🪷**  

The **Lotus Platform API** is responsible for:  

1. **Platform State Management** – Tracks the active platform, input state, and timing data.  
2. **Window Management** – Handles creation, destruction, and manipulation of engine windows.  
3. **Event & Input Polling** – Retrieves system and user events (like key presses, mouse movement, and window events).  
4. **Cursor Control** – Allows applications to show, hide, bind, or center the cursor.  
5. **Dynamic Library Loading** – Loads external shared libraries (`.dll`, `.so`, `.dylib`) and retrieves symbols.  
6. **Timing & Performance** – Provides time-related functions for measuring frame times and implementing delays.  
7. **OpenGL Context Management** – Supports OpenGL context creation, retrieval, and buffer swapping.  

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**  

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)  


---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)  

#### **`Lotus_Platform_Tag`** 🪷  

```c
typedef enum Lotus_Platform_Tag {
    LOTUS_WINDOWS_TAG,
    LOTUS_MACOS_TAG,
    LOTUS_LINUX_TAG,
    LOTUS_PLATFORM_TAGS
} Lotus_Platform_Tag;
```

**Description**:  
Represents the operating system running the **Lotus Engine**.  

- **LOTUS_WINDOWS_TAG** – Windows platform.  
- **LOTUS_MACOS_TAG** – macOS platform.  
- **LOTUS_LINUX_TAG** – Linux platform.  

---

#### **`Lotus_DyLib`** 🪷  

```c
typedef struct Lotus_DyLib {
    char *name;
    void *handle;
} Lotus_DyLib;
```

**Description**:  
Represents a **dynamically loaded library**. Used for loading shared libraries and retrieving function symbols.  

- **name** – Name of the loaded library.  
- **handle** – Pointer to the library handle.  

---

#### **`Lotus_Window_Flag`** 🪷  

```c
typedef enum Lotus_Window_Flag {
    LOTUS_SHOW_CURSOR =     (1 << 0),
    LOTUS_BIND_CURSOR =     (1 << 1),
    LOTUS_CENTER_CURSOR =   (1 << 2)
} Lotus_Window_Flag;
```

**Description**:  
Flags used to control window behaviors, particularly cursor states.  

- **LOTUS_SHOW_CURSOR** – Show the cursor in the window.  
- **LOTUS_BIND_CURSOR** – Lock the cursor to the window.  
- **LOTUS_CENTER_CURSOR** – Keep the cursor centered in the window.  

---

#### **`Lotus_Window`** 🪷  

```c
typedef struct Lotus_Window {
    ubyte focused;
    ubyte2 flags;
    char title[256];
    int location[2];
    int size[2];
    float aspect_ratio;
    void *internal_data;
} Lotus_Window;
```

**Description**:  
Represents an **application window** managed by the engine.  

- **focused** – Whether the window is currently active.  
- **flags** – Stores window flags (e.g., cursor settings).  
- **title** – The window title.  
- **location** – The `(x, y)` position of the window.  
- **size** – The `(width, height)` dimensions of the window.  
- **aspect_ratio** – The width-to-height ratio of the window.  
- **internal_data** – Pointer to platform-specific window data.  

---

#### **`Lotus_Platform_State`** 🪷  

```c
typedef struct Lotus_Platform_State {
    Lotus_Window *window;
    f64 clock_frequency;
    Lotus_Platform_Tag platform;
    Lotus_Input_State *input_state;
} Lotus_Platform_State;
```

**Description**:  
Tracks the current platform state, including the active window, timing data, and input system.  

- **window** – Pointer to the active window.  
- **clock_frequency** – The system clock frequency (used for timing calculations).  
- **platform** – The active platform (Windows, macOS, or Linux).  
- **input_state** – Pointer to the input system state.  

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**  

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)  

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.  

### **Platform Management Functions:**  
- [get_state](#get_state) | [cleanup](#cleanup) | [get_time](#get_time) | [sleep](#sleep)  

### **Dynamic Library Functions:**  
- [load_library](#load_library) | [get_library_symbol](#get_library_symbol) | [unload_library](#unload_library)  

### **Window Functions:**  
- [create_window](#create_window) | [destroy_window](#destroy_window)  
- [get_window_flag](#get_window_flag) | [set_window_flag](#set_window_flag) | [rem_window_flag](#rem_window_flag)  

### **Event & Input Functions:**  
- [poll_events](#poll_events) | [poll_inputs](#poll_inputs)  

### **Cursor Functions:**  
- [show_cursor](#show_cursor) | [hide_cursor](#hide_cursor)  
- [bind_cursor](#bind_cursor) | [unbind_cursor](#unbind_cursor)  
- [center_cursor](#center_cursor) | [decenter_cursor](#decenter_cursor)  

### **OpenGL Context Functions:**  
- [create_gl_context](#create_gl_context) | [get_gl_context](#get_gl_context)  
- [swap_buffers](#swap_buffers) | [destroy_gl_context](#destroy_gl_context)  

---

<a id="get_state"></a>
#### **`get_state()`** 🪷  

**Description**:  
Retrieves the current platform state.  

**Return Type**:  
`Lotus_Platform_State*` - A pointer to the platform state.  

---

<a id="cleanup"></a>
#### **`cleanup()`** 🪷  

**Description**:  
Performs platform cleanup operations before shutdown.  

**Return Type**:  
None.  

---

<a id="get_time"></a>
#### **`get_time()`** 🪷  

**Description**:  
Retrieves the current system time in seconds.  

**Return Type**:  
`f64` - The current time in seconds.  

---

<a id="sleep"></a>
#### **`sleep()`** 🪷  

**Description**:  
Pauses execution for a given number of seconds.  

**Parameters**:  
- `seconds` (`f64`): The duration to sleep.  

**Return Type**:  
None.  

---

<a id="load_library"></a>
#### **`load_library()`** 🪷  

**Description**:  
Loads a dynamic library (`.dll`, `.so`, `.dylib`).  

**Parameters**:  
- `path` (`const char*`): The path to the library file.  
- `name` (`char*`): The name of the library.  

**Return Type**:  
`Lotus_DyLib` - The loaded library structure.  

---

<a id="get_library_symbol"></a>
#### **`get_library_symbol()`** 🪷  

**Description**:  
Retrieves a function pointer from a loaded library.  

**Parameters**:  
- `library` (`Lotus_DyLib*`): Pointer to the loaded library.  
- `symbol_name` (`const char*`): The name of the function symbol.  

**Return Type**:  
`void*` - Pointer to the function.  

---

<a id="unload_library"></a>
#### **`unload_library()`** 🪷  

**Description**:  
Unloads a previously loaded dynamic library.  

**Parameters**:  
- `library` (`Lotus_DyLib*`): Pointer to the library to unload.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="create_window"></a>
#### **`create_window()`** 🪷  

**Description**:  
Creates a new window.  

**Parameters**:  
- `title` (`const char*`): The window title.  
- `width` (`int`): The width of the window.  
- `height` (`int`): The height of the window.  

**Return Type**:  
`Lotus_Window*` - Pointer to the created window.  

---

<a id="destroy_window"></a>
#### **`destroy_window()`** 🪷  

**Description**:  
Destroys a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): Pointer to the window to destroy.  

**Return Type**:  
None.  

---

<a id="get_window_flag"></a>
#### **`get_window_flag()`** 🪷  

**Description**:  
Checks if a flag is set for a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window to check.  
- `flag` (`ubyte2`): The flag to check.  

**Return Type**:  
`ubyte` - `1` if the flag is set, `0` otherwise.  

---

<a id="set_window_flag"></a>
#### **`set_window_flag()`** 🪷  

**Description**:  
Sets a flag on a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window to modify.  
- `flag` (`ubyte2`): The flag to set.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="rem_window_flag"></a>
#### **`rem_window_flag()`** 🪷  

**Description**:  
Removes a flag from a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window to modify.  
- `flag` (`ubyte2`): The flag to remove.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="poll_events"></a>
#### **`poll_events()`** 🪷  

**Description**:  
Polls system events (e.g., window events, user input).  

**Return Type**:  
`ubyte` - `1` if events were processed, `0` otherwise.  

---

<a id="poll_inputs"></a>
#### **`poll_inputs()`** 🪷  

**Description**:  
Polls the input system for keyboard, mouse, and controller events.  

**Return Type**:  
None.  

---

<a id="show_cursor"></a>
#### **`show_cursor()`** 🪷  

**Description**:  
Shows the cursor in the window.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="hide_cursor"></a>
#### **`hide_cursor()`** 🪷  

**Description**:  
Hides the cursor in the window.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="bind_cursor"></a>
#### **`bind_cursor()`** 🪷  

**Description**:  
Locks the cursor to the window.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="unbind_cursor"></a>
#### **`unbind_cursor()`** 🪷  

**Description**:  
Unlocks the cursor from the window.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="center_cursor"></a>
#### **`center_cursor()`** 🪷  

**Description**:  
Centers the cursor in the window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window in which to center the cursor.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="decenter_cursor"></a>
#### **`decenter_cursor()`** 🪷  

**Description**:  
Restores normal cursor movement, removing forced centering.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window in which to remove cursor centering.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="create_gl_context"></a>
#### **`create_gl_context()`** 🪷  

**Description**:  
Creates an OpenGL rendering context for a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window to create the OpenGL context for.  

**Return Type**:  
`ubyte` - `1` on success, `0` on failure.  

---

<a id="get_gl_context"></a>
#### **`get_gl_context()`** 🪷  

**Description**:  
Retrieves the OpenGL context associated with a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window to get the OpenGL context from.  

**Return Type**:  
`void*` - Pointer to the OpenGL context.  

---

<a id="swap_buffers"></a>
#### **`swap_buffers()`** 🪷  

**Description**:  
Swaps the front and back buffers for a window’s OpenGL context.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window whose buffers should be swapped.  

**Return Type**:  
None.  

---

<a id="destroy_gl_context"></a>
#### **`destroy_gl_context()`** 🪷  

**Description**:  
Destroys an OpenGL context associated with a window.  

**Parameters**:  
- `window` (`Lotus_Window*`): The window whose OpenGL context should be destroyed.  

**Return Type**:  
None.  

---