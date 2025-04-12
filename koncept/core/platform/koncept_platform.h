#pragma once

#include <koncept/core/koncept_defines.h>
#include <koncept/core/events/koncept_events.h>
#include <koncept/core/input/koncept_input.h>

typedef enum KC_Window_Flag {
    KC_SHOW_CURSOR =     (1 << 0),
    KC_BIND_CURSOR =     (1 << 1),
    KC_CENTER_CURSOR =   (1 << 2)
} KC_Window_Flag;

typedef struct KC_Window {
    u8 focused;             // Window focused (KC_TRUE/KC_FALSE)
    u16 flags;              // Window flags
    int size[2];            // Window width and height
    int location[2];        // Window x, y position
    char title[256];        // Window title
    float aspect_ratio;     // Aspect ratio (width / height)
} KC_Window;

typedef struct KC_DLL {
    void* handle;
    str name;
} KC_DLL;

typedef struct _koncept_platform_api {
    KC_Window* (*create_window)(const char *title, int width, int height);
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

    KC_DLL (*load_library)(const char *path, char *name);
    void* (*get_symbol)(KC_DLL* library, str name);
    u8 (*unload_library)(KC_DLL* library);
} _koncept_platform_api;

u8 _koncept_init_platform(_koncept_events_api* events_api, _koncept_input_api* input_api, _koncept_platform_api* api);
u8 _koncept_cleanup_platform(_koncept_platform_api* api);