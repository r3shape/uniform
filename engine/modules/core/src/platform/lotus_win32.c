#include "../../include/memory/lotus_memory.h"
#include "../../include/platform/lotus_platform.h"

#include <stdio.h>

#if defined(LOTUS_PLATFORM_WINDOWS)
#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

typedef struct Platform_Window_Data {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
    RAWINPUTDEVICE rid;
} Platform_Window_Data;

Lotus_Platform_API *lotus_platform_api = NULL;
static Lotus_Platform_State internal_platform_state = {0};

/* PLATFORM API IMPLEMENTATION */
void _windows_cleanup_impl(void) {
    UnregisterClass("Lotus Window Class", GetModuleHandle(NULL));
    
    lotus_shutdown_input();
    lotus_shutdown_events();
    
    internal_platform_state.input_state = NULL;
    internal_platform_state.clock_frequency = 0.0;
    internal_platform_state.platform = LOTUS_PLATFORM_TAGS;
}

LRESULT CALLBACK _window_proc(HWND handle, ubyte4 msg, WPARAM wParam, LPARAM lParam) {
    Lotus_Window *window = internal_platform_state.windowPtr;
    Platform_Window_Data *windowData = (window == NULL) ? NULL : (Platform_Window_Data *)window->internal_data;
    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY:
            lotus_event_api->push_event((Lotus_Event){.ubyte[0]=1}, LOTUS_EVENT_APP_QUIT);
            PostQuitMessage(0);
            return 0;
        case WM_EXITSIZEMOVE: {
            RECT newRect = {0};
            GetWindowRect(handle, &newRect);

            if (window && (window->cursor_bounded == LOTUS_TRUE)) {
                window->location[0] = newRect.left;
                window->location[1] = newRect.top;
                window->size[0] = newRect.right;
                window->size[1] = newRect.bottom;
                ClipCursor(&newRect);
            }
           
            ubyte2 width = newRect.right - newRect.left;
            ubyte2 height = newRect.bottom - newRect.top;
            lotus_event_api->push_event((Lotus_Event){.ubyte2[0]=width, .ubyte2[1]=height}, LOTUS_EVENT_RESIZE);
        } break;
        case WM_KILLFOCUS: {
            ClipCursor(NULL);
        } break;
        case WM_SETFOCUS: {
            RECT newRect = {0};
            GetWindowRect(handle, &newRect);
            
            if (window && (window->cursor_bounded == LOTUS_TRUE)) {
                window->location[0] = newRect.left;
                window->location[1] = newRect.top;
                window->size[0] = newRect.right;
                window->size[1] = newRect.bottom;
                ClipCursor(&newRect);
            }
        } break;
        /* LEGACY INPUT */
        case WM_KEYUP:          // fall through WM_SYSKEYUP
        case WM_KEYDOWN:        // fall through WM_SYSKEYUP
        case WM_SYSKEYDOWN:     // fall through WM_SYSKEYUP
        case WM_SYSKEYUP: {
            // key pressed/released
            Lotus_Keyboard_Key key = (ubyte2)wParam;
            ubyte pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            lotus_process_key_input(key, pressed);
        } break;
        /* RAW INPUT */
        case WM_INPUT: {
            UINT dwSize;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            
            RAWINPUT raw;
            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
                printf("GetRawInputData did not return correct size!\n");
                return DefWindowProcA(handle, msg, wParam, lParam);
            }

            if (raw.header.dwType == RIM_TYPEMOUSE) {
                RAWMOUSE *mouse = &raw.data.mouse;

                // handle mouse movement
                if (mouse->usFlags & MOUSE_MOVE_ABSOLUTE) {
                    // ignore absolute movement (used for remote desktop, etc.)
                } else {
                    sbyte2 x = (sbyte2)mouse->lLastX;
                    sbyte2 y = (sbyte2)mouse->lLastY;
                    lotus_process_mouse_move_input(x, y);
                }

                // handle mouse buttons
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                    lotus_process_mouse_button_input(LOTUS_MBUTTON_LEFT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                    lotus_process_mouse_button_input(LOTUS_MBUTTON_LEFT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                    lotus_process_mouse_button_input(LOTUS_MBUTTON_RIGHT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                    lotus_process_mouse_button_input(LOTUS_MBUTTON_RIGHT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                    lotus_process_mouse_button_input(LOTUS_MBUTTON_MIDDLE, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                    lotus_process_mouse_button_input(LOTUS_MBUTTON_MIDDLE, 0);
                }

                // handle mouse wheel
                if (mouse->usButtonFlags & RI_MOUSE_WHEEL) {
                    sbyte4 z = (sbyte4)((SHORT)mouse->usButtonData) / WHEEL_DELTA;
                    lotus_process_mouse_wheel_input(z);
                }
            }
        } break;
    }
    return DefWindowProcA(handle, msg, wParam, lParam);
}

Lotus_Window _windows_create_window_impl(const char *title, int width, int height) {
    Lotus_Window window;

    window.internal_data = (Platform_Window_Data *)malloc(sizeof(Platform_Window_Data));
    if (!window.internal_data) {
        return (Lotus_Window){0};
    }
    Platform_Window_Data *data = (Platform_Window_Data*)window.internal_data;

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = _window_proc;
    wc.hInstance = data->instance;
    wc.lpszClassName = "Lotus Window Class";

    if (!RegisterClass(&wc)) {
        free(window.internal_data);
        return (Lotus_Window){0};
    }

    // Create the window
    HWND hwnd = CreateWindow(
        "Lotus Window Class",
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,  // default x
        CW_USEDEFAULT,  // default y
        width,
        height,
        NULL,   // parent
        NULL,   // menu
        data->instance,
        NULL);

    if (!hwnd) {
        free(window.internal_data);
        return (Lotus_Window){0};
    }

    data->handle = hwnd;
    data->gl_context = NULL;
    data->device_context = GetDC(hwnd);

     // Register rawinput devices
    data->rid.usUsagePage = 0x01;       // HID_USAGE_PAGE_GENERIC
    data->rid.usUsage = 0x02;           // HID_USAGE_GENERIC_MOUSE
    data->rid.dwFlags = 0;              // RIDEV_NOLEGACY adds mouse and ignores legacy mouse messages
    data->rid.hwndTarget = data->handle;

    if (!RegisterRawInputDevices(&data->rid, 1, sizeof(data->rid))) {
        printf("Failed to register rawinput devices!\n");
        free(window.internal_data);
        return (Lotus_Window){0};
    }

    // Initialize the generic window structure
    strncpy(window.title, title, sizeof(window.title) - 1);
    window.title[sizeof(window.title) - 1] = '\0';
    window.location[0] = CW_USEDEFAULT;
    window.location[1] = CW_USEDEFAULT;
    window.size[0] = width;
    window.size[1] = height;
    window.aspect_ratio = (float)width / height;
    window.cursor_bounded = LOTUS_FALSE;

    internal_platform_state.windowPtr = &window;

    return window;
}

void _windows_destroy_window_impl(Lotus_Window *window) {
    if (!window) return;    // error: null ptr!
    if (window && window->internal_data) {
        Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
        if (data->handle) {
            DestroyWindow(data->handle);
        }
        free(data);
        window->internal_data = NULL;
    }
}


ubyte _windows_show_cursor_impl(Lotus_Window *window) {
    if (!window) return LOTUS_FALSE;    // error: null ptr!

    // TODO: move this logic to the window process based on a window state flag
    if (ShowCursor(LOTUS_TRUE) >= 0) return LOTUS_TRUE;
    else return LOTUS_FALSE;
}

ubyte _windows_hide_cursor_impl(Lotus_Window *window) {
    if (!window) return LOTUS_FALSE;    // error: null ptr!

    // TODO: move this logic to the window process based on a window state flag
    if (ShowCursor(LOTUS_FALSE) < 0) return LOTUS_TRUE;
    else return LOTUS_FALSE;
}

ubyte _windows_center_cursor_impl(Lotus_Window *window) {
    if (!window || !window->internal_data) return LOTUS_FALSE;    // error: null ptr!

    Platform_Window_Data *windowData = (Platform_Window_Data *)window->internal_data;

    // TODO: move this logic to the window process based on a window state flag
    RECT rect;
    GetWindowRect(windowData->handle, &rect);
    if (SetCursorPos(
        (rect.left + rect.right) / 2,
        (rect.top + rect.bottom) / 2
    )) { return LOTUS_TRUE; }
    return LOTUS_FALSE;
}

ubyte _windows_bound_cursor_impl(Lotus_Window *window) {
    if (!window || !window->internal_data) return LOTUS_FALSE;    // error: null ptr!

    Platform_Window_Data *windowData = (Platform_Window_Data *)window->internal_data;

    RECT rect;
    GetWindowRect(windowData->handle, &rect);

    // TODO: move this logic to the window process based on a window state flag
    if (ClipCursor(&rect)) {
        window->cursor_bounded = LOTUS_TRUE;
        windowData->rid.dwFlags = RIDEV_NOLEGACY;   // enable raw input / disable legacy input
        return LOTUS_TRUE;
    }

    return LOTUS_FALSE;
}

ubyte _windows_unbound_cursor_impl(Lotus_Window *window) {
    if (!window) return LOTUS_FALSE;    // error: null ptr!
    
    Platform_Window_Data *windowData = (Platform_Window_Data *)window->internal_data;
    if (!windowData) return LOTUS_FALSE;

    // TODO: move this logic to the window process based on a window state flag
    if (ClipCursor(NULL)) {
        window->cursor_bounded = LOTUS_FALSE;
        windowData->rid.dwFlags = 0;   // enable legacy input / disable raw input
        return LOTUS_TRUE;
    }

    return LOTUS_FALSE;
}


ubyte _windows_create_gl_context_impl(Lotus_Window *window) {
    if (!window || !window->internal_data) {
        // lotus_log_error("window_ptr/window->internal_data_ptr invalid!");
        return LOTUS_FALSE;
    }

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0, 0,
        0, 0, 0, 0, 0,
        24, 8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int format = ChoosePixelFormat(data->device_context, &pfd);
    if (!SetPixelFormat(data->device_context, format, &pfd)) {
        // lotus_log_error("failed to set pixel format");
        return LOTUS_FALSE;
    }

    data->gl_context = wglCreateContext(data->device_context);
    if (!data->gl_context) {
        // lotus_log_error("failed to create gl context");
        return LOTUS_FALSE;
    }

    if (!wglMakeCurrent(data->device_context, data->gl_context)) {
        // lotus_log_error("failed to bind gl context");
        wglDeleteContext(data->gl_context);
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

void _windows_destroy_gl_context_impl(Lotus_Window *window) {
    if (!window || !window->internal_data) return;

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
    if (data->gl_context) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(data->gl_context);
        data->gl_context = NULL;
    }
}


double _windows_get_time_impl() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / internal_platform_state.clock_frequency;
}

void _windows_sleep_impl(f64 seconds) {
    Sleep((DWORD)(seconds  *1000.0));
}

void _windows_swap_buffers_impl(Lotus_Window *window) {
    if (!window || !window->internal_data) return;

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
    SwapBuffers(data->device_context);
}


ubyte _windows_poll_events_impl(void) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return LOTUS_TRUE;
}

void _windows_poll_inputs_impl(void) {
    lotus_update_input(0.0);
}


void *_windows_get_gl_context_impl(Lotus_Window *window) {
    if (!window || !window->internal_data) {
        // lotus_log_error("Invalid window or internal data!");
        return NULL;
    }

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
    return (void*)data->gl_context;
}

Lotus_Platform_State *_windows_get_state_impl(void) { return &internal_platform_state; }


Lotus_DyLib _windows_load_library_impl(const char *path, const char *name) {
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s.dll", path, name);

    HMODULE handle = LoadLibrary(full_path);
    if (!handle) {
        // lotus_log_error("Failed to load library: %s", full_path);
        return (Lotus_DyLib){.name = NULL, .handle = NULL};
    }

    Lotus_DyLib lib = {.name = strdup(name), .handle = handle};
    return lib;
}

void *_windows_get_library_symbol_impl(Lotus_DyLib *library, const char *symbol_name) {
    if (!library || !library->handle || !symbol_name) {
        // lotus_log_error("Invalid library or symbol name");
        return NULL;
    }
    return (void*)GetProcAddress((HMODULE)library->handle, symbol_name);
}

ubyte _windows_unload_library_impl(Lotus_DyLib *library) {
    if (!library || !library->handle) return LOTUS_FALSE;

    if (!FreeLibrary((HMODULE)library->handle)) {
        // lotus_log_error("Failed to unload library: %s", library->name);
        return LOTUS_FALSE;
    }

    free((char*)library->name);
    library->handle = NULL;
    return LOTUS_TRUE;
}
#endif // LOTUS_PLATFORM_WINDOWS == LOTUS_TRUE
/* PLATFORM API IMPLEMENTATION */


ubyte lotus_init_platform(void) {
    internal_platform_state.platform = LOTUS_WINDOWS_TAG;
    
    if (!lotus_init_events()) return LOTUS_FALSE;   // error: failed to init event layer!
    
    internal_platform_state.input_state = lotus_init_input();
    if (!internal_platform_state.input_state) return LOTUS_FALSE;   // error: failed to init input layer!

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    internal_platform_state.clock_frequency = 1.0 / (f64)frequency.QuadPart;

    lotus_platform_api = lotus_memory_api->alloc(sizeof(Lotus_Platform_API), 16);
    if (!lotus_platform_api) return LOTUS_FALSE;    // error: out of memory!

    lotus_platform_api->get_state = _windows_get_state_impl;
    lotus_platform_api->cleanup = _windows_cleanup_impl;
    lotus_platform_api->get_time = _windows_get_time_impl;
    lotus_platform_api->sleep = _windows_sleep_impl;
    
    lotus_platform_api->load_library = _windows_load_library_impl;
    lotus_platform_api->get_library_symbol = _windows_get_library_symbol_impl;
    lotus_platform_api->unload_library = _windows_unload_library_impl;
    
    lotus_platform_api->poll_events = _windows_poll_events_impl;
    lotus_platform_api->poll_inputs = _windows_poll_inputs_impl;
    
    lotus_platform_api->create_window = _windows_create_window_impl;
    lotus_platform_api->destroy_window = _windows_destroy_window_impl;

    lotus_platform_api->show_cursor = _windows_show_cursor_impl;
    lotus_platform_api->hide_cursor = _windows_hide_cursor_impl;
    lotus_platform_api->bound_cursor = _windows_bound_cursor_impl;
    lotus_platform_api->center_cursor = _windows_center_cursor_impl;
    lotus_platform_api->unbound_cursor = _windows_unbound_cursor_impl;

    lotus_platform_api->create_gl_context = _windows_create_gl_context_impl;
    lotus_platform_api->get_gl_context = _windows_get_gl_context_impl;
    lotus_platform_api->swap_buffers = _windows_swap_buffers_impl;
    lotus_platform_api->destroy_gl_context = _windows_destroy_gl_context_impl;

    return LOTUS_TRUE;
}

void lotus_shutdown_platform(void) {
    lotus_platform_api->cleanup();
    lotus_memory_api->free(lotus_platform_api);
}
