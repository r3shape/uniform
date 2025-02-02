# **Lotus Camera API Reference 🪷**

## **Introduction**  
The **Lotus Camera API** provides the functionality for handling camera systems in **Lotus**. This API allows for controlling the camera's position, rotation, and movement in 2D and 3D space. It includes essential functions for setting up a camera, translating it through the world, rotating it, and updating its vectors and view matrix.

---

<br>
<br>

<a id="Structures"></a>
# **Structures 🪷**

### **Reference Sections:** [Functions](#Functions)

#### **`Lotus_Camera`** 🪷

```c
typedef struct Lotus_Camera {
    Lotus_Vec3 globalUp;
    f32 yaw;
    f32 roll;
    f32 pitch;
    f32 fov;
    f32 speed;
    f32 sensitivity;
    f32 farPlane;
    f32 nearPlane;
    Lotus_Vec3 target;
    Lotus_Vec3 location;
    Lotus_Vec3 up;
    Lotus_Vec3 right;
    Lotus_Vec3 direction;
    Lotus_Mat4 view;
} Lotus_Camera;
```

**Description**:  
This structure defines the properties and transformation data of the camera.

- **globalUp**: A vector representing the global "up" direction (usually the Y-axis).
- **yaw**: The rotation of the camera around the vertical axis.
- **roll**: The rotation of the camera around its own forward axis.
- **pitch**: The rotation of the camera around the horizontal axis.
- **fov**: The field of view of the camera.
- **speed**: The movement speed of the camera.
- **sensitivity**: The sensitivity for mouse movement input.
- **farPlane**: The far clipping plane distance.
- **nearPlane**: The near clipping plane distance.
- **target**: The point the camera is looking at.
- **location**: The camera's current position in 3D space.
- **up**: The camera's local "up" direction.
- **right**: The camera's local "right" direction.
- **direction**: The camera's forward direction.
- **view**: The camera's view matrix.

---

<br>
<br>

<a id="Functions"></a>
# **Functions 🪷**

### **Reference Sections:** [Structures](#Structures)

Below is a list of the functions provided in the API. Click on any function to jump directly to its documentation.

### **Camera Setup Functions:**
- [lotus_init_camera](#lotus_init_camera)

### **Camera Movement Functions:**
- [lotus_translate_camera](#lotus_translate_camera) | [lotus_rotate_camera](#lotus_rotate_camera)

### **Camera Update Functions:**
- [lotus_update_camera](#lotus_update_camera) | [lotus_freelook_camera](#lotus_freelook_camera)

---

<a id="lotus_init_camera"></a>
#### **`lotus_init_camera()`** 🪷

**Description**:  
Initializes a new camera at a specific location and targeting a specific point.

**Parameters**:  
- `location` (`Lotus_Vec3`): The initial position of the camera.
- `target` (`Lotus_Vec3`): The point the camera should look at.

**Return Type**:  
`Lotus_Camera` - The initialized camera.

---

<a id="lotus_translate_camera"></a>
#### **`lotus_translate_camera()`** 🪷

**Description**:  
Translates the camera by a given amount in 3D space.

**Parameters**:  
- `camera` (`Lotus_Camera*`): The camera to translate.
- `x` (`sbyte`): The amount to move along the X-axis.
- `y` (`sbyte`): The amount to move along the Y-axis.
- `z` (`sbyte`): The amount to move along the Z-axis.

**Return Type**:  
None.

---

<a id="lotus_rotate_camera"></a>
#### **`lotus_rotate_camera()`** 🪷

**Description**:  
Rotates the camera by specified amounts along the X and Y axes.

**Parameters**:  
- `camera` (`Lotus_Camera*`): The camera to rotate.
- `deltaX` (`f32`): The amount of rotation around the X-axis.
- `deltaY` (`f32`): The amount of rotation around the Y-axis.

**Return Type**:  
None.

---

<a id="lotus_update_camera"></a>
#### **`lotus_update_camera()`** 🪷

**Description**:  
Updates the camera's internal state, recalculating its view matrix and direction based on its position and orientation.

**Parameters**:  
- `camera` (`Lotus_Camera*`): The camera to update.

**Return Type**:  
None.

---

<a id="lotus_freelook_camera"></a>
#### **`lotus_freelook_camera()`** 🪷

**Description**:  
Allows the camera to be oriented freely based on user input from the mouse.

**Parameters**:  
- `camera` (`Lotus_Camera*`): The camera to control.

**Return Type**:  
None.

---