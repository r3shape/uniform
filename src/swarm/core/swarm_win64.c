#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/swarm_renderer.h>
#include <include/swarm/core/swarm_events.h>
#include <include/swarm/core/swarm_inputs.h>

#if defined(SWARM_PLATFORM_WINDOWS)
#include <stdio.h>
#include <Windows.h>
#include <windowsx.h>   // parameter input extraction

static struct _WindowsInternal {
    HWND handle;
    HGLRC glContext;
    HDC deviceContext;
    GPUBackend backend;
    HINSTANCE instance;
    RAWINPUTDEVICE rid;
    SwarmWindow window;
} _WindowsInternal = {0};


LRESULT CALLBACK _windowProc(HWND handle, u32 msg, WPARAM wParam, LPARAM lParam) {
    if (!handle) return DefWindowProcA(handle, msg, wParam, lParam);    // error: how did you get here?

    RECT window_rect;
    GetWindowRect(_WindowsInternal.handle, &window_rect);

    // only handle window flags during focus
    if (swarmPlatform->getWindowFlag(WINDOW_FOCUSED)) {
        ShowCursor(swarmPlatform->getWindowFlag(WINDOW_SHOW_CURSOR));
        if (swarmPlatform->getWindowFlag(WINDOW_BIND_CURSOR))
            ClipCursor(&window_rect);
        if (swarmPlatform->getWindowFlag(WINDOW_CENTER_CURSOR))
            SetCursorPos((window_rect.left + window_rect.right) / 2, (window_rect.top + window_rect.bottom) / 2);
    }

    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY:
            swarmEvents->pushEvent(SWARM_EVENT_QUIT, (SwarmEvent){.u8[0]=1}),
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:       // fall through to WMEXITSIZEMOVE
        case WM_MOVE:       // fall through to WMEXITSIZEMOVE
        case WM_EXITSIZEMOVE: {
            RECT newRect = {0};
            GetWindowRect(handle, &newRect);
            _WindowsInternal.window.size.x = newRect.right;
            _WindowsInternal.window.size.y = newRect.bottom;
            _WindowsInternal.window.location.y = newRect.top;
            _WindowsInternal.window.location.x = newRect.left;
            
            if ((_WindowsInternal.window.flags & WINDOW_BIND_CURSOR) == WINDOW_BIND_CURSOR) ClipCursor(&newRect);
            
            u16 width = newRect.right - newRect.left;
            u16 height = newRect.bottom - newRect.top;
            swarmEvents->pushEvent(SWARM_EVENT_RESIZE, (SwarmEvent){.u16[0]=width, .u16[1]=height});
        } break;
        case WM_KILLFOCUS: {
            swarmPlatform->remWindowFlag(WINDOW_FOCUSED);
            ClipCursor(NULL);
        } break;
        case WM_SETFOCUS: {
            swarmPlatform->setWindowFlag(WINDOW_FOCUSED);
            if ((_WindowsInternal.window.flags & WINDOW_BIND_CURSOR) == WINDOW_BIND_CURSOR) {
                RECT newRect = {0};
                GetWindowRect(handle, &newRect);
                _WindowsInternal.window.location.x = newRect.left;
                _WindowsInternal.window.location.y = newRect.top;
                _WindowsInternal.window.size.x = newRect.right;
                _WindowsInternal.window.size.y = newRect.bottom;
                ClipCursor(&newRect);
            }
        } break;
        /* LEGACY INPUT */
        case WM_KEYUP:          // fall through WM_SYSKEYUP
        case WM_KEYDOWN:        // fall through WM_SYSKEYUP
        case WM_SYSKEYDOWN:     // fall through WM_SYSKEYUP
        case WM_SYSKEYUP: {
            // key pressed/released
            SwarmKeyboardKey key = (u16)wParam;
            byte pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            swarmInputs->processKeyInput(key, pressed);
        } break;
        /* RAW INPUT */
        case WM_INPUT: {
            UINT dwSize;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            
            RAWINPUT raw;
            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
                saneLog->log(SANE_LOG_ERROR, "[Platform] GetRawInputData returned incorrect size");
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
                    swarmInputs->processMouseMoveInput(x, y);
                }

                // handle mouse buttons
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                    swarmInputs->processMouseButtonInput(SWARM_MBUTTON_LEFT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                    swarmInputs->processMouseButtonInput(SWARM_MBUTTON_LEFT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                    swarmInputs->processMouseButtonInput(SWARM_MBUTTON_RIGHT, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                    swarmInputs->processMouseButtonInput(SWARM_MBUTTON_RIGHT, 0);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                    swarmInputs->processMouseButtonInput(SWARM_MBUTTON_MIDDLE, 1);
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                    swarmInputs->processMouseButtonInput(SWARM_MBUTTON_MIDDLE, 0);
                }

                // handle mouse wheel
                if (mouse->usButtonFlags & RI_MOUSE_WHEEL) {
                    i32 z = (i32)((SHORT)mouse->usButtonData) / WHEEL_DELTA;
                    swarmInputs->processMouseWheelInput(z);
                }
            }
        } break;
    }
    return DefWindowProcA(handle, msg, wParam, lParam);
}

byte _createWindowImpl(cstr title, u32 width, u32 height) {
    if (_WindowsInternal.handle != NULL) return SSDK_TRUE;  // error: window already created!

    _WindowsInternal.window = (SwarmWindow){0};
    SwarmWindow* win = &_WindowsInternal.window;

    // Register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = _windowProc;
    wc.hInstance = _WindowsInternal.instance;
    wc.lpszClassName = "Swarm Window";
    
    if (!RegisterClass(&wc)) return SSDK_FALSE; // error: failed to register window class!

    // Create the window
    HWND hwnd = CreateWindow(
        "Swarm Window",
        title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,  // default x
        CW_USEDEFAULT,  // default y
        width,
        height,
        NULL,   // parent
        NULL,   // menu
        _WindowsInternal.instance,
        NULL);

    if (!hwnd) return SSDK_FALSE; // error: failed to create window class!

    _WindowsInternal.handle = hwnd;
    _WindowsInternal.glContext = NULL;
    _WindowsInternal.deviceContext = GetDC(hwnd);

    // Register rawinput devices
    _WindowsInternal.rid.usUsagePage = 0x01;       // HID_USAGE_PAGE_GENERIC
    _WindowsInternal.rid.usUsage = 0x02;           // HID_USAGE_GENERIC_MOUSE
    _WindowsInternal.rid.dwFlags = 0;              // RIDEV_NOLEGACY adds raw mouse and ignores legacy mouse messages
    _WindowsInternal.rid.hwndTarget = _WindowsInternal.handle;

    if (!RegisterRawInputDevices(&_WindowsInternal.rid, 1, sizeof(_WindowsInternal.rid))) return SSDK_FALSE;    // error: failed to register rawinput devices!

    // Initialize the window structure
    win->flags = 0;
    win->title = title;
    win->size.x = width;
    win->size.y = height;
    win->location.x = CW_USEDEFAULT;
    win->location.y = CW_USEDEFAULT;
    win->aspect_ratio = (f32)width / (f32)height;
    
    // Set default window flags
    swarmPlatform->setWindowFlag(WINDOW_FOCUSED);
    swarmPlatform->setWindowFlag(WINDOW_SHOW_CURSOR);

    // Initialize the renderer
    swarmRenderer->init(_WindowsInternal.backend);

    return SSDK_TRUE;
}

void _destroyWindowImpl(void) {
    if (!_WindowsInternal.handle) return;    // error: window not yet created!
    swarmRenderer->shutdown();
    DestroyWindow(_WindowsInternal.handle);
    if (_WindowsInternal.glContext) swarmPlatform->destroyGLContext();
}


byte _createGLContextImpl(void) {
    if (!_WindowsInternal.handle) return SSDK_FALSE;    // error: window not yet created!

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

    int format = ChoosePixelFormat(_WindowsInternal.deviceContext, &pfd);
    if (!SetPixelFormat(_WindowsInternal.deviceContext, format, &pfd)) {
        return SSDK_FALSE;  // error: failed to set pixel format!
    }

    _WindowsInternal.glContext = wglCreateContext(_WindowsInternal.deviceContext);
    if (!_WindowsInternal.glContext) {
        return SSDK_FALSE;  // error: failed to create GL context!
    }

    if (!wglMakeCurrent(_WindowsInternal.deviceContext, _WindowsInternal.glContext)) {
        wglDeleteContext(_WindowsInternal.glContext);
        return SSDK_FALSE;  // error: failed to make context current!
    }

    return SSDK_TRUE;
}

void _swapBuffersImpl(void) {
    if (!_WindowsInternal.handle) return;    // error: no window created!
    SwapBuffers(_WindowsInternal.deviceContext);
}

void _destroyGLContextImpl(void) {
    if (!_WindowsInternal.handle) return;    // error: no window created!

    if (_WindowsInternal.glContext) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(_WindowsInternal.glContext);
        _WindowsInternal.glContext = NULL;
    }
}


void _pollEventsImpl(void) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

void _pollInputsImpl(void) {
    swarmInputs->update();
}


SwarmLib _loadLibImpl(cstr path, cstr name) {
    HMODULE handle;
    char full_path[MAX_PATH];
    if (path != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s.dll", path, name);
    } else snprintf(full_path, sizeof(full_path), "%s.dll", name);
    
    handle = LoadLibrary(full_path);
    if (!handle) return (SwarmLib){.name = NULL, .handle = NULL}; // error: failed to load lib!

    SwarmLib lib = {.name = name, .handle = handle};
    return lib;
}

ptr _loadSymbolImpl(cstr name, SwarmLib* lib) {
    if (!lib || !lib->handle || !name) return NULL; // error: null ptr!
    ptr proc = wglGetProcAddress(name);
    if (proc == NULL || (proc == (ptr)0x1) || (proc == (ptr)0x2) || (proc == (ptr)0x3) || (proc == (ptr)-1)) {
        proc = GetProcAddress((HMODULE)lib->handle, name);
    }
    return proc;
}

byte _unloadLibImpl(SwarmLib* lib) {
    if (!lib || !lib->handle) return SSDK_FALSE;    // error: null ptr!
    if (!FreeLibrary((HMODULE)lib->handle)) return SSDK_FALSE;  // error: failed to free lib!
    lib->name = NULL;
    lib->handle = NULL;
    return SSDK_TRUE;
}


byte _getWindowFlagImpl(u32 flag) {
    if (!_WindowsInternal.handle) return SSDK_FALSE;    // error: window not yet created!
    return ((_WindowsInternal.window.flags & flag) == flag) ? SSDK_TRUE : SSDK_FALSE;
}

byte _setWindowFlagImpl(u32 flag) {
    if (!_WindowsInternal.handle) return SSDK_FALSE;    // error: window not yet created!
    _WindowsInternal.window.flags |= flag;
    return SSDK_TRUE;
}

byte _remWindowFlagImpl(u32 flag) {
    if (!_WindowsInternal.handle) return SSDK_FALSE;    // error: window not yet created!
    _WindowsInternal.window.flags &= ~flag;
    return SSDK_TRUE;
}


void _toggleVsyncImpl(byte toggle) {
    typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALPROC)(int);
    PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT) wglSwapIntervalEXT(toggle);
}


none _initPlatform(GPUBackend backend) {
    _WindowsInternal.backend = backend;
    saneLog->log(SANE_LOG_SUCCESS, "[Platform] Initialized");
}

none _shutdownPlatform(none) {
    swarmPlatform->destroyWindow();
    saneLog->log(SANE_LOG_SUCCESS, "[Platform] Shutdown");
}

SwarmPlatform* swarmPlatform = &(SwarmPlatform) {
    .init = _initPlatform,
    .shutdown = _shutdownPlatform,

    .createWindow = _createWindowImpl,
    .destroyWindow = _destroyWindowImpl,
    
    .swapBuffers = _swapBuffersImpl,
    .createGLContext = _createGLContextImpl,
    .destroyGLContext = _destroyGLContextImpl,

    .pollEvents = _pollEventsImpl,
    .pollInputs = _pollInputsImpl,
    
    .getWindowFlag = _getWindowFlagImpl,
    .setWindowFlag = _setWindowFlagImpl,
    .remWindowFlag = _remWindowFlagImpl,

    .loadLib = _loadLibImpl,
    .loadSymbol = _loadSymbolImpl,
    .unloadLib = _unloadLibImpl,
};

#endif  // SWARM_PLATFORM_WINDOWS
