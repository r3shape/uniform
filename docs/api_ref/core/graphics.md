# **Lotus Graphics API Reference 🪷**

## **Introduction**  
The **Lotus Graphics API** provides the functionality for rendering 2D and 3D graphics, using OpenGL (or any platform-specific OpenGL equivalents). It allows you to create and manage shaders, vertex buffers, textures, and more. This API is essential for rendering objects, managing graphics states, and ensuring the visual output of your game or application.

The **GL_API** structure contains the lower-level OpenGL functions, which are exposed through the **Lotus Graphics API**. However, detailed documentation of OpenGL calls is not provided here—please refer to the [official OpenGL website](https://www.opengl.org/documentation/).

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)

```c
#define LOTUS_STATIC_DRAW                0x88E4
#define LOTUS_ARRAY_BUFFER               0x8892
#define LOTUS_ELEMENT_ARRAY_BUFFER       0x8893
#define LOTUS_VERTEX_SHADER              0x8B31
#define LOTUS_FRAGMENT_SHADER            0x8B30
#define LOTUS_LINK_STATUS                0x8B82
#define LOTUS_COMPILE_STATUS             0x8B81
#define LOTUS_ACTIVE_ATTRIBUTES          0x8B89
#define LOTUS_SHADING_LANGUAGE_VERSION   0x8B8C
#define LOTUS_TEXTURE0                   0x84C0
#define LOTUS_TEXTURE1                   0x84C1
#define LOTUS_TEXTURE2                   0x84C2
#define LOTUS_TEXTURE3                   0x84C3
#define LOTUS_ACTIVE_TEXTURE             0x84E0
#define LOTUS_MAX_TEXTURE_UNITS          0x84E2
#define LOTUS_LINE                       0x1B01
#define LOTUS_FRONT_AND_BACK             0x0408
```

**Description**:  
These are macros representing various OpenGL constants, including buffer types, shader types, texture units, and draw modes. They are mapped to their respective OpenGL equivalents.

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)

#### **`Lotus_Vertex_Data`** 🪷

```c
typedef struct Lotus_Vertex_Data {
    ubyte attrs;
    ubyte4 vbo;
    ubyte4 ebo;
    ubyte4 vao;
    f32* vertices;
    ubyte4 vertexCount;
    ubyte4* indices;
    ubyte4 indexCount;
} Lotus_Vertex_Data;
```

**Description**:  
This structure contains the data necessary for rendering a mesh in **Lotus**.

- **attrs**: The attributes that define how the vertex data is organized (e.g., position, color, texture coordinates).
- **vbo**: Vertex buffer object, which holds the vertex data.
- **ebo**: Element buffer object, used for indexed rendering.
- **vao**: Vertex array object, which stores the state of the vertex data.
- **vertices**: Array of vertices that define the mesh.
- **vertexCount**: The number of vertices in the mesh.
- **indices**: Array of indices for indexed drawing.
- **indexCount**: The number of indices in the mesh.

---

#### **`Lotus_Graphics_State`** 🪷

```c
typedef struct Lotus_Graphics_State {
    ubyte8 draws;
    Lotus_Shader* shader;
    Lotus_Draw_Mode mode;
    Lotus_Mat4 projection;
    Lotus_Vec4 clearColor;
} Lotus_Graphics_State;
```

**Description**:  
This structure stores the current state of the graphics system.

- **draws**: The number of draw calls made.
- **shader**: The currently active shader for rendering.
- **mode**: The current draw mode (e.g., points, lines, triangles).
- **projection**: The projection matrix used for rendering.
- **clearColor**: The color used to clear the framebuffer.

---

#### **`Lotus_Shader`** 🪷

```c
typedef struct Lotus_Shader {
    sbyte4 program;
    Lotus_Hashmap* uniforms;
} Lotus_Shader;
```

**Description**:  
Represents an OpenGL shader program.

- **program**: The OpenGL shader program ID.
- **uniforms**: A hashmap storing the uniform variables for the shader.

---

#### **`Lotus_Uniform`** 🪷

```c
typedef struct Lotus_Uniform {
    sbyte4 location;
    void* value;
    const char* name;
} Lotus_Uniform;
```

**Description**:  
Represents a uniform variable within a shader.

- **location**: The location of the uniform in the shader program.
- **value**: A pointer to the data associated with the uniform.
- **name**: The name of the uniform variable.

---

### **GL_API Structure**  
As mentioned, the **GL_API** structure contains the actual OpenGL function pointers used by **Lotus**. Since OpenGL programming knowledge is required to interface with this API, and many of these functions are low-level, they are not documented here. For detailed OpenGL documentation, refer to the [official OpenGL website](https://www.opengl.org/documentation/).

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

### **Graphics State Functions:**
- [get_state](#get_state) | [cleanup](#cleanup)

### **Vertex Data Functions:**
- [create_vertex_data](#create_vertex_data) | [destroy_vertex_data](#destroy_vertex_data)

### **Shader Functions:**
- [create_shader](#create_shader) | [destroy_shader](#destroy_shader)  
- [set_uniform](#set_uniform) | [get_uniform](#get_uniform) | [send_uniform](#send_uniform)

### **Drawing Functions:**
- [set_color](#set_color) | [set_mode](#set_mode) | [set_shader](#set_shader)
- [draw_begin](#draw_begin) | [draw_clear](#draw_clear) | [draw_data](#draw_data)

### **Graphics State Setter/Toggler Functions:**
- [wireframe_mode](#wireframe_mode)

### **GL_API Wrapper Functions** (Handled by **Lotus Graphics API**):
- OpenGL functions are available through the **GL_API** structure, and detailed documentation for those functions should be sought in OpenGL resources.

---

<a id="get_state"></a>
#### **`get_state()`** 🪷

**Description**:  
Retrieves the current graphics state, including the active shader, draw mode, and clear color.

**Parameters**:  
None.

**Return Type**:  
`Lotus_Graphics_State*` - A pointer to the current graphics state.

---

<a id="cleanup"></a>
#### **`cleanup()`** 🪷

**Description**:  
Cleans up the graphics resources.

**Parameters**:  
None.

**Return Type**:  
None.

---

<a id="create_vertex_data"></a>
#### **`create_vertex_data()`** 🪷

**Description**:  
Creates a new vertex data object for rendering a mesh.

**Parameters**:  
- `vertices` (`f32*`): An array of vertex positions.
- `vertexCount` (`ubyte4`): The number of vertices in the array.
- `indices` (`ubyte4*`): An array of indices for indexed drawing.
- `indexCount` (`ubyte4`): The number of indices.
- `attrs` (`ubyte`): Vertex attributes (e.g., position, color).

**Return Type**:  
`Lotus_Vertex_Data` - The generated vertex data.

---

<a id="destroy_vertex_data"></a>
#### **`destroy_vertex_data()`** 🪷

**Description**:  
Destroys vertex data and frees associated resources.

**Parameters**:  
- `vertexData` (`Lotus_Vertex_Data*`): The vertex data to destroy.

**Return Type**:  
None.

---

<a id="create_shader"></a>
#### **`create_shader()`** 🪷

**Description**:  
Creates a shader program from vertex and fragment shaders.

**Parameters**:  
- `vertex_shader` (`const char*`): The source code for the vertex shader.
- `fragment_shader` (`const char*`): The source code for the fragment shader.

**Return Type**:  
`Lotus_Shader` - The created shader.

---

<a id="destroy_shader"></a>
#### **`destroy_shader()`** 🪷

**Description**:  
Destroys a shader program.

**Parameters**:  
- `shader` (`Lotus_Shader*`): The shader to destroy.

**Return Type**:  
None.

---

<a id="set_uniform"></a>
#### **`set_uniform()`** 🪷

**Description**:  
Sets the value of a uniform variable in a shader.

**Parameters**:  
- `shader` (`Lotus_Shader*`): The shader containing the uniform.
- `name` (`const char*`): The name of the uniform variable.
- `value` (`void*`): The value to set for the uniform.

**Return Type**:  
`ubyte` - `1` if successful, `0` if failure.

---

<a id="get_uniform"></a>
#### **`get_uniform()`** 🪷

**Description**:  
Retrieves a uniform from a shader.

**Parameters**:  
- `shader` (`Lotus_Shader*`): The shader containing the uniform.
- `name` (`const char*`): The name of the uniform.

**Return Type**:  
`Lotus_Uniform` - The uniform.

---

<a id="send_uniform"></a>
#### **`send_uniform()`** 🪷

**Description**:  
Sends a uniform value to the shader.

**Parameters**:  
- `shader` (`Lotus_Shader*`): The shader containing the uniform.
- `type` (`Lotus_Uniform_Type`): The type of the uniform (e.g., `LOTUS_UNIFORM_VEC3`).
- `name` (`const char*`): The name of the uniform.

**Return Type**:  
None.

---

<a id="set_color"></a>
#### **`set_color()`** 🪷

**Description**:  
Sets the color for rendering.

**Parameters**:  
- `color4` (`Lotus_Vec4`): The RGBA color.

**Return Type**:  
None.

---

<a id="set_mode"></a>
#### **`set_mode()`** 🪷

**Description**:  
Sets the drawing mode (e.g., points, lines, triangles).

**Parameters**:  
- `mode` (`Lotus_Draw_Mode`): The drawing mode.

**Return Type**:  
None.

---

<a id="set_shader"></a>
#### **`set_shader()`** 🪷

**Description**:  
Sets the active shader for rendering.

**Parameters**:  
- `shader` (`Lotus_Shader*`): The shader to set as active.

**Return Type**:  
None.

---

<a id="draw_begin"></a>
#### **`draw_begin()`** 🪷

**Description**:  
Begins drawing with specified mode and color.

**Parameters**:  
- `mode` (`Lotus_Draw_Mode`): The draw mode (e.g., `LOTUS_TRIANGLE_MODE`).
- `color4` (`Lotus_Vec4`): The color to use when clearing the frame.
- `projection` (`Lotus_Mat4`): The projection matrix for drawing.

**Return Type**:  
None.

---

<a id="draw_clear"></a>
#### **`draw_clear()`** 🪷

**Description**:  
Clears the current framebuffer.

**Parameters**:  
None.

**Return Type**:  
None.

---

<a id="draw_data"></a>
#### **`draw_data()`** 🪷

**Description**:  
Draws the provided vertex data.

**Parameters**:  
- `vertexData` (`Lotus_Vertex_Data`): The vertex data to render.

**Return Type**:  
None.

---

<a id="wireframe_mode"></a>
#### **`wireframe_mode()`** 🪷

**Description**:  
Toggles wireframe rendering mode.

**Parameters**:  
- `toggle` (`ubyte`): `1` to enable wireframe mode, `0` to disable.

**Return Type**:  
None.

---