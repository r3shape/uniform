#include "ltplatform.h"

#if LOTUS_PLATFORM_WINDOWS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <windowsx.h>   // param input extraction

#include <gl/GL.h>

static f64 clockFrequency;
static LARGE_INTEGER startTime;

typedef struct tagPlatformInternal {
    HWND hwnd;
    HINSTANCE instance;
    HDC deviceContext;
    HGLRC glContext;
} LTplatformInternal;

LRESULT CALLBACK ltWindowProcess(HWND hwnd, u32 msg, WPARAM w, LPARAM l);

b8 ltPlatformInit(LTplatformState* state, const char* application, i32 x, i32 y, i32 w, i32 h) {
    state->internal = malloc(sizeof(LTplatformInternal));
    LTplatformInternal* internalState = (LTplatformInternal*)state->internal;

    internalState->instance = GetModuleHandleA(0);  // get handle to current application

    // populate and register window
    HICON icon = LoadIcon(internalState->instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = ltWindowProcess;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = internalState->instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "Eclipse Window";

    if(!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window Registration Failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return LOTUS_FALSE;
    }

    u32 clientX = x;
    u32 clientY = y;
    u32 clientW = w;
    u32 clientH = h;
    
    u32 windowX = clientX;
    u32 windowY = clientY;
    u32 windowW = clientW;
    u32 windowH = clientH;

    u32 windowStyle = WS_TILED | WS_SYSMENU | WS_CAPTION;
    u32 windowExStyle = WS_EX_APPWINDOW;

    windowStyle |= WS_MAXIMIZEBOX;
    windowStyle |= WS_MINIMIZEBOX;
    windowStyle |= WS_THICKFRAME;

    RECT windowBorder = {0, 0, 0, 0};
    AdjustWindowRectEx(&windowBorder, windowStyle, 0, windowExStyle);

    windowX += windowBorder.left;
    windowY += windowBorder.top;

    windowW += windowBorder.right - windowBorder.left;
    windowH += windowBorder.bottom - windowBorder.top;

    HWND handle = CreateWindowExA(
        windowExStyle, "Eclipse Window", application,
        windowStyle, x, y, w, h, 0, 0, internalState->instance, 0
    );

    if (handle == 0) {
        printf("Window Creation Failed\n");
        return LOTUS_FALSE;
    } else { 
        internalState->hwnd = handle;
        state->windowPtr = &internalState->hwnd;
    }

    // get current device context and define a pixel format
    internalState->deviceContext = GetDC(internalState->hwnd);
    PIXELFORMATDESCRIPTOR pfd = { 
        sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
        1,                              // Version
        PFD_DRAW_TO_WINDOW |            // Support window rendering
        PFD_SUPPORT_OPENGL |            // Support OpenGL rendering
        PFD_DOUBLEBUFFER,               // Support double buffering
        PFD_TYPE_RGBA,                  // RGBA pixel format
        32,                             // 32-bit color depth
        0, 0, 0, 0, 0, 0,               // Color bits ignored
        0,                              // No alpha buffer
        0,                              // Shift bit ignored
        0,                              // No accumulation buffer
        0, 0, 0, 0,                     // Accumulation bits ignored
        24,                             // 24-bit z-buffer
        8,                              // 8-bit stencil buffer
        0,                              // No auxiliary buffer
        PFD_MAIN_PLANE,                 // Main drawing layer
        0,                              // Reserved
        0, 0, 0                         // Layer masks ignored
    };
    int pixelFormat = ChoosePixelFormat(internalState->deviceContext, &pfd);
    if (!SetPixelFormat(internalState->deviceContext, pixelFormat, &pfd)) {
        MessageBoxA(0, "Failed to set pixel format.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return LOTUS_FALSE;
    }

    // create and activate OpenGL context
    internalState->glContext = wglCreateContext(internalState->deviceContext);
    if (!wglMakeCurrent(internalState->deviceContext, internalState->glContext)) {
        MessageBoxA(0, "Failed to activate OpenGL context.", "Error", MB_ICONEXCLAMATION | MB_OK);
        return LOTUS_FALSE;
    }

    // show the window
    b32 shouldActivate = 1;
    i32 showCommandFlags = shouldActivate ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(internalState->hwnd, showCommandFlags);

    // clock setup
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clockFrequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&startTime);

    return LOTUS_TRUE;
}

void ltPlatformExit(LTplatformState* state) {
    LTplatformInternal* internalState = (LTplatformInternal*)state->internal;

    if (internalState->hwnd) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(internalState->glContext);
        ReleaseDC(internalState->hwnd, internalState->deviceContext);
    }

    free (state->internal);
}

b8 ltPlatformPump(LTplatformState* state) {
    MSG message;
    while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    return LOTUS_TRUE;
}

void* ltMemAlloc(u64 size, b8 aligned) {
    return malloc(size);
}

void* ltMemZero(void* block, u64 size) {
    return memset(block, 0, size);
}

void ltMemFree(void* block, b8 aligned) {
    free(block);
}

void* ltMemSet(void* block, i32 value, u64 size) {
    return memset(block, value, size);
}

void* ltMemCopy(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}

void ltConsoleWrite(const char* message, u8 color) {
    // levels: fatal, error, warning, info, debug, trace
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, levels[color]);

    LPDWORD written = 0;
    u64 len = strlen(message);
    OutputDebugStringA(message);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)len, written, 0);
}

void ltConsoleWriteError(const char* message, u8 color) {
    // levels: fatal, error, warning, info, debug, trace
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    HANDLE console = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(console, levels[color]);

    LPDWORD written = 0;
    u64 len = strlen(message);
    OutputDebugStringA(message);
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)len, written, 0);
}

f64 ltPlatformGetTime(void) {
    LARGE_INTEGER nowTime;
    QueryPerformanceCounter(&nowTime);
    return (f64)nowTime.QuadPart * clockFrequency;
}

void ltPlatformSleep(u64 ms) {
    Sleep(ms);
}

LRESULT CALLBACK ltWindowProcess(HWND hwnd, u32 msg, WPARAM w, LPARAM l) {
    switch(msg) {
        case WM_ERASEBKGND: return 1;
        case WM_CLOSE:
            // TODO: emit quit event
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            RECT r;
            GetClientRect(hwnd, &r);
            u32 w = r.right - r.left;
            u32 h = r.bottom - r.top;

            // TODO emit resize event
        } break;
        case WM_KEYUP:          // fall trough WM_SYSKEYUP
        case WM_KEYDOWN:        // fall trough WM_SYSKEYUP
        case WM_SYSKEYDOWN:     // fall trough WM_SYSKEYUP
        case WM_SYSKEYUP: {
            // key pressed/released
            // b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            // TODO: input processing
            
        } break;
        case WM_MOUSEMOVE: {
            // mouse movement
            // i32 x, y = GET_X_LPARAM(l); GET_Y_LPARAM(l);
            // TODO: input processing
        } break;
        case WM_MOUSEWHEEL: {
            // i32 z = GET_WHEEL_DELTA_WPARAM(w);
            // if (z != 0) {
            //     // clamp input to (-1, 1)
            //     z = (z < 0) ? -1 : 1;
            //     // TODO: input processing
            // }
        } break;
        case WM_LBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_MBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_RBUTTONDOWN:    // fall trough WM_RBUTTONUP
        case WM_LBUTTONUP:      // fall trough WM_RBUTTONUP
        case WM_MBUTTONUP:      // fall trough WM_RBUTTONUP
        case WM_RBUTTONUP: {
            // b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            // TODO: input processing
        } break;
    }
    
    return DefWindowProcA(hwnd, msg, w, l);
}

void ltPlatformSwapBuffers(LTplatformState* state) {
    LTplatformInternal* internalState = (LTplatformInternal*)state->internal;
    SwapBuffers(GetDC(internalState->hwnd));
}

#endif // LT_PLATFORM_WINDOWS