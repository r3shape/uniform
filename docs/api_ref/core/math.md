## **Lotus Math API Reference 🪷**

Welcome to the **Lotus Math API Reference**! 🪷  
The **Lotus Math API** provides a collection of mathematical functions and types for working with vectors, matrices, and other mathematical constructs commonly used in game development. These utilities are optimized for game engines and include functions for transformations, vector math, and coordinate system manipulations.

---

<br>
<br>

<a id="Macros"></a>
# **Macros 🪷**

### **Reference Sections:** [Structures](#Structures) | [Functions](#Functions)

```c
#define LOTUS_PI 3.14159265358979323846

#define LOTUS_VEC2(type, x, y) (type[2]){x, y}
#define LOTUS_VEC3(type, x, y, z) (type[3]){x, y, z}
#define LOTUS_VEC4(type, x, y, z, w) (type[4]){x, y, z, w}

#define LOTUS_MAT4(type) (type[16]){0}
#define LOTUS_IDENTITY(type) (type[16]){ \
    1.0, 0.0, 0.0, 0.0, \
    0.0, 1.0, 0.0, 0.0, \
    0.0, 0.0, 1.0, 0.0, \
    0.0, 0.0, 0.0, 1.0  \
}
```

**Description**:  
These macros are used to define and initialize vectors and matrices, as well as provide constants like **LOTUS_PI**. They help simplify the creation of mathematical constructs in the **Lotus** engine.

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**

### **Reference Sections:** [Macros](#Macros) | [Functions](#Functions)

<a id="Lotus_Vec2"></a>
#### **`Lotus_Vec2`** 🪷
```c
typedef struct Lotus_Vec2 {
    f32 x, y;
} Lotus_Vec2;
```

**Description**:  
Represents a 2D vector with `x` and `y` components.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.

---

<a id="Lotus_Vec2i"></a>
#### **`Lotus_Vec2i`** 🪷
```c
typedef struct Lotus_Vec2i {
    sbyte4 x, y;
} Lotus_Vec2i;
```

**Description**:  
Represents a 2D vector with `x` and `y` components using integer values.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.

---

<a id="Lotus_Vec2b"></a>
#### **`Lotus_Vec2b`** 🪷
```c
typedef struct Lotus_Vec2b {
    ubyte x, y;
} Lotus_Vec2b;
```

**Description**:  
Represents a 2D vector with `x` and `y` components using byte values.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.

---

<a id="Lotus_Vec3"></a>
#### **`Lotus_Vec3`** 🪷
```c
typedef struct Lotus_Vec3 {
    f32 x, y, z;
} Lotus_Vec3;
```

**Description**:  
Represents a 3D vector with `x`, `y`, and `z` components.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.
- **z**: The z-coordinate of the vector.

---

<a id="Lotus_Vec3i"></a>
#### **`Lotus_Vec3i`** 🪷
```c
typedef struct Lotus_Vec3i {
    sbyte4 x, y, z;
} Lotus_Vec3i;
```

**Description**:  
Represents a 3D vector with `x`, `y`, and `z` components using integer values.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.
- **z**: The z-coordinate of the vector.

---

<a id="Lotus_Vec3b"></a>
#### **`Lotus_Vec3b`** 🪷
```c
typedef struct Lotus_Vec3b {
    ubyte x, y, z;
} Lotus_Vec3b;
```

**Description**:  
Represents a 3D vector with `x`, `y`, and `z` components using byte values.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.
- **z**: The z-coordinate of the vector.

---

<a id="Lotus_Vec4"></a>
#### **`Lotus_Vec4`** 🪷
```c
typedef struct Lotus_Vec4 {
    f32 x, y, z, w;
} Lotus_Vec4;
```

**Description**:  
Represents a 4D vector with `x`, `y`, `z`, and `w` components.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.
- **z**: The z-coordinate of the vector.
- **w**: The w-coordinate of the vector.

---

<a id="Lotus_Vec4i"></a>
#### **`Lotus_Vec4i`** 🪷
```c
typedef struct Lotus_Vec4i {
    sbyte4 x, y, z, w;
} Lotus_Vec4i;
```

**Description**:  
Represents a 4D vector with `x`, `y`, `z`, and `w` components using integer values.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.
- **z**: The z-coordinate of the vector.
- **w**: The w-coordinate of the vector.

---

<a id="Lotus_Vec4b"></a>
#### **`Lotus_Vec4b`** 🪷
```c
typedef struct Lotus_Vec4b {
    ubyte x, y, z, w;
} Lotus_Vec4b;
```

**Description**:  
Represents a 4D vector with `x`, `y`, `z`, and `w` components using byte values.

- **x**: The x-coordinate of the vector.
- **y**: The y-coordinate of the vector.
- **z**: The z-coordinate of the vector.
- **w**: The w-coordinate of the vector.

---

<a id="Lotus_Mat4"></a>
#### **`Lotus_Mat4`** 🪷
```c
typedef struct Lotus_Mat4{
    f32 m[16];
} Lotus_Mat4;
```

**Description**:  
Represents a 4x4 matrix used for transformations in 3D space.

- **m**: A 16-element array representing the 4x4 matrix.

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Macros](#Macros) | [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

- [lotus_to_radians](#lotus_to_radians)  
- [lotus_new_vec2](#lotus_new_vec2) | [lotus_scale_vec2](#lotus_scale_vec2) | [lotus_add_vec2](#lotus_add_vec2) | [lotus_sub_vec2](#lotus_sub_vec2) | [lotus_dot_vec2](#lotus_dot_vec2) | [lotus_norm_vec2](#lotus_norm_vec2)  
- [lotus_new_vec3](#lotus_new_vec3) | [lotus_scale_vec3](#lotus_scale_vec3) | [lotus_add_vec3](#lotus_add_vec3) | [lotus_sub_vec3](#lotus_sub_vec3) | [lotus_dot_vec3](#lotus_dot_vec3) | [lotus_norm_vec3](#lotus_norm_vec3) | [lotus_cross_vec3](#lotus_cross_vec3)  
- [lotus_new_vec4](#lotus_new_vec4) | [lotus_scale_vec4](#lotus_scale_vec4) | [lotus_add_vec4](#lotus_add_vec4) | [lotus_sub_vec4](#lotus_sub_vec4) | [lotus_dot_vec4](#lotus_dot_vec4) | [lotus_norm_vec4](#lotus_norm_vec4)  
- [lotus_identity](#lotus_identity) | [lotus_mul_mat4](#lotus_mul_mat4) | [lotus_mul_mat4_vec3](#lotus_mul_mat4_vec3) | [lotus_trans_mat4](#lotus_trans_mat4) | [lotus_scale_mat4](#lotus_scale_mat4) | [lotus_look_at](#lotus_look_at)  
- [lotus_rotx_mat4](#lotus_rotx_mat4) | [lotus_roty_mat4](#lotus_roty_mat4) | [lotus_rotz_mat4](#lotus_rotz_mat4) | [lotus_rot_mat4](#lotus_rot_mat4)  
- [lotus_perspective](#lotus_perspective) | [lotus_ortho](#lotus_ortho)  

---

<a id="lotus_to_radians"></a>
#### **`lotus_to_radians()`** 🪷

**Description**:  
Converts an angle from degrees to radians.

**Parameters**:  
- `deg` (`f32`): The angle in degrees to convert.

**Return Type**:  
`f32` - The angle in radians.

---

<a id="lotus_new_vec2"></a>
#### **`lotus_new_vec2()`** 🪷

**Description**:  
Creates a new 2D vector.

**Parameters**:  
- `x` (`f32`): The x-coordinate of the vector.
- `y` (`f32`): The y-coordinate of the vector.

**Return Type**:  
`Lotus_Vec2` - The created 2D vector.

---

<a

 id="lotus_scale_vec2"></a>
#### **`lotus_scale_vec2()`** 🪷

**Description**:  
Scales a 2D vector by a given factor.

**Parameters**:  
- `v` (`Lotus_Vec2`): The vector to scale.
- `scale` (`f32`): The scaling factor.

**Return Type**:  
`Lotus_Vec2` - The scaled vector.

---

<a id="lotus_add_vec2"></a>
#### **`lotus_add_vec2()`** 🪷

**Description**:  
Adds two 2D vectors.

**Parameters**:  
- `v1` (`Lotus_Vec2`): The first vector.
- `v2` (`Lotus_Vec2`): The second vector.

**Return Type**:  
`Lotus_Vec2` - The result of adding the two vectors.

---

<a id="lotus_sub_vec2"></a>
#### **`lotus_sub_vec2()`** 🪷

**Description**:  
Subtracts one 2D vector from another.

**Parameters**:  
- `v1` (`Lotus_Vec2`): The first vector.
- `v2` (`Lotus_Vec2`): The second vector.

**Return Type**:  
`Lotus_Vec2` - The result of subtracting the vectors.

---

<a id="lotus_dot_vec2"></a>
#### **`lotus_dot_vec2()`** 🪷

**Description**:  
Calculates the dot product of two 2D vectors.

**Parameters**:  
- `v1` (`Lotus_Vec2`): The first vector.
- `v2` (`Lotus_Vec2`): The second vector.

**Return Type**:  
`f32` - The dot product.

---

<a id="lotus_norm_vec2"></a>
#### **`lotus_norm_vec2()`** 🪷

**Description**:  
Normalizes a 2D vector (scales it to unit length).

**Parameters**:  
- `v` (`Lotus_Vec2`): The vector to normalize.

**Return Type**:  
`Lotus_Vec2` - The normalized vector.

---

<a id="lotus_new_vec3"></a>
#### **`lotus_new_vec3()`** 🪷

**Description**:  
Creates a new 3D vector.

**Parameters**:  
- `x` (`f32`): The x-coordinate of the vector.
- `y` (`f32`): The y-coordinate of the vector.
- `z` (`f32`): The z-coordinate of the vector.

**Return Type**:  
`Lotus_Vec3` - The created 3D vector.

---

<a id="lotus_scale_vec3"></a>
#### **`lotus_scale_vec3()`** 🪷

**Description**:  
Scales a 3D vector by a given factor.

**Parameters**:  
- `v` (`Lotus_Vec3`): The vector to scale.
- `scale` (`f32`): The scaling factor.

**Return Type**:  
`Lotus_Vec3` - The scaled vector.

---

<a id="lotus_add_vec3"></a>
#### **`lotus_add_vec3()`** 🪷

**Description**:  
Adds two 3D vectors.

**Parameters**:  
- `v1` (`Lotus_Vec3`): The first vector.
- `v2` (`Lotus_Vec3`): The second vector.

**Return Type**:  
`Lotus_Vec3` - The result of adding the two vectors.

---

<a id="lotus_sub_vec3"></a>
#### **`lotus_sub_vec3()`** 🪷

**Description**:  
Subtracts one 3D vector from another.

**Parameters**:  
- `v1` (`Lotus_Vec3`): The first vector.
- `v2` (`Lotus_Vec3`): The second vector.

**Return Type**:  
`Lotus_Vec3` - The result of subtracting the vectors.

---

<a id="lotus_dot_vec3"></a>
#### **`lotus_dot_vec3()`** 🪷

**Description**:  
Calculates the dot product of two 3D vectors.

**Parameters**:  
- `a` (`Lotus_Vec3`): The first vector.
- `b` (`Lotus_Vec3`): The second vector.

**Return Type**:  
`f32` - The dot product.

---

<a id="lotus_norm_vec3"></a>
#### **`lotus_norm_vec3()`** 🪷

**Description**:  
Normalizes a 3D vector (scales it to unit length).

**Parameters**:  
- `v` (`Lotus_Vec3`): The vector to normalize.

**Return Type**:  
`Lotus_Vec3` - The normalized vector.

---

<a id="lotus_cross_vec3"></a>
#### **`lotus_cross_vec3()`** 🪷

**Description**:  
Calculates the cross product of two 3D vectors.

**Parameters**:  
- `v1` (`Lotus_Vec3`): The first vector.
- `v2` (`Lotus_Vec3`): The second vector.

**Return Type**:  
`Lotus_Vec3` - The cross product of the two vectors.

---

<a id="lotus_new_vec4"></a>
#### **`lotus_new_vec4()`** 🪷

**Description**:  
Creates a new 4D vector.

**Parameters**:  
- `x` (`f32`): The x-coordinate of the vector.
- `y` (`f32`): The y-coordinate of the vector.
- `z` (`f32`): The z-coordinate of the vector.
- `w` (`f32`): The w-coordinate of the vector.

**Return Type**:  
`Lotus_Vec4` - The created 4D vector.

---

<a id="lotus_scale_vec4"></a>
#### **`lotus_scale_vec4()`** 🪷

**Description**:  
Scales a 4D vector by a given factor.

**Parameters**:  
- `v` (`Lotus_Vec4`): The vector to scale.
- `scale` (`f32`): The scaling factor.

**Return Type**:  
`Lotus_Vec4` - The scaled vector.

---

<a id="lotus_add_vec4"></a>
#### **`lotus_add_vec4()`** 🪷

**Description**:  
Adds two 4D vectors.

**Parameters**:  
- `v1` (`Lotus_Vec4`): The first vector.
- `v2` (`Lotus_Vec4`): The second vector.

**Return Type**:  
`Lotus_Vec4` - The result of adding the two vectors.

---

<a id="lotus_sub_vec4"></a>
#### **`lotus_sub_vec4()`** 🪷

**Description**:  
Subtracts one 4D vector from another.

**Parameters**:  
- `v1` (`Lotus_Vec4`): The first vector.
- `v2` (`Lotus_Vec4`): The second vector.

**Return Type**:  
`Lotus_Vec4` - The result of subtracting the vectors.

---

<a id="lotus_dot_vec4"></a>
#### **`lotus_dot_vec4()`** 🪷

**Description**:  
Calculates the dot product of two 4D vectors.

**Parameters**:  
- `v1` (`Lotus_Vec4`): The first vector.
- `v2` (`Lotus_Vec4`): The second vector.

**Return Type**:  
`f32` - The dot product.

---

<a id="lotus_norm_vec4"></a>
#### **`lotus_norm_vec4()`** 🪷

**Description**:  
Normalizes a 4D vector (scales it to unit length).

**Parameters**:  
- `v` (`Lotus_Vec4`): The vector to normalize.

**Return Type**:  
`Lotus_Vec4` - The normalized vector.

---

<a id="lotus_identity"></a>
#### **`lotus_identity()`** 🪷

**Description**:  
Creates a 4x4 identity matrix.

**Parameters**:  
None.

**Return Type**:  
`Lotus_Mat4` - The identity matrix.

---

<a id="lotus_mul_mat4"></a>
#### **`lotus_mul_mat4()`** 🪷

**Description**:  
Multiplies two 4x4 matrices.

**Parameters**:  
- `a` (`Lotus_Mat4`): The first matrix.
- `b` (`Lotus_Mat4`): The second matrix.

**Return Type**:  
`Lotus_Mat4` - The result of multiplying the two matrices.

---

<a id="lotus_mul_mat4_vec3"></a>
#### **`lotus_mul_mat4_vec3()`** 🪷

**Description**:  
Multiplies a 4x4 matrix by a 3D vector.

**Parameters**:  
- `m` (`Lotus_Mat4`): The matrix to multiply.
- `v` (`Lotus_Vec3`): The vector to multiply.

**Return Type**:  
`Lotus_Vec3` - The result of multiplying the matrix and vector.

---

<a id="lotus_trans_mat4"></a>
#### **`lotus_trans_mat4()`** 🪷

**Description**:  
Creates a translation matrix for 3D transformations.

**Parameters**:  
- `x` (`f32`): The translation along the x-axis.
- `y` (`f32`): The translation along the y-axis.
- `z` (`f32`): The translation along the z-axis.

**Return Type**:  
`Lotus_Mat4` - The translation matrix.

---

<a id="lotus_scale_mat4"></a>
#### **`lotus_scale_mat4()`** 🪷

**Description**:  
Creates a scaling matrix for 3D transformations.

**Parameters**:  
- `x` (`f32`): The scaling factor along the x-axis.
- `y` (`f32`): The scaling factor along the y-axis.
- `z` (`f32`): The scaling factor along the z-axis.

**Return Type**:  
`Lotus_Mat4` - The scaling matrix.

---

<a id="lotus_look_at"></a>
#### **`lotus_look_at()`** 🪷

**Description**:  
Creates a view matrix that looks at a target position from a specific point in space.

**Parameters**:  
- `eye` (`Lotus_Vec3`): The position of the camera.
- `center` (`Lotus_Vec3`): The position the camera is looking at.
- `up` (`Lotus_Vec3`): The up direction for the camera.

**Return Type**:  
`Lotus_Mat4` - The look-at transformation matrix.

---

<a id="lotus_rotx_mat4"></a>
#### **`lotus_rotx_mat4()`** 🪷

**Description**:  
Creates a rotation matrix around the X-axis.

**Parameters**:  
- `angle` (`f32`): The angle to rotate by, in radians.

**Return Type**:  
`Lotus_Mat4` - The rotation matrix around the X-axis.

---

<a id="lotus_roty_mat4"></a>
#### **`lotus_roty_mat4()`** 🪷

**Description**:  
Creates a rotation matrix around the Y-axis.

**Parameters**:  
- `angle` (`f32`): The angle to rotate by, in radians.

**Return Type**:  
`Lotus_Mat4` - The rotation matrix around the Y-axis.

---

<a id="lotus_rotz_mat4"></a>
#### **`lotus_rotz_mat4()`** 🪷

**Description**:  
Creates a rotation matrix around the Z-axis.

**Parameters**:  
- `angle` (`f32`): The angle to rotate by, in radians.

**Return Type**:  
`Lotus_Mat4` - The rotation matrix around the Z-axis.

---

<a id="lotus_rot_mat4"></a>
#### **`lotus_rot_mat4()`** 🪷

**Description**:  
Creates a rotation matrix around an arbitrary axis.

**Parameters**:  
- `x` (`f32`): The x-component of the axis.
- `y` (`f32`): The y-component of the axis.
- `z` (`f32`): The z-component of the axis.
- `angle` (`f32`): The angle to rotate by, in radians.

**Return Type**:  
`Lotus_Mat4` - The rotation matrix.

---

<a id="lotus_perspective"></a>
#### **`lotus_perspective()`** 🪷

**Description**:  
Creates a perspective projection matrix.

**Parameters**:  
- `fov` (`f32`): The field of view in radians.
- `aspect` (`f32`): The

 aspect ratio of the view (width / height).
- `near` (`f32`): The near clipping plane distance.
- `far` (`f32`): The far clipping plane distance.

**Return Type**:  
`Lotus_Mat4` - The perspective matrix.

---

<a id="lotus_ortho"></a>
#### **`lotus_ortho()`** 🪷

**Description**:  
Creates an orthogonal projection matrix.

**Parameters**:  
- `left` (`f32`): The left plane of the view.
- `right` (`f32`): The right plane of the view.
- `bottom` (`f32`): The bottom plane of the view.
- `top` (`f32`): The top plane of the view.
- `near` (`f32`): The near clipping plane distance.
- `far` (`f32`): The far clipping plane distance.

**Return Type**:  
`Lotus_Mat4` - The orthogonal matrix.

---