# **Hello Triangle Example 🪷**

Welcome to the **Hello Triangle** example! 🪷  
In this example, we will walk through creating a basic 2D application that renders a triangle using **Lotus**. This example will also introduce you to **Lotus's architecture**, including how modules such as **Lotus 2D**, **ECS**, and **Lotus Core** interact to provide game engine functionality.

By the end of this example, you will have a basic understanding of how to initialize **Lotus**, create a window, set up graphics, and render a triangle. You'll also learn how to build the project using **Cbuild**, a lightweight build tool for C projects.

---

<br>
<br>

## **Step 1: Understanding Lotus Architecture** 🪷

### **Lotus Architecture** 🪷
**Lotus** follows a modular architecture. The base **Lotus Core** module provides fundamental functionalities such as memory management, platform abstraction, input handling, and event management. All other modules like **Lotus 2D**, **Lotus 3D**, and **Lotus ECS** are built on top of **Lotus Core**.

- **Lotus Core**: Provides the core functionality, including memory management, platform API, and event handling.
- **Lotus ECS**: The **Entity-Component-System** (ECS) module that drives the game logic by organizing entities and components in a data-driven manner.
- **Lotus 2D & 3D**: These modules provide functionality for rendering and managing 2D and 3D content, respectively, and are built directly on top of **Lotus ECS**.

### **Engine Initialization** 🪷
When you call `lotus_init_2D()` or `lotus_init_3D()`, these functions initialize the necessary engine layers:
- **Memory Layer**: Allocates and manages memory for the engine.
- **Platform Layer**: Manages platform-specific operations (e.g., window creation, input handling).
- **Event Layer**: Sets up event handling for things like keyboard presses, mouse movements, etc.
- **Input Layer**: Handles input from devices such as the keyboard and mouse.

Graphics initialization (e.g., window creation, OpenGL context setup) requires a **platform API call** (`lotus_platform_api->create_window()`) to establish a window and OpenGL context. This is why graphics cannot be initialized successfully until after calling this function.

---

<br>
<br>

## **Step 2: Setting Up Your Project with Cbuild** 🪷

Before diving into the code, let's set up the project and build it using **Cbuild**.

1. **Install Cbuild**:  
   Cbuild is a minimalistic build tool designed to simplify the compilation process for C projects. It is lightweight and ideal for projects with few dependencies.

2. **Project Directory Structure**:  
   Organize your project directory as follows:

   ```
   Hello_Triangle/
   ├── examples/
   │   └── hello_triangle/
   │       └── main.c
   ├── lotus/
   └── build.cbuild
   ```

3. **Cbuild Configuration**:  
   Inside your project directory, create a file named `cbuild.json` to specify your build configuration:

   ```json
   {
       "compiler": "MinGW",
       "project": "Hello_Triangle",
       "include_dirs": ["lotus/include"],
       "source_files": ["examples/hello_triangle/main.c"],
       "libraries": {
           "Lotus": "build"
       },
       "output": "exe",
       "output_dir": "build"
   }
   ```

   This configuration tells **Cbuild** where to find the source files, includes and linked libraries, as well as where to output the compiled executable.

4. **Build the Project**:  
   Run the following command to compile the project:

   ```bash
   cbuild build.cbuild
   ```

   **Cbuild** will automatically handle the compilation and place the executable in the `build` directory.

---

<br>
<br>

## **Step 3: Writing the Code** 🪷

Now, let’s walk through the code to render a basic triangle using **Lotus**.

### **Creating the Window** 🪷
The first step is to create a window using **Lotus Platform API**. The `lotus_init_2D()` function initializes the 2D graphics module, but before we can use the **Graphics API**, we need to create a window and OpenGL context:

```c
#define LOTUS_2D
#include "../../lotus/include/lotus.h"

int main() {
    lotus_init_2D();

    Lotus_Window* window = lotus_platform_api->create_window("Hello Triangle", 1280, 720);
    lotus_platform_api->create_gl_context(window);
```

- `lotus_init_2D()` initializes the **2D module**, which is built on top of **ECS** and **Lotus Core**.
- `lotus_platform_api->create_window()` creates a window and initializes the OpenGL context for rendering. The window size is set to 1280x720 pixels.
- `lotus_platform_api->create_gl_context()` creates the OpenGL context for rendering to this window.

### **Setting Up Graphics** 🪷
After the window is created, we initialize the **Graphics API**:

```c
    lotus_init_graphics();
    lotus_graphics_api->GL_API.viewport(0, 0, window->size[0], window->size[1]);
```

- `lotus_init_graphics()` initializes the **Graphics Layer** of the engine.
- `lotus_graphics_api->GL_API.viewport()` sets up the OpenGL viewport to match the window size, ensuring proper rendering.

### **Setting Up the Camera** 🪷
Next, we create a camera to view the scene. Here, we initialize the camera at a position and direct it towards the origin (0, 0, 0):

```c
    Lotus_Camera cam = lotus_init_camera(
        lotus_new_vec3(0, 0, 1.0),
        lotus_new_vec3(0, 0, 0)
    );
```

The camera is placed at `(0, 0, 1.0)` and is looking at `(0, 0, 0)`. This is essential for setting the scene's perspective.

### **Loading Shaders** 🪷
Now, we load the vertex and fragment shaders:  
(Note: shader code can also be loaded from memory utilizing multi-line strings)
```c
    Lotus_Shader shader = lotus_graphics_api->create_shader(
        lotus_read_file("lotus/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("lotus/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);
```

Shaders are essential for rendering graphics. The **vertex shader** handles the geometry of our triangle, and the **fragment shader** determines its color. We load the shaders from disk using `lotus_read_file()` and pass them to `lotus_graphics_api->create_shader()`.

### **Setting Initial State** 🪷
We near the ***main-loop*** by specifying the draw mode, clear color, and projection matrix:

```c
    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(123, 161, 172, 255),
        lotus_ortho(0, window->size[0], 0, window->size[1], 0.1, 10.0)
    );
```

- `LOTUS_TRIANGLE_MODE` specifies that we are drawing triangles.
- `LOTUS_COLOR4(123, 161, 172, 255)` sets the color for the triangle.
- `lotus_ortho()` defines the orthogonal projection for 2D rendering.

### **ECS: Creating the Triangle Entity** 🪷
We now introduce **ECS** (Entity-Component-System). We create an entity and add a **Mesh2D** component and a **Transform2D** component to it:

```c
    ubyte2 entity = lotus_ecs_api->create_entity();
    Lotus_Mesh2D* mesh = lotus_ecs_api->add_component(entity, LOTUS_MESH2D);
    Lotus_Transform2D* transform = lotus_ecs_api->add_component(entity, LOTUS_TRANSFORM2D);
```

ECS provides a clean, data-driven way to organize game objects (entities) and their properties (components).

### **Defining the Mesh and Transform** 🪷
We define the triangle's size, color, and its initial position:

```c
    mesh->size = lotus_new_vec2(64, 64);
    mesh->color = LOTUS_COLOR3(85, 42, 115);
    mesh->vertexData = lotus_2D->create_triangle2D(mesh->size, mesh->color);

    transform->speed = 5.0;
    transform->scale = lotus_new_vec2(1.0, 1.0);
    transform->location = lotus_new_vec2(640, 360);
```

- `lotus_2D->create_triangle2D()` creates the vertex data for a simple triangle.
- The transform component defines the position, scale, and speed of the triangle, allowing us to move it around the screen.

### **Main Loop** 🪷
Now, we enter the **main loop**. This loop continuously checks for input, updates the entity's transform, and re-renders the scene.

So to get all that done, firstly, we poll for events and input via the platform API. 

```c
    ubyte running = 1;
    while (running) {
        lotus_platform_api->poll_events();
        lotus_platform_api->poll_inputs();
```

<br>

Then we clear the screen, and handle input to move the triangle with **W, A, S, D** keys and stop the program with the **Escape** key:

```c
        lotus_graphics_api->draw_clear();

        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (lotus_key_is_down(LOTUS_KEY_A)) transform->velocity.x = -transform->speed;
        else if (lotus_key_is_down(LOTUS_KEY_D)) transform->velocity.x = transform->speed;
        else transform->velocity.x = 0.0;
        
        if (lotus_key_is_down(LOTUS_KEY_W)) transform->velocity.y = transform->speed;
        else if (lotus_key_is_down(LOTUS_KEY_S)) transform->velocity.y = -transform->speed;
        else transform->velocity.y = 0.0;
```

<br>

Then we make a call to the ECS API to run our component systems on the Transform2D and Mesh2D components:

```c
        lotus_ecs_api->run_system(LOTUS_TRANSFORM2D);
        lotus_ecs_api->run_system(LOTUS_MESH2D);
```

<br>

To finish off the main loop, we update our camera, and tell the platform layer to swap our window buffers:
```c
        lotus_update_camera(&cam);
        lotus_platform_api->swap_buffers(window);
    }
```


### **Cleanup**
Finally, once we've exited the main loop, we can clean up by destroying the shader and entity, and shutting down the graphics layer and 2D module:

```c
    lotus_graphics_api->destroy_shader(&shader);
    lotus_ecs_api->destroy_entity(entity);

    lotus_shutdown_graphics();
    lotus_shutdown_2D();
    return 0;
}
```

---

<br>
<br>

## **Full Source Code**

Here’s the complete source code for your reference:

```c
#define LOTUS_2D
#include "../../lotus/include/lotus.h"

int main () {
    lotus_init_2D();

    Lotus_Window* window = lotus_platform_api->create_window("Hello Triangle", 1280, 720);
    lotus_platform_api->create_gl_context(window);

    lotus_init_graphics();
    lotus_graphics_api->GL_API.viewport(0, 0, window->size[0], window->size[1]);

    Lotus_Camera cam = lotus_init_camera(
        lotus_new_vec3(0, 0, 1.0),
        lotus_new_vec3(0, 0, 0)
    );

    Lotus_Shader shader = lotus_graphics_api->create_shader(
        lotus_read_file("lotus/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("lotus/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);

    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(123, 161, 172, 255),
        lotus_ortho(0, window->size[0], 0, window->size[1], 0.1, 10.0)
    );

    ubyte2 entity = lotus_ecs_api->create_entity();
    Lotus_Mesh2D* mesh = lotus_ecs_api->add_component(entity, LOTUS_MESH2D);
    Lotus_Transform2D* transform = lotus_ecs_api->add_component(entity, LOTUS_TRANSFORM2D);

    mesh->size = lotus_new_vec2(64, 64);
    mesh->color = LOTUS_COLOR3(85, 42, 115);
    mesh->vertexData = lotus_2D->create_triangle2D(mesh->size, mesh->color);

    transform->speed = 5.0;
    transform->scale = lotus_new_vec2(1.0, 1.0);
    transform->location = lotus_new_vec2(640, 360);

    ubyte running = 1;
    while (running) {
        lotus_platform_api->poll_events();
        lotus_platform_api->poll_inputs();
        lotus_graphics_api->draw_clear();

        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) running = 0;
        
        if (lotus_key_is_down(LOTUS_KEY_A)) transform->velocity.x = -transform->speed;
        else if (lotus_key_is_down(LOTUS_KEY_D)) transform->velocity.x = transform->speed;
        else transform->velocity.x = 0.0;
        
        if (lotus_key_is_down(LOTUS_KEY_W)) transform->velocity.y = transform->speed;
        else if (lotus_key_is_down(LOTUS_KEY_S)) transform->velocity.y = -transform->speed;
        else transform->velocity.y = 0.0;

        lotus_ecs_api->run_system(LOTUS_TRANSFORM2D);
        lotus_ecs_api->run_system(LOTUS_MESH2D);

        lotus_update_camera(&cam);
        lotus_platform_api->swap_buffers(window);
    }

    lotus_graphics_api->destroy_shader(&shader);
    lotus_ecs_api->destroy_entity(entity);

    lotus_shutdown_graphics();
    lotus_shutdown_2D();
    return 0;
}
```

---

<br>
<br>

## **Conclusion 🪷**

In this example, we learned how to:
1. Set up the project and build it using **Cbuild**.
2. Create a window and OpenGL context with **Lotus**.
3. Set up the camera, load shaders, and render a triangle.
4. Move the triangle using the keyboard input (W, A, S, D).
5. Clean up and shut down the graphics and 2D systems.

With **Lotus**'s modular structure, we were able to combine **Lotus Core**, **Lotus ECS**, and **Lotus 2D** to create a functional 2D scene in just a few lines of code.

This example serves as a foundation for building more complex applications. You can now expand this project by adding more entities, implementing more systems, and enhancing your scene!

---

### **Notes** 🪷

- This example uses **Cbuild** for building the project. For more detailed instructions, check the **README** file in the repository.