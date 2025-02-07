#pragma once

#include "lotus_event.h"
#include "lotus_input.h"
#include "../lotus_core_defines.h"

typedef enum Lotus_Platform_Tag {
    LOTUS_WINDOWS_TAG,
    LOTUS_MACOS_TAG,
    LOTUS_LINUX_TAG,
    LOTUS_PLATFORM_TAGS
} Lotus_Platform_Tag;

typedef struct Lotus_DyLib {
    char *name;
    void *handle;
} Lotus_DyLib;

typedef enum Lotus_Window_Flag {
    LOTUS_SHOW_CURSOR =     (1 << 0),
    LOTUS_BIND_CURSOR =     (1 << 1),
    LOTUS_CENTER_CURSOR =   (1 << 2)
} Lotus_Window_Flag;

typedef struct Lotus_Window{
    ubyte focused;         // Window focused (LOTUS_TRUE/LOTUS_FALSE)
    ubyte2 flags;          // Window flags
    char title[256];       // Window title
    int location[2];       // Window x, y position
    int size[2];           // Window width and height
    float aspect_ratio;    // Aspect ratio (width / height)
    void *internal_data;   // Pointer to platform-specific window data
} Lotus_Window;

typedef struct Lotus_Clock {
    f64 frequency;
    f64 start_time;
    f64 current_time;
    f64 frame_time;
    f64 delta_time;
    f64 target_fps;
    f64 current_fps;
} Lotus_Clock;

typedef struct Lotus_Platform_State {
    Lotus_Clock clock;
    Lotus_Window* window;
    Lotus_Platform_Tag platform;
    Lotus_Input_State* input_state;
} Lotus_Platform_State;

typedef struct Lotus_Platform_API {
    Lotus_Platform_State* (*get_state)(void);
    void (*cleanup)(void);
    
    // clock + time management
    f64 (*get_time)(void);
    void (*update_clock)(void);
    void (*sleep)(f64 seconds);
    void (*set_clock)(f64 target_fps);
    
    Lotus_DyLib (*load_library)(const char *path, char *name);
    void* (*get_library_symbol)(Lotus_DyLib *library, const char *symbol_name);
    ubyte (*unload_library)(Lotus_DyLib *library);
    
    ubyte (*poll_events)(void);
    void (*poll_inputs)(void);

    ubyte (*get_window_flag)(Lotus_Window *window, ubyte2 flag);
    ubyte (*set_window_flag)(Lotus_Window *window, ubyte2 flag);
    ubyte (*rem_window_flag)(Lotus_Window *window, ubyte2 flag);

    ubyte (*show_cursor)(Lotus_Window *window);
    ubyte (*hide_cursor)(Lotus_Window *window);
    ubyte (*bind_cursor)(Lotus_Window *window);
    ubyte (*unbind_cursor)(Lotus_Window *window);
    ubyte (*center_cursor)(Lotus_Window *window);
    ubyte (*decenter_cursor)(Lotus_Window *window);
    
    Lotus_Window* (*create_window)(const char *title, int width, int height);
    void (*destroy_window)(Lotus_Window *window);

    ubyte (*create_gl_context)(Lotus_Window *window);
    void* (*get_gl_context)(Lotus_Window *window);
    void (*swap_buffers)(Lotus_Window *window);
    void (*destroy_gl_context)(Lotus_Window *window);
} Lotus_Platform_API;
extern Lotus_Platform_API *lotus_platform_api;

LOTUS_API ubyte lotus_init_platform(void);
LOTUS_API void lotus_shutdown_platform(void);
