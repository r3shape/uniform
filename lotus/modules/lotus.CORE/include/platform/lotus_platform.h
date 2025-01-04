#pragma once

#include "../types.h"
#include "../defines.h"

#include "lotus_event.h"
#include "lotus_input.h"

typedef enum Lotus_Platform_Tag {
    LOTUS_WINDOWS_TAG,
    LOTUS_MACOS_TAG,
    LOTUS_LINUX_TAG,
    LOTUS_PLATFORM_TAGS
} Lotus_Platform_Tag;

typedef struct Lotus_DyLib {
    const char* name;
    void* handle;
} Lotus_DyLib;

typedef struct Lotus_Platform_State {
    f64 clock_frequency;
    Lotus_Platform_Tag platform;
    Lotus_Event_State* event_state;
    Lotus_Input_State* input_state;
} Lotus_Platform_State;

typedef struct Lotus_Window{
    char title[256];       // Window title
    int location[2];       // Window x, y position
    int size[2];           // Window width and height
    float aspect_ratio;    // Aspect ratio (width / height)
    void* internal_data;   // Pointer to platform-specific window data
} Lotus_Window;

typedef struct Lotus_Platform_API {
    Lotus_Platform_State* (*get_state)(void);
    void (*shutdown)(void);
    f64 (*get_time)(void);
    void (*sleep)(f64 seconds);
    
    Lotus_DyLib (*load_library)(const char* path, const char* name);
    void* (*get_library_symbol)(Lotus_DyLib* library, const char* symbol_name);
    ubyte (*unload_library)(Lotus_DyLib* library);
    
    ubyte (*poll_events)(void);
    
    Lotus_Window (*create_window)(const char* title, int width, int height);
    void (*destroy_window)(Lotus_Window* window);

    ubyte (*create_gl_context)(Lotus_Window* window);
    void* (*get_gl_context)(Lotus_Window* window);
    void (*swap_buffers)(Lotus_Window* window);
    void (*destroy_gl_context)(Lotus_Window* window);
} Lotus_Platform_API;

Lotus_Platform_API* lotus_init_platform(void);
