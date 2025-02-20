#include "../../include/memory/r3_memory.h"
#include "../../include/platform/r3_platform.h"

#include <stdio.h>

#if defined(R3_PLATFORM_WINDOWS)
#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

typedef struct Platform_Window_Data {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
    RAWINPUTDEVICE rid;
} Platform_Window_Data;

R3_Platform_API *r3_platform_api = NULL;
static R3_Platform_State internal_platform_state = {0};

/* PLATFORM API IMPLEMENTATION */
void _windows_cleanup_impl(void) {
    UnregisterClass("R3 Window Class", GetModuleHandle(NULL));
    
    r3_shutdown_input();
    r3_shutdown_events();
    
    internal_platform_state.input_state = NULL;
    internal_platform_state.platform = R3_PLATFORM_TAGS;
}

LRESULT CALLBACK _window_proc(HWND handle, ubyte4 msg, WPARAM wParam, LPARAM lParam) {
    R3_Window *window = internal_platform_state.window;
    if (!window) return DefWindowProcA(handle, msg, wParam, lParam);    // error: how did you get here?

    Platform_Window_Data *window_data = (Platform_Window_Data *)window->internal_data;

    RECT window_rect;
    GetWindowRect(window_data->handle, &window_rect);

    // only handle window flags during focus
    if (window->focused) {
        ShowCursor(r3_platform_api->get_window_flag(window, R3_SHOW_CURSOR));
        if (r3_platform_api->get_window_flag(window, R3_BIND_CURSOR))
            ClipCursor(&window_rect);
        if (r3_platform_api->get_window_flag(window, R3_CENTER_CURSOR))
            SetCursorPos((window_rect.left + window_rect.right) / 2, (window_rect.top + window_rect.bottom) / 2);
    }

    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY:
            r3_event_api->push_event((R3_Event){.ubyte[0]=1}, R3_EVENT_APP_QUIT);
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:       // fall through to WMEXITSIZEMOVE
        case WM_MOVE:       // fall through to WMEXITSIZEMOVE
        case WM_EXITSIZEMOVE: {
            RECT newRect = {0};
            GetWindowRect(handle, &newRect);
            window->size[0] = newRect.right;
            window->size[1] = newRect.bottom;
            window->location[1] = newRect.top;
            window->location[0] = newRect.left;

            if (r3_platform_api->get_window_flag(window, R3_BIND_CURSOR)) ClipCursor(&newRect);
           
            ubyte2 width = newRect.right - newRect.left;
            ubyte2 height = newRect.bottom - newRect.top;
            r3_event_api->push_event((R3_Event){.ubyte2[0]=width, .ubyte2[1]=height}, R3_EVENT_RESIZE);
        } break;
        case WM_KILLFOCUS: {
            window->focused = R3_FALSE;
            ClipCursor(NULL);
        } break;
        case WM_SETFOCUS: {
            window->focused = R3_TRUE;
            if (r3_platform_api->get_window_flag(window, R3_BIND_CURSOR)) {
                RECT newRect = {0};
                GetWindowRect(handle, &newRect);
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
            R3_Keyboard_Key key = (ubyte2)wParam;
            ubyte pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            r3_process_key_input(key, pressed);
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
                    r3_process_mouse_move_input(x, y);
                }

                // handle mouse buttons
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                    r3_process_mouse_button_input(R3_MBUTTON_LEFT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                    r3_process_mouse_button_input(R3_MBUTTON_LEFT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                    r3_process_mouse_button_input(R3_MBUTTON_RIGHT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                    r3_process_mouse_button_input(R3_MBUTTON_RIGHT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                    r3_process_mouse_button_input(R3_MBUTTON_MIDDLE, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                    r3_process_mouse_button_input(R3_MBUTTON_MIDDLE, 0);
                }

                // handle mouse wheel
                if (mouse->usButtonFlags & RI_MOUSE_WHEEL) {
                    sbyte4 z = (sbyte4)((SHORT)mouse->usButtonData) / WHEEL_DELTA;
                    r3_process_mouse_wheel_input(z);
                }
            }
        } break;
    }
    return DefWindowProcA(handle, msg, wParam, lParam);
}

R3_Window* _windows_create_window_impl(const char *title, int width, int height) {
    internal_platform_state.window = (R3_Window*)malloc(sizeof(R3_Window));
    if (!internal_platform_state.window) return NULL;    // error: out of memory!
    
    internal_platform_state.window->internal_data = (Platform_Window_Data *)malloc(sizeof(Platform_Window_Data));
    if (!internal_platform_state.window->internal_data) {   // error: out of memory!
        free(internal_platform_state.window);
        return NULL;
    }

    R3_Window* window = internal_platform_state.window;
    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = _window_proc;
    wc.hInstance = data->instance;
    wc.lpszClassName = "R3 Window Class";

    if (!RegisterClass(&wc)) {  // error: failed to register window class!
        free(window->internal_data);
        free(window);
        return NULL;
    }

    // Create the window
    HWND hwnd = CreateWindow(
        "R3 Window Class",
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

    if (!hwnd) {    // error: failed to create window class!
        free(window->internal_data);
        free(window);
        return NULL;
    }

    data->handle = hwnd;
    data->gl_context = NULL;
    data->device_context = GetDC(hwnd);

     // Register rawinput devices
    data->rid.usUsagePage = 0x01;       // HID_USAGE_PAGE_GENERIC
    data->rid.usUsage = 0x02;           // HID_USAGE_GENERIC_MOUSE
    data->rid.dwFlags = 0;              // RIDEV_NOLEGACY adds raw mouse and ignores legacy mouse messages
    data->rid.hwndTarget = data->handle;

    if (!RegisterRawInputDevices(&data->rid, 1, sizeof(data->rid))) {
        // error: failed to register rawinput devices!
        free(window->internal_data);
        free(window);
        return NULL;
    }

    // Initialize the generic window structure
    strncpy(window->title, title, sizeof(window->title) - 1);
    window->focused = R3_TRUE;
    window->flags = 0;
    window->title[sizeof(window->title) - 1] = '\0';
    window->location[0] = CW_USEDEFAULT;
    window->location[1] = CW_USEDEFAULT;
    window->size[0] = width;
    window->size[1] = height;
    window->aspect_ratio = (float)width / height;

    // Set default window flags
    if (
        !r3_platform_api->set_window_flag(window, R3_SHOW_CURSOR)
    ) { // error: failed to set default flags!
        free(window->internal_data);
        free(window);
        return NULL;
    }

    return window;
}

void _windows_destroy_window_impl(R3_Window *window) {
    if (!window) return;    // error: null ptr!
    if (window->internal_data) {
        Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
        if (data->handle) {
            DestroyWindow(data->handle);
        }
        free(data);
    }
    free(window);
}


/* WINDOW STATE API */
ubyte _get_window_flag_impl(R3_Window *window, ubyte2 flag) {
    if (!window) return R3_FALSE;
    return ((window->flags & flag) == flag) ? R3_TRUE : R3_FALSE;
}

ubyte _set_window_flag_impl(R3_Window *window, ubyte2 flag) {
    if (!window) return R3_FALSE;
    window->flags |= flag;
    return R3_TRUE;
}

ubyte _rem_window_flag_impl(R3_Window *window, ubyte2 flag) {
    if (!window) return R3_FALSE;
    window->flags &= ~flag;
    return R3_TRUE;
}


/* WINDOW CURSOR API */
ubyte _windows_show_cursor_impl(R3_Window *window) {
    if (!window) return R3_FALSE;    // error: null ptr!
    if (r3_platform_api->set_window_flag(window, R3_SHOW_CURSOR)) return R3_TRUE;
    else return R3_FALSE;
}

ubyte _windows_hide_cursor_impl(R3_Window *window) {
    if (!window) return R3_FALSE;    // error: null ptr!
    if (r3_platform_api->rem_window_flag(window, R3_SHOW_CURSOR)) return R3_TRUE;
    else return R3_FALSE;
}

ubyte _windows_center_cursor_impl(R3_Window *window) {
    if (!window) return R3_FALSE;    // error: null ptr!
    if (r3_platform_api->set_window_flag(window, R3_CENTER_CURSOR)) return R3_TRUE;
    return R3_FALSE;
}

ubyte _windows_decenter_cursor_impl(R3_Window *window) {
    if (!window) return R3_FALSE;    // error: null ptr!
    if (r3_platform_api->rem_window_flag(window, R3_CENTER_CURSOR)) return R3_TRUE;
    return R3_FALSE;
}

ubyte _windows_bind_cursor_impl(R3_Window *window) {
    if (!window) return R3_FALSE;    // error: null ptr!
    if (r3_platform_api->set_window_flag(window, R3_BIND_CURSOR)) return R3_TRUE;
    return R3_FALSE;
}

ubyte _windows_unbind_cursor_impl(R3_Window *window) {
    Platform_Window_Data *window_data = (Platform_Window_Data *)window->internal_data;
    if (!window || !window_data) return R3_FALSE;    // error: null ptr!

    if (r3_platform_api->rem_window_flag(window, R3_BIND_CURSOR)) {
        if (ClipCursor(NULL)) return R3_TRUE;
    }
    return R3_FALSE;
}


ubyte _windows_create_gl_context_impl(R3_Window *window) {
    if (!window || !window->internal_data) {
        // r3_log_error("window_ptr/window->internal_data_ptr invalid!");
        return R3_FALSE;
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
        // r3_log_error("failed to set pixel format");
        return R3_FALSE;
    }

    data->gl_context = wglCreateContext(data->device_context);
    if (!data->gl_context) {
        // r3_log_error("failed to create gl context");
        return R3_FALSE;
    }

    if (!wglMakeCurrent(data->device_context, data->gl_context)) {
        // r3_log_error("failed to bind gl context");
        wglDeleteContext(data->gl_context);
        return R3_FALSE;
    }

    return R3_TRUE;
}

void _windows_destroy_gl_context_impl(R3_Window *window) {
    if (!window || !window->internal_data) return;

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
    if (data->gl_context) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(data->gl_context);
        data->gl_context = NULL;
    }
}


f64 _windows_get_time_impl(void) {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (f64)counter.QuadPart * internal_platform_state.clock.frequency; // seconds = ticks * seconds per tick
}

void _windows_sleep_impl(f64 seconds) {
    Sleep((DWORD)(seconds  *1000.0));
}

void _set_clock_impl(f64 target_fps) {
    internal_platform_state.clock.start_time = r3_platform_api->get_time();
    internal_platform_state.clock.current_time = internal_platform_state.clock.start_time;
    internal_platform_state.clock.frame_time = 0.0f;
    internal_platform_state.clock.delta_time = 0.0f;
    internal_platform_state.clock.target_fps = target_fps;
    internal_platform_state.clock.current_fps = 0.0f;
}

void _update_clock_impl(void) {
    f64 new_time = r3_platform_api->get_time();
    internal_platform_state.clock.delta_time = new_time - internal_platform_state.clock.current_time;
    internal_platform_state.clock.frame_time = internal_platform_state.clock.delta_time;
    internal_platform_state.clock.current_time = new_time;

    // calculate FPS
    if (internal_platform_state.clock.delta_time > 0.0) {
        internal_platform_state.clock.current_fps = 1.0 / internal_platform_state.clock.delta_time;
    } else {
        internal_platform_state.clock.current_fps = internal_platform_state.clock.target_fps;
    }

    // frame cap logic
    f64 target_frame_time = 1.0 / internal_platform_state.clock.target_fps;
    f64 sleep_time = target_frame_time - internal_platform_state.clock.delta_time;

    if (sleep_time > 0.002) {  
        r3_platform_api->sleep(sleep_time - 0.001); // sleep to avoid overshooting frame time
    }

    // recalculate time after sleep
    while ((r3_platform_api->get_time() - internal_platform_state.clock.current_time) < target_frame_time) {
        SwitchToThread();  // yield CPU during busy-wait
    }
}


ubyte _windows_poll_events_impl(void) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return R3_TRUE;
}

void _windows_poll_inputs_impl(void) {
    r3_update_input(0.0);
}


void *_windows_get_gl_context_impl(R3_Window *window) {
    if (!window || !window->internal_data) {
        // r3_log_error("Invalid window or internal data!");
        return NULL;
    }

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
    return (void*)data->gl_context;
}

void _windows_swap_buffers_impl(R3_Window *window) {
    if (!window || !window->internal_data) return;

    Platform_Window_Data *data = (Platform_Window_Data*)window->internal_data;
    SwapBuffers(data->device_context);
}


R3_Platform_State *_windows_get_state_impl(void) { return &internal_platform_state; }


R3_DyLib _windows_load_library_impl(const char *path, char *name) {
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s.dll", path, name);

    HMODULE handle = LoadLibrary(full_path);
    if (!handle) {
        // r3_log_error("Failed to load library: %s", full_path);
        return (R3_DyLib){.name = NULL, .handle = NULL};
    }

    R3_DyLib lib = {.name = name, .handle = handle};
    return lib;
}

void *_windows_get_library_symbol_impl(R3_DyLib *library, const char *symbol_name) {
    if (!library || !library->handle || !symbol_name) {
        // r3_log_error("Invalid library or symbol name");
        return NULL;
    }
    return (void*)GetProcAddress((HMODULE)library->handle, symbol_name);
}

ubyte _windows_unload_library_impl(R3_DyLib *library) {
    if (!library || !library->handle) return R3_FALSE;

    if (!FreeLibrary((HMODULE)library->handle)) {
        // r3_log_error("Failed to unload library: %s", library->name);
        return R3_FALSE;
    }

    library->name = NULL;
    library->handle = NULL;

    return R3_TRUE;
}
#endif // R3_PLATFORM_WINDOWS == R3_TRUE
/* PLATFORM API IMPLEMENTATION */


ubyte r3_init_platform(void) {
    internal_platform_state.platform = R3_WINDOWS_TAG;
    
    if (!r3_init_events()) return R3_FALSE;   // error: failed to init event layer!
    
    internal_platform_state.input_state = r3_init_input();
    if (!internal_platform_state.input_state) return R3_FALSE;   // error: failed to init input layer!

    r3_platform_api = r3_memory_api->alloc(sizeof(R3_Platform_API), 16);
    if (!r3_platform_api) return R3_FALSE;    // error: out of memory!

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    internal_platform_state.clock.frequency = 1.0 / (f64)frequency.QuadPart;    // store seconds per tick

    r3_platform_api->get_state = _windows_get_state_impl;
    r3_platform_api->cleanup = _windows_cleanup_impl;
    
    r3_platform_api->set_clock = _set_clock_impl;
    r3_platform_api->sleep = _windows_sleep_impl;
    r3_platform_api->get_time = _windows_get_time_impl;
    r3_platform_api->update_clock = _update_clock_impl;
    
    r3_platform_api->load_library = _windows_load_library_impl;
    r3_platform_api->get_library_symbol = _windows_get_library_symbol_impl;
    r3_platform_api->unload_library = _windows_unload_library_impl;
    
    r3_platform_api->poll_events = _windows_poll_events_impl;
    r3_platform_api->poll_inputs = _windows_poll_inputs_impl;
    
    r3_platform_api->create_window = _windows_create_window_impl;
    r3_platform_api->destroy_window = _windows_destroy_window_impl;

    r3_platform_api->get_window_flag = _get_window_flag_impl;
    r3_platform_api->set_window_flag = _set_window_flag_impl;
    r3_platform_api->rem_window_flag = _rem_window_flag_impl;

    r3_platform_api->show_cursor = _windows_show_cursor_impl;
    r3_platform_api->hide_cursor = _windows_hide_cursor_impl;
    r3_platform_api->bind_cursor = _windows_bind_cursor_impl;
    r3_platform_api->unbind_cursor = _windows_unbind_cursor_impl;
    r3_platform_api->center_cursor = _windows_center_cursor_impl;
    r3_platform_api->decenter_cursor = _windows_decenter_cursor_impl;

    r3_platform_api->create_gl_context = _windows_create_gl_context_impl;
    r3_platform_api->get_gl_context = _windows_get_gl_context_impl;
    r3_platform_api->swap_buffers = _windows_swap_buffers_impl;
    r3_platform_api->destroy_gl_context = _windows_destroy_gl_context_impl;

    return R3_TRUE;
}

void r3_shutdown_platform(void) {
    r3_platform_api->destroy_gl_context(internal_platform_state.window);
    r3_platform_api->destroy_window(internal_platform_state.window);
    r3_platform_api->cleanup();
    r3_memory_api->free(r3_platform_api);
}
