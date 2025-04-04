#pragma once

#include "r3.events.h"
#include "r3.input.h"
#include "r3.defines.h"

typedef enum R3_Window_Flag {
    R3_SHOW_CURSOR =     (1 << 0),
    R3_BIND_CURSOR =     (1 << 1),
    R3_CENTER_CURSOR =   (1 << 2)
} R3_Window_Flag;

typedef struct R3_Window {
    u8 focused;             // Window focused (R3_TRUE/R3_FALSE)
    u16 flags;              // Window flags
    int size[2];            // Window width and height
    int location[2];        // Window x, y position
    char title[256];        // Window title
    float aspect_ratio;     // Aspect ratio (width / height)
} R3_Window;

typedef struct R3_DLL {
    void* handle;
    str name;
} R3_DLL;

typedef struct _r3_platform_api {
    R3_Window* (*create_window)(const char *title, int width, int height);
    void (*destroy_window)(void);

    u8 (*create_gl_context)(void);
    void (*swap_buffers)(void);
    void (*destroy_gl_context)(void);

    void (*poll_events)(void);
    void (*poll_inputs)(void);

    u8 (*get_window_flag)(u16 flag);
    u8 (*set_window_flag)(u16 flag);
    u8 (*rem_window_flag)(u16 flag);
    u8 (*show_cursor)(void);
    u8 (*hide_cursor)(void);
    u8 (*center_cursor)(void);
    u8 (*decenter_cursor)(void);
    u8 (*bind_cursor)(void);
    u8 (*unbind_cursor)(void);

    void (*toggle_vsync)(u8 toggle);

    R3_DLL (*load_library)(const char *path, char *name);
    void* (*get_symbol)(R3_DLL* library, str name);
    u8 (*unload_library)(R3_DLL* library);
} _r3_platform_api;

u8 _r3_init_platform(_r3_events_api* events_api, _r3_input_api* input_api, _r3_platform_api* api);
u8 _r3_cleanup_platform(_r3_platform_api* api);