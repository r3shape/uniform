#pragma once

#include "r3_event.h"
#include "r3_input.h"
#include "../r3_core_defines.h"

typedef enum R3_Platform_Tag {
    R3_WINDOWS_TAG,
    R3_MACOS_TAG,
    R3_LINUX_TAG,
    R3_PLATFORM_TAGS
} R3_Platform_Tag;

typedef struct R3_DyLib {
    char *name;
    void *handle;
} R3_DyLib;

typedef enum R3_Window_Flag {
    R3_SHOW_CURSOR =     (1 << 0),
    R3_BIND_CURSOR =     (1 << 1),
    R3_CENTER_CURSOR =   (1 << 2)
} R3_Window_Flag;

typedef struct R3_Window {
    ubyte focused;         // Window focused (R3_TRUE/R3_FALSE)
    ubyte2 flags;          // Window flags
    char title[256];       // Window title
    int location[2];       // Window x, y position
    int size[2];           // Window width and height
    float aspect_ratio;    // Aspect ratio (width / height)
    void *internal_data;   // Pointer to platform-specific window data
} R3_Window;

typedef struct R3_Clock {
    f64 frequency;
    f64 start_time;
    f64 current_time;
    f64 frame_time;
    f64 delta_time;
    f64 target_fps;
    f64 current_fps;
} R3_Clock;

typedef struct R3_Platform_State {
    R3_Clock clock;
    R3_Window* window;
    R3_Platform_Tag platform;
    R3_Input_State* input_state;
} R3_Platform_State;

typedef struct R3_Platform_API {
    R3_Platform_State* (*get_state)(void);
    void (*cleanup)(void);
    
    // clock + time management
    f64 (*get_time)(void);
    void (*update_clock)(void);
    void (*sleep)(f64 seconds);
    void (*set_clock)(f64 target_fps);
    
    R3_DyLib (*load_library)(const char *path, char *name);
    void* (*get_library_symbol)(R3_DyLib *library, const char *symbol_name);
    ubyte (*unload_library)(R3_DyLib *library);
    
    ubyte (*poll_events)(void);
    void (*poll_inputs)(void);

    ubyte (*get_window_flag)(R3_Window *window, ubyte2 flag);
    ubyte (*set_window_flag)(R3_Window *window, ubyte2 flag);
    ubyte (*rem_window_flag)(R3_Window *window, ubyte2 flag);

    ubyte (*show_cursor)(R3_Window *window);
    ubyte (*hide_cursor)(R3_Window *window);
    ubyte (*bind_cursor)(R3_Window *window);
    ubyte (*unbind_cursor)(R3_Window *window);
    ubyte (*center_cursor)(R3_Window *window);
    ubyte (*decenter_cursor)(R3_Window *window);
    
    R3_Window* (*create_window)(const char *title, int width, int height);
    void (*destroy_window)(R3_Window *window);

    ubyte (*create_gl_context)(R3_Window *window);
    void* (*get_gl_context)(R3_Window *window);
    void (*swap_buffers)(R3_Window *window);
    void (*destroy_gl_context)(R3_Window *window);
} R3_Platform_API;
extern R3_Platform_API *r3_platform_api;

R3_API ubyte r3_init_platform(void);
R3_API void r3_shutdown_platform(void);
