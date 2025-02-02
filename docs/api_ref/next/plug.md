# **Lotus Plug API Reference 🪷**

### **Introduction**  
The **Lotus Plug** module introduces a lightweight yet powerful plugin system for dynamically extending the **Lotus** engine. It allows developers to load, manage, and unload external plugins at runtime, providing a modular and flexible approach to engine extensions without modifying the core codebase.

With **Lotus Plug**, you can integrate new features, game logic, or tools dynamically, making the engine more adaptable for different projects. The plugin system ensures that third-party extensions remain isolated, reducing the risk of breaking existing functionality.

***If you're looking to go deeper and see your experiments/features directly with Lotus, check out the [nightshade branch](https://github.com/d34d0s/Lotus/tree/nightshade).***

---

### **How Plugins Work 🪷**  

The **Lotus Plug** system follows a simple but effective mechanism:  
1. **Plugins define an API** using the `LOTUS_PLUG_API` macro, which creates a structured function table.  
2. **Plugins export their API** using the `LOTUS_PLUG_EXPORT` macro, making the function table accessible to the engine.  
3. **Plugins implement initialization and shutdown logic** using the `LOTUS_PLUG_INIT` and `LOTUS_PLUG_SHUTDOWN` macros.  
4. **The engine loads plugins dynamically** using `load_plug`, retrieves them via `get_plug`, and removes them when no longer needed using `unload_plug`.  

This approach ensures a clean and modular extension system that does not require recompiling the engine to add new functionality.

---

### **Lotus Plug Overview 🪷**

The **Lotus Plug** module consists of key macros and functions that facilitate plugin creation and management.

#### **Plugin Management 🪷**
- **Purpose**: Provides an API for loading, retrieving, and unloading plugins dynamically.
- **Key Functions**:
  - `load_plug()`: Loads a plugin from a specified file path.
  - `get_plug()`: Retrieves a previously loaded plugin by name.
  - `unload_plug()`: Unloads a loaded plugin and removes its reference.
- **API Reference**: The **Plugin API** functions are covered in the **API Reference** section below.

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**

### **Reference Sections:** [Functions](#Functions)

Below is a list of the macros provided in the API. Click on any macro to jump directly to its documentation.

### **Configuration Macros:**
- [LOTUS_PLUG_API](#LOTUS_PLUG_API) | [LOTUS_PLUG_API_FUNC](#LOTUS_PLUG_API_FUNC)  
- [LOTUS_PLUG_INIT](#LOTUS_PLUG_INIT) | [LOTUS_PLUG_SHUTDOWN](#LOTUS_PLUG_SHUTDOWN) | [LOTUS_PLUG_EXPORT](#LOTUS_PLUG_EXPORT)

---

<a id="LOTUS_PLUG_API"></a>
#### **`LOTUS_PLUG_API()`** 🪷

**Description**:  
Defines a structured API for a plugin.

**Usage**:
```c
LOTUS_PLUG_API(MyPlugin,
    LOTUS_PLUG_API_FUNC(void, my_function, int x, int y);
    LOTUS_PLUG_API_FUNC(int, another_function, float a);
);
```

**Detailed Explanation**:  
This macro generates a structure that holds function pointers, forming the dispatch table for a plugin.

---

<a id="LOTUS_PLUG_API_FUNC"></a>
#### **`LOTUS_PLUG_API_FUNC()`** 🪷

**Description**:  
Defines a function pointer within a plugin API structure.

**Usage**:
```c
LOTUS_PLUG_API_FUNC(void, my_function, int x, int y);
```

**Detailed Explanation**:  
This macro ensures consistency when defining function pointers in a plugin API table.

---

<a id="LOTUS_PLUG_INIT"></a>
#### **`LOTUS_PLUG_INIT()`** 🪷

**Description**:  
Defines the initialization function for a plugin.

**Usage**:
```c
LOTUS_PLUG_INIT(
    printf("Plugin initialized!\n");
    return 1;
);
```

**Detailed Explanation**:  
This macro generates the `_plug_init_impl()` function, which is called when the plugin is loaded.

---

<a id="LOTUS_PLUG_SHUTDOWN"></a>
#### **`LOTUS_PLUG_SHUTDOWN()`** 🪷

**Description**:  
Defines the shutdown function for a plugin.

**Usage**:
```c
LOTUS_PLUG_SHUTDOWN(
    printf("Plugin shutting down...\n");
);
```

**Detailed Explanation**:  
This macro generates the `_plug_shutdown_impl()` function, which is called when the plugin is unloaded.

---

<a id="LOTUS_PLUG_EXPORT"></a>
#### **`LOTUS_PLUG_EXPORT()`** 🪷

**Description**:  
Exports a plugin’s API dispatch table.

**Usage**:
```c
LOTUS_PLUG_EXPORT(MyPlugin_API,
    .my_function = my_function_impl,
    .another_function = another_function_impl
);
```

**Detailed Explanation**:  
This macro defines the `_plug_export_api_impl()` function, which returns a pointer to the plugin’s function table.

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

### **Plugin Functions:**
- [load_plug](#load_plug) | [get_plug](#get_plug) | [unload_plug](#unload_plug)

---

<a id="load_plug"></a>
#### **`load_plug()`** 🪷

**Description**:  
Loads a plugin dynamically from a given file path and retrieves its API dispatch table.

**Parameters**:  
- `name` (`char*`): The unique name of the plugin.  
- `path` (`char*`): The file path to the plugin’s shared library (`.dll` on Windows, `.so` on Linux).  

**Return Type**:  
`void*` - A pointer to the plugin’s API table, as defined by `LOTUS_PLUG_API`.

**Detailed Explanation**:  
This function loads a plugin, initializes it, and retrieves its API structure. The returned dispatch table contains the plugin’s functions, allowing the engine to call them dynamically.

---

<a id="get_plug"></a>
#### **`get_plug()`** 🪷

**Description**:  
Retrieves a previously loaded plugin’s API table.

**Parameters**:  
- `name` (`char*`): The unique name of the plugin.

**Return Type**:  
`void*` - A pointer to the plugin’s API table, previously returned by `load_plug()`.

**Detailed Explanation**:  
If a plugin has already been loaded, this function retrieves its function table without needing to reload it. This is useful for accessing plugin functions at different points in execution.

---

<a id="unload_plug"></a>
#### **`unload_plug()`** 🪷

**Description**:  
Unloads a previously loaded plugin and releases its resources.

**Parameters**:  
- `name` (`char*`): The unique name of the plugin.

**Return Type**:  
`ubyte` - Returns `1` if the plugin was successfully unloaded, `0` if the plugin was not found.

**Detailed Explanation**:  
Calling this function removes a plugin from memory and frees its resources. Once unloaded, the plugin’s functions can no longer be accessed.

---