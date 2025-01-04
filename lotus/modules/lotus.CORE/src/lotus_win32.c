#include "../include/platform/lotus_logger.h"
#include "../include/platform/lotus_platform.h"

#if defined(LOTUS_PLATFORM_WINDOWS)

#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

typedef struct Platform_Window_Data {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
} Platform_Window_Data;

static Lotus_Platform_API internal_platform_api = {0};
static Lotus_Platform_State internal_platform_state = {0};

void _windows_shutdown_impl(void) {
    UnregisterClass("Lotus Window Class", GetModuleHandle(NULL));
    lotus_exit_event();
    lotus_exit_input();
}

LRESULT CALLBACK _window_proc(HWND handle, ubyte4 msg, WPARAM w, LPARAM l) {
    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY:
            lotus_push_event((Lotus_Event){.event_data.ubyte[0]=1}, LOTUS_EVENT_APP_QUIT);
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            RECT r;
            GetClientRect(handle, &r);
            ubyte2 w = r.right - r.left;
            ubyte2 h = r.bottom - r.top;
            // TODO: handle resize event with resize callback
            lotus_push_event((Lotus_Event){.event_data.ubyte2[0]=w, .event_data.ubyte2[1]=h}, LOTUS_EVENT_RESIZE);
        } break;
        case WM_KEYUP:          // fall trough WM_SYSKEYUP
        case WM_KEYDOWN:        // fall trough WM_SYSKEYUP
        case WM_SYSKEYDOWN:     // fall trough WM_SYSKEYUP
        case WM_SYSKEYUP: {
            // key pressed/released
            Lotus_Keyboard_Key key = (ubyte2)w;
            ubyte pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            lotus_process_key_input(key, pressed);
        } break;
        case WM_MOUSEMOVE: {
            // mouse movement
            sbyte4 x = GET_X_LPARAM(l); 
            sbyte4 y = GET_Y_LPARAM(l);
            lotus_process_mouse_move_input(x, y);
        } break;
        case WM_MOUSEWHEEL: {
            sbyte4 z = GET_WHEEL_DELTA_WPARAM(w);
            if (z != 0) {
                // clamp input to OS-independent values (-1, 1)
                z = (z < 0) ? -1 : 1;
                lotus_process_mouse_wheel_input(z);
            }
        } break;
        case WM_LBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_MBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_RBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_LBUTTONUP:      // fall trough WM_RBUTTONUP
        case WM_MBUTTONUP:      // fall trough WM_RBUTTONUP
        case WM_RBUTTONUP: {
            Lotus_Mouse_Button button = LOTUS_MBUTTON_MAX_BUTTONS;
            ubyte pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = LOTUS_MBUTTON_LEFT;
                    break;
                
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = LOTUS_MBUTTON_MIDDLE;
                    break;
                
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = LOTUS_MBUTTON_RIGHT;
                    break;
            }
            if (button != LOTUS_MBUTTON_MAX_BUTTONS) lotus_process_mouse_button_input(button, pressed);
        } break;
    }
    
    return DefWindowProcA(handle, msg, w, l);
}

Lotus_Window _windows_create_window_impl(const char* title, int width, int height) {
    Lotus_Window window;

    window.internal_data = (Platform_Window_Data*)malloc(sizeof(Platform_Window_Data));
    if (!window.internal_data) {
        return (Lotus_Window){0};
    }
    Platform_Window_Data* data = (Platform_Window_Data*)window.internal_data;

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = _window_proc;
    wc.hInstance = data->instance;
    wc.lpszClassName = "Lotus Window Class";

    if (!RegisterClass(&wc)) {
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
        NULL,
        NULL,
        data->instance,
        NULL);

    if (!hwnd) {
        return (Lotus_Window){0};
    }

    data->handle = hwnd;
    data->gl_context = NULL;
    data->device_context = GetDC(hwnd);

    // Initialize the generic window structure
    strncpy(window.title, title, sizeof(window.title) - 1);
    window.title[sizeof(window.title) - 1] = '\0';
    window.location[0] = CW_USEDEFAULT;
    window.location[1] = CW_USEDEFAULT;
    window.size[0] = width;
    window.size[1] = height;
    window.aspect_ratio = (float)width / height;

    return window;
}

void _windows_destroy_window_impl(Lotus_Window* window) {
    if (window && window->internal_data) {
        Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
        if (data->handle) {
            DestroyWindow(data->handle);
        }
        free(data);
        window->internal_data = NULL;
    }
}

ubyte _windows_create_gl_context_impl(Lotus_Window* window) {
    if (!window || !window->internal_data) {
        lotus_log_error("window_ptr/window->internal_data_ptr invalid!");
        return LOTUS_FALSE;
    }

    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;

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
        lotus_log_error("failed to set pixel format");
        return LOTUS_FALSE;
    }

    data->gl_context = wglCreateContext(data->device_context);
    if (!data->gl_context) {
        lotus_log_error("failed to create gl context");
        return LOTUS_FALSE;
    }

    if (!wglMakeCurrent(data->device_context, data->gl_context)) {
        lotus_log_error("failed to bind gl context");
        wglDeleteContext(data->gl_context);
        return LOTUS_FALSE;
    }

    return LOTUS_TRUE;
}

void _windows_destroy_gl_context_impl(Lotus_Window* window) {
    if (!window || !window->internal_data) return;

    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
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
    Sleep((DWORD)(seconds * 1000.0));
}

void _windows_swap_buffers_impl(Lotus_Window* window) {
    if (!window || !window->internal_data) return;

    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
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

void* _windows_get_gl_context_impl(Lotus_Window* window) {
    if (!window || !window->internal_data) {
        lotus_log_error("Invalid window or internal data!");
        return NULL;
    }

    Platform_Window_Data* data = (Platform_Window_Data*)window->internal_data;
    return (void*)data->gl_context;
}

Lotus_Platform_State* _windows_get_state_impl(void) { return &internal_platform_state; }

Lotus_DyLib _windows_load_library_impl(const char* path, const char* name) {
    char full_path[MAX_PATH];
    snprintf(full_path, sizeof(full_path), "%s/%s.dll", path, name);

    HMODULE handle = LoadLibrary(full_path);
    if (!handle) {
        lotus_log_error("Failed to load library: %s", full_path);
        return (Lotus_DyLib){.name = NULL, .handle = NULL};
    }

    Lotus_DyLib lib = {.name = strdup(name), .handle = handle};
    return lib;
}

void* _windows_get_library_symbol_impl(Lotus_DyLib* library, const char* symbol_name) {
    if (!library || !library->handle || !symbol_name) {
        lotus_log_error("Invalid library or symbol name");
        return NULL;
    }
    return (void*)GetProcAddress((HMODULE)library->handle, symbol_name);
}

ubyte _windows_unload_library_impl(Lotus_DyLib* library) {
    if (!library || !library->handle) return LOTUS_FALSE;

    if (!FreeLibrary((HMODULE)library->handle)) {
        lotus_log_error("Failed to unload library: %s", library->name);
        return LOTUS_FALSE;
    }

    free((char*)library->name);
    library->handle = NULL;
    return LOTUS_TRUE;
}

Lotus_Platform_API* lotus_init_platform(void) {
    internal_platform_state.platform = LOTUS_WINDOWS_TAG;
    
    internal_platform_state.event_state = lotus_init_event();
    internal_platform_state.input_state = lotus_init_input();

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    internal_platform_state.clock_frequency = 1.0 / (f64)frequency.QuadPart;

    internal_platform_api = (Lotus_Platform_API) {
        .get_state = _windows_get_state_impl,
        .shutdown = _windows_shutdown_impl,
        .get_time = _windows_get_time_impl,
        .sleep = _windows_sleep_impl,
        
        .load_library = _windows_load_library_impl,
        .get_library_symbol = _windows_get_library_symbol_impl,
        .unload_library = _windows_unload_library_impl,
        
        .poll_events = _windows_poll_events_impl,
        
        .create_window = _windows_create_window_impl,
        .destroy_window = _windows_destroy_window_impl,
        
        .create_gl_context = _windows_create_gl_context_impl,
        .get_gl_context = _windows_get_gl_context_impl,
        .swap_buffers = _windows_swap_buffers_impl,
        .destroy_gl_context = _windows_destroy_gl_context_impl
    };

    return &internal_platform_api;
}

#endif // LOTUS_PLATFORM_WINDOWS == LOTUS_TRUE