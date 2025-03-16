#include "../../include/r3.platform.h"

#if defined(R3_PLATFORM_WINDOWS)
#include <stdio.h>
#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

static struct internal_platform_state {
    HWND handle;
    HGLRC gl_context;
    HDC device_context;
    HINSTANCE instance;
    RAWINPUTDEVICE rid;
    
    R3_Window* window;
    
    _r3_input_api* input_api;
    _r3_events_api* events_api;
} internal_platform_state = {0};

LRESULT CALLBACK _window_proc(HWND handle, u32 msg, WPARAM wParam, LPARAM lParam) {
    if (!internal_platform_state.window) return DefWindowProcA(handle, msg, wParam, lParam);    // error: how did you get here?

    RECT window_rect;
    GetWindowRect(internal_platform_state.handle, &window_rect);

    // only handle window flags during focus
    // if (internal_platform_state.window->focused) {
        // ShowCursor(r3_platform_api->get_window_flag(window, R3_SHOW_CURSOR));
        // if (r3_platform_api->get_window_flag(window, R3_BIND_CURSOR))
        //     ClipCursor(&window_rect);
        // if (r3_platform_api->get_window_flag(window, R3_CENTER_CURSOR))
        //     SetCursorPos((window_rect.left + window_rect.right) / 2, (window_rect.top + window_rect.bottom) / 2);
    // }

    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY:
            internal_platform_state.events_api->push_event(R3_EVENT_QUIT, (R3_Event){.u8[0]=1});
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:       // fall through to WMEXITSIZEMOVE
        case WM_MOVE:       // fall through to WMEXITSIZEMOVE
        case WM_EXITSIZEMOVE: {
            RECT newRect = {0};
            GetWindowRect(handle, &newRect);
            // internal_platform_state.window->size[0] = newRect.right;
            // internal_platform_state.window->size[1] = newRect.bottom;
            // internal_platform_state.window->location[1] = newRect.top;
            // internal_platform_state.window->location[0] = newRect.left;
            
            if ((internal_platform_state.window->flags & R3_BIND_CURSOR) == R3_BIND_CURSOR) ClipCursor(&newRect);
            
            u16 width = newRect.right - newRect.left;
            u16 height = newRect.bottom - newRect.top;
            internal_platform_state.events_api->push_event(R3_EVENT_RESIZE, (R3_Event){.u16[0]=0, .u16[1]=0});
        } break;
        case WM_KILLFOCUS: {
            internal_platform_state.window->focused = LIBX_FALSE;
            ClipCursor(NULL);
        } break;
        case WM_SETFOCUS: {
            internal_platform_state.window->focused = LIBX_TRUE;
            if ((internal_platform_state.window->flags & R3_BIND_CURSOR) == R3_BIND_CURSOR) {
                RECT newRect = {0};
                GetWindowRect(handle, &newRect);
                internal_platform_state.window->location[0] = newRect.left;
                internal_platform_state.window->location[1] = newRect.top;
                internal_platform_state.window->size[0] = newRect.right;
                internal_platform_state.window->size[1] = newRect.bottom;
                ClipCursor(&newRect);
            }
        } break;
        /* LEGACY INPUT */
        case WM_KEYUP:          // fall through WM_SYSKEYUP
        case WM_KEYDOWN:        // fall through WM_SYSKEYUP
        case WM_SYSKEYDOWN:     // fall through WM_SYSKEYUP
        case WM_SYSKEYUP: {
            // key pressed/released
            R3_Keyboard_Key key = (u16)wParam;
            u8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            internal_platform_state.input_api->_process_key_input(key, pressed);
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
                    i16 x = (i16)mouse->lLastX;
                    i16 y = (i16)mouse->lLastY;
                    internal_platform_state.input_api->_process_mouse_move_input(x, y);
                }

                // handle mouse buttons
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                    internal_platform_state.input_api->_process_mouse_button_input(R3_MBUTTON_LEFT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                    internal_platform_state.input_api->_process_mouse_button_input(R3_MBUTTON_LEFT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                    internal_platform_state.input_api->_process_mouse_button_input(R3_MBUTTON_RIGHT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                    internal_platform_state.input_api->_process_mouse_button_input(R3_MBUTTON_RIGHT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                    internal_platform_state.input_api->_process_mouse_button_input(R3_MBUTTON_MIDDLE, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                    internal_platform_state.input_api->_process_mouse_button_input(R3_MBUTTON_MIDDLE, 0);
                }

                // handle mouse wheel
                if (mouse->usButtonFlags & RI_MOUSE_WHEEL) {
                    i32 z = (i32)((SHORT)mouse->usButtonData) / WHEEL_DELTA;
                    internal_platform_state.input_api->_process_mouse_wheel_input(z);
                }
            }
        } break;
    }
    return DefWindowProcA(handle, msg, wParam, lParam);
}

R3_Window _create_window_impl(const char *title, int width, int height) {
    R3_Window win = {0};

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = _window_proc;
    wc.hInstance = internal_platform_state.instance;
    wc.lpszClassName = "R3 Window Class";
    
    if (!RegisterClass(&wc)) return win; // error: failed to register window class!

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
        internal_platform_state.instance,
        NULL);

    if (!hwnd) return win; // error: failed to create window class!

    internal_platform_state.handle = hwnd;
    internal_platform_state.gl_context = NULL;
    internal_platform_state.device_context = GetDC(hwnd);

    // Register rawinput devices
    internal_platform_state.rid.usUsagePage = 0x01;       // HID_USAGE_PAGE_GENERIC
    internal_platform_state.rid.usUsage = 0x02;           // HID_USAGE_GENERIC_MOUSE
    internal_platform_state.rid.dwFlags = 0;              // RIDEV_NOLEGACY adds raw mouse and ignores legacy mouse messages
    internal_platform_state.rid.hwndTarget = internal_platform_state.handle;

    if (!RegisterRawInputDevices(&internal_platform_state.rid, 1, sizeof(internal_platform_state.rid))) return win;    // error: failed to register rawinput devices!

    // Initialize the window structure
    strncpy(win.title, title, sizeof(win.title) - 1);
    win.focused = LIBX_TRUE;
    win.flags = 0;
    win.title[sizeof(win.title) - 1] = '\0';
    win.location[0] = CW_USEDEFAULT;
    win.location[1] = CW_USEDEFAULT;
    win.size[0] = width;
    win.size[1] = height;
    win.aspect_ratio = (float)width / height;
    
    internal_platform_state.window = &win;
    return win;
}

void _destroy_window_impl(R3_Window *window) {
    if (!window) return;    // error: null ptr!
    if (internal_platform_state.handle) DestroyWindow(internal_platform_state.handle);
    *window = (R3_Window){0};
}


u8 _create_gl_context_impl(void) {
    if (!internal_platform_state.window) return LIBX_FALSE;    // error: no window created!

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

    int format = ChoosePixelFormat(internal_platform_state.device_context, &pfd);
    if (!SetPixelFormat(internal_platform_state.device_context, format, &pfd)) {
        return LIBX_FALSE;  // error: failed to set pixel format!
    }

    internal_platform_state.gl_context = wglCreateContext(internal_platform_state.device_context);
    if (!internal_platform_state.gl_context) {
        return LIBX_FALSE;  // error: failed to create GL context!
    }

    if (!wglMakeCurrent(internal_platform_state.device_context, internal_platform_state.gl_context)) {
        wglDeleteContext(internal_platform_state.gl_context);
        return LIBX_FALSE;  // error: failed to make context current!
    }

    return LIBX_TRUE;
}

void _swap_buffers_impl(void) {
    if (!internal_platform_state.window) return;    // error: no window created!
    SwapBuffers(internal_platform_state.device_context);
}

void _destroy_gl_context_impl(void) {
    if (!internal_platform_state.window) return;    // error: no window created!

    if (internal_platform_state.gl_context) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(internal_platform_state.gl_context);
        internal_platform_state.gl_context = NULL;
    }
}


void _poll_events_impl(void) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void _poll_inputs_impl(void) {
    internal_platform_state.input_api->update();
}

#endif  // R3_PLATFORM_WINDOWS

u8 _r3_init_platform(_r3_events_api* events_api, _r3_input_api* input_api, _r3_platform_api* api) {
    if (!input_api || !events_api || !api) return LIBX_FALSE;    // error: null ptr!

    internal_platform_state.input_api = input_api;
    internal_platform_state.events_api = events_api;

    api->create_window = _create_window_impl;
    api->destroy_window = _destroy_window_impl;
    
    api->create_gl_context = _create_gl_context_impl;
    api->swap_buffers = _swap_buffers_impl;
    api->destroy_gl_context = _destroy_gl_context_impl;

    api->poll_events = _poll_events_impl;
    api->poll_inputs = _poll_inputs_impl;

    return LIBX_TRUE;
}

u8 _r3_cleanup_platform(_r3_platform_api* api) {
    if (!api) return LIBX_FALSE;    // error: null ptr!

    internal_platform_state.input_api = NULL;
    internal_platform_state.events_api = NULL;

    api->create_window = NULL;
    api->destroy_window = NULL;
    
    api->create_gl_context = NULL;
    api->destroy_gl_context = NULL;

    api->poll_events = NULL;
    api->poll_inputs = NULL;
    
    return LIBX_TRUE;
}
