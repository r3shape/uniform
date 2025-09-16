#include "ufcore/UFDEF.h"
#include <r3kit/include/io/log.h>
#include <ufcore/UFEVIN.h>
#include <ufcore/UFOS.h>

#ifdef _UF_PLATFORM_WINDOWS_

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>   // for parameter input extraction

static struct UFW64 {
    UFEVINInterface* evinPtr;   // passed from runtime on call to `ufInitOS`
    
    UFResource keyboardDefault; // default keyboard device (registered to UFEVIN on init)
    UFResource mouseDefault;    // default mouse device (registered to UFEVIN on init)

    u8 windows;
    struct W64Window {
        u32 winX; u32 winY;
        u32 winW; u32 winH;
        UFGpuApi api;

        HWND winHandle;
        HGLRC winGpuContext;
        HDC winDeviceContext;
        HINSTANCE winInstance;
        RAWINPUTDEVICE winRID;
    } W64Window[10];
} UFW64 = {0};

// internal window process (used to dispatch/process UFEVIN data)
LRESULT CALLBACK _ufWindowProc(HWND handle, u32 msg, WPARAM wp, LPARAM lp) {
    if (!handle) return DefWindowProcA(handle, msg, wp, lp);

    switch(msg) {
        case WM_ERASEBKGND: { return 0; }

        case WM_QUIT:       // fall through WM_DESTROY
        case WM_CLOSE:      // fall through WM_DESTROY
        case WM_DESTROY: {
                UFW64.evinPtr->sendEvent(UF_EVENT_EXIT, (UFEvent){ .u8[0]=1 });
                PostQuitMessage(0);
                return 0;
            }
        
        case WM_SIZE:       // fall through to WMEXITSIZEMOVE
        case WM_MOVE:       // fall through to WMEXITSIZEMOVE
        case WM_EXITSIZEMOVE: {
                RECT newRect = {0};
                GetWindowRect(handle, &newRect);

                u16 w = newRect.right - newRect.left;
                u16 h = newRect.bottom - newRect.top;
                UFW64.evinPtr->sendEvent(UF_EVENT_RESIZE, (UFEvent){ .u16[0]=w, .u16[1]=h });
                return 0;
            }
        
        case WM_SETFOCUS: break;
        case WM_KILLFOCUS: break;
        
        /* LEGACY INPUT */
        case WM_KEYUP:          // fall through WM_SYSKEYDOWN
        case WM_KEYDOWN:        // fall through WM_SYSKEYDOWN
        case WM_SYSKEYUP:       // fall through WM_SYSKEYDOWN
        case WM_SYSKEYDOWN: {
                UFKeyboardKey key = (u16)wp;
                u8 event = (msg == WM_KEYUP || msg == WM_SYSKEYUP) ? UF_EVENT_KEY_RELEASED : UF_EVENT_KEY_PRESSED;
                UFW64.evinPtr->sendEvent(event, (UFEvent){ .key=key, .device=UFW64.keyboardDefault });
                return 0;
            }

        /* RAW INPUT */
        case WM_INPUT: {
            UINT dwSize;
            GetRawInputData((HRAWINPUT)lp, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            
            RAWINPUT raw;
            if (GetRawInputData((HRAWINPUT)lp, RID_INPUT, &raw, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
                r3_log_stdout(WARN_LOG, "[UFOS] `GetRawInputData` returned incorrect size\n");
                return DefWindowProcA(handle, msg, wp, lp);
            }

            if (raw.header.dwType == RIM_TYPEMOUSE) {
                RAWMOUSE *mouse = &raw.data.mouse;

                // handle mouse movement
                if (mouse->usFlags & MOUSE_MOVE_ABSOLUTE) {
                    // ignore absolute movement (used for remote desktop, etc.)
                } else {
                    s16 dx = (s16)mouse->lLastX;
                    s16 dy = (s16)mouse->lLastY;
                    UFW64.evinPtr->updateDeviceDelta(dx, dy, UFW64.mouseDefault);
                }

                // handle mouse buttons
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                    UFW64.evinPtr->setMouseState(UF_DEVICE_STATE_ACTIVE, UF_MOUSE_LEFT, UF_DEVICE_BUFFER_NOW, UFW64.mouseDefault);
                    UFW64.evinPtr->sendEvent(UF_EVENT_BUTTON_PRESSED, (UFEvent){ .button=UF_MOUSE_LEFT, .device=UFW64.mouseDefault});
                }
                if (mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                    UFW64.evinPtr->setMouseState(UF_DEVICE_STATE_INACTIVE, UF_MOUSE_LEFT, UF_DEVICE_BUFFER_NOW, UFW64.mouseDefault);
                    UFW64.evinPtr->sendEvent(UF_EVENT_BUTTON_RELEASED, (UFEvent){ .button=UF_MOUSE_LEFT, .device=UFW64.mouseDefault});
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                    UFW64.evinPtr->setMouseState(UF_DEVICE_STATE_ACTIVE, UF_MOUSE_RIGHT, UF_DEVICE_BUFFER_NOW, UFW64.mouseDefault);
                    UFW64.evinPtr->sendEvent(UF_EVENT_BUTTON_PRESSED, (UFEvent){ .button=UF_MOUSE_RIGHT, .device=UFW64.mouseDefault});
                }
                if (mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                    UFW64.evinPtr->setMouseState(UF_DEVICE_STATE_INACTIVE, UF_MOUSE_RIGHT, UF_DEVICE_BUFFER_NOW, UFW64.mouseDefault);
                    UFW64.evinPtr->sendEvent(UF_EVENT_BUTTON_RELEASED, (UFEvent){ .button=UF_MOUSE_RIGHT, .device=UFW64.mouseDefault});
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                    UFW64.evinPtr->setMouseState(UF_DEVICE_STATE_ACTIVE, UF_MOUSE_MIDDLE, UF_DEVICE_BUFFER_NOW, UFW64.mouseDefault);
                    UFW64.evinPtr->sendEvent(UF_EVENT_BUTTON_PRESSED, (UFEvent){ .button=UF_MOUSE_MIDDLE, .device=UFW64.mouseDefault});
                }
                if (mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                    UFW64.evinPtr->setMouseState(UF_DEVICE_STATE_INACTIVE, UF_MOUSE_MIDDLE, UF_DEVICE_BUFFER_NOW, UFW64.mouseDefault);
                    UFW64.evinPtr->sendEvent(UF_EVENT_BUTTON_RELEASED, (UFEvent){ .button=UF_MOUSE_MIDDLE, .device=UFW64.mouseDefault});
                }

                // handle mouse wheel
                if (mouse->usButtonFlags & RI_MOUSE_WHEEL) {
                    s32 z = (s32)((SHORT)mouse->usButtonData) / WHEEL_DELTA;
                    UFW64.evinPtr->sendEvent(UF_EVENT_MOUSE_WHEEL, (UFEvent){ .s8[0] = z, .device=UFW64.mouseDefault});
                }
            } return 0;
        } break;
    }

    return DefWindowProcA(handle, msg, wp, lp);
}

UFResource newWindow(u32 x, u32 y, u32 w, u32 h, UFGpuApi api) {
    u8 windows = UFW64.windows;
    if (windows == 5) {
        r3_log_stdout(ERROR_LOG, "[UFOS] Failed `newWindow` -- window maximum reached\n");
        return 0;
    }

    struct W64Window* winPtr = &UFW64.W64Window[windows];

    // Register window class
    if (!RegisterClass(&(WNDCLASS) {
            .lpszClassName = "UFWindow",
            .lpfnWndProc = _ufWindowProc,
            .hInstance = winPtr->winInstance
        })) {
        r3_log_stdoutf(ERROR_LOG, "[UFOS] failed to register window class (err=%lu)\n", GetLastError());
        return 0;
    }

    winPtr->winHandle = CreateWindow(
        "UFWindow", "Uniform Engine",   // class name, title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        x, y,
        w, h,
        NULL,   // parent
        NULL,   // menu
        winPtr->winInstance,
        NULL
    );

    if (!winPtr->winHandle) {
        r3_log_stdout(ERROR_LOG, "[UFOS] Failed `newWindow` -- window class registration failed\n");
        return 0;
    }

    winPtr->winGpuContext = NULL;
    winPtr->winDeviceContext = GetDC(winPtr->winHandle);

    // register default mouse as RID
    winPtr->winRID.usUsagePage = 0x01;       // HID_USAGE_PAGE_GENERIC
    winPtr->winRID.usUsage = 0x02;           // HID_USAGE_GENERIC_MOUSE
    winPtr->winRID.dwFlags = 0;              // RIDEV_NOLEGACY adds raw mouse and ignores legacy mouse messages
    winPtr->winRID.hwndTarget = winPtr->winHandle;

    if (!RegisterRawInputDevices(&winPtr->winRID, 1, sizeof(winPtr->winRID))) {
        r3_log_stdout(ERROR_LOG, "[UFOS] failed `newWindow` -- RID registeration failed\n");
        return 0;
    }

    // populate window structure
    winPtr->winX = x;
    winPtr->winY = y;
    winPtr->winW = w;
    winPtr->winH = h;
    winPtr->api = api;

    // increment then assign
    UFResource window = ++UFW64.windows;

    return window;
}

none setWindowFlag(UFWindowFlag flag) {}
none getWindowFlag(UFWindowFlag flag) {}
none remWindowFlag(UFWindowFlag flag) {}


none getEvents(none) {
    MSG msg;
    do {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    } while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE));
}

none getInputs(none) { UFW64.evinPtr->updateDevices(); }


none swapBuffers(UFResource window) {
    if (!window || window >= 5) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed `swapBuffers` -- window not found\n");
        return;
    }

    struct W64Window* winPtr = &UFW64.W64Window[window - 1];
    if (!winPtr->winDeviceContext) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed `swapBuffers` -- window device context NULL\n");
        return;
    }
    SwapBuffers(winPtr->winDeviceContext);
}

u8 newGpuCtx(UFResource window) {
    if (!window || window >= 5) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed `newGpuCtx` -- window not found\n");
        return 0;
    }

    struct W64Window* winPtr = &UFW64.W64Window[window - 1];
    if (winPtr->winGpuContext != NULL) {
        r3_log_stdout(WARN_LOG, "[UFOS] Skippng call to `newGpuCtx` -- context exists\n");
        return 1;
    }

    switch (winPtr->api) {
        case (UF_GPU_GL_API): {
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

            int format = ChoosePixelFormat(winPtr->winDeviceContext, &pfd);
            if (!SetPixelFormat(winPtr->winDeviceContext, format, &pfd)) {
                return 0;  // error: failed to set pixel format!
            }

            winPtr->winGpuContext = wglCreateContext(winPtr->winDeviceContext);
            if (!winPtr->winGpuContext) {
                return 0;  // error: failed to create GL context!
            }

            if (!wglMakeCurrent(winPtr->winDeviceContext, winPtr->winGpuContext)) {
                wglDeleteContext(winPtr->winGpuContext);
                return 0;  // error: failed to make context current!
            }

            return 1;
        }
        case (UF_GPU_MT_API):       // fall through default
        case (UF_GPU_VK_API):       // fall through default
        case (UF_GPU_DX_API):       // fall through default
        case (UF_GPU_INVALID_API):  // fall through default
        default: {
            r3_log_stdoutf(WARN_LOG, "[UFOS] Failed `newGpuCtx` -- unsupported api: %d\n", winPtr->api);
            return 0;
        }
    }
}

u8 delGpuCtx(UFResource window) {
    if (!window || window >= 5) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed `delGpuCtx` -- window not found\n");
        return 0;
    }

    struct W64Window* winPtr = &UFW64.W64Window[window - 1];
    if (!winPtr->winGpuContext) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed `delGpuCtx` -- window gpu context NULL\n");
        return 1;
    }

    switch (winPtr->api) {
        case (UF_GPU_GL_API): {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(winPtr->winGpuContext);
        } break;
        case (UF_GPU_MT_API):       // fall through default
        case (UF_GPU_VK_API):       // fall through default
        case (UF_GPU_DX_API):       // fall through default
        case (UF_GPU_INVALID_API):  // fall through default
        default: {
            r3_log_stdoutf(WARN_LOG, "[UFOS] Failed `delGpuCtx` -- unsupported api: %d\n", winPtr->api);
            return 0;
        }
    }

    winPtr->winGpuContext = NULL;
    return 1;
}

u8 delWindow(UFResource window) {
    if (!window || window >= 5) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed `delWindow` -- window not found\n");
        return 0;
    }

    struct W64Window* winPtr = &UFW64.W64Window[window - 1];
    
    if (winPtr->winGpuContext != NULL) delGpuCtx(window);
    DestroyWindow(winPtr->winHandle);

    return 1;
}


u8 loadLibrary(CString path, CString name, UFLibrary* library) {
    char full_path[MAX_PATH];
    if (path != NULL) {
        // TODO: levarage r3kit strings instead of snprintf!!!
        snprintf(full_path, sizeof(full_path), "%s/%s.dll", path, name);
    } else snprintf(full_path, sizeof(full_path), "%s.dll", name);
    
    library->handle = LoadLibrary(full_path);
    if (!library->handle) {
        r3_log_stdoutf(ERROR_LOG, "[UFOS] Failed `loadLibrary` -- %s\n", name);
        return 0;
    } else r3_log_stdoutf(SUCCESS_LOG, "[UFOS] Loaded Library -- %s\n", name);

    library->name = name;

    return 1;

}

u8 loadSymbol(char* name, ptr* symbol, UFLibrary* library) {
    if (!library || !library->handle || !name) {
        r3_log_stdoutf(ERROR_LOG, "[UFOS] Failed `loadSymbol` -- %s\n", name);
        return 0; // error: null ptr!
    }

    *symbol = wglGetProcAddress(name);
    if ((*symbol == NULL)     ||
        (*symbol == (ptr)0x1) ||
        (*symbol == (ptr)0x2) ||
        (*symbol == (ptr)0x3) ||
        (*symbol == (ptr)-1)) {
        *symbol = GetProcAddress((HMODULE)library->handle, name);
    }

    if (*symbol == NULL) {
        r3_log_stdoutf(ERROR_LOG, "[UFOS] Failed `loadSymbol` -- %s\n", name);
        return 0;
    } else {
        r3_log_stdoutf(SUCCESS_LOG, "[UFOS] Loaded Symbol: %s @%p\n", name, *symbol);
        return 1;
    }
}

u8 unloadLibrary(UFLibrary* library) {
    if (!library || !library->handle) {
        r3_log_stdout(ERROR_LOG, "[UFOS] Failed `unloadLibrary` -- invalid `UFLibrary` pointer\n");
        return 0;
    }
    if (!FreeLibrary((HMODULE)library->handle)) {
        r3_log_stdoutf(ERROR_LOG, "[UFOS] Failed `unloadLibrary` -- %s\n", library->name);
        return 0;
    }

    library->name = NULL;
    library->handle = NULL;
    
    return 1;
}


u8 ufInitOS(UFEVINInterface* evinPtr, UFOSInterface* osInterface) {
    if (!osInterface) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed to init -- invalid interface pointer\n");
        return 0;
    }

    if (!evinPtr || !osInterface) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed to exit -- invalid interface pointer\n");
        return 0;
    }
    
    UFW64.evinPtr = evinPtr;  // set internal UFEVIN API ptr

    // register default HIDs
    UFW64.mouseDefault = UFW64.evinPtr->newDevice(UF_DEVICE_MOUSE); // registered as an RID on call to `newWindow`
    UFW64.keyboardDefault = UFW64.evinPtr->newDevice(UF_DEVICE_KEYBOARD);

    osInterface->newWindow = newWindow;
    osInterface->delWindow = delWindow;
    
    osInterface->setWindowFlag = setWindowFlag;
    osInterface->getWindowFlag = getWindowFlag;
    osInterface->remWindowFlag = remWindowFlag;
    
    osInterface->getEvents = getEvents;
    osInterface->getInputs = getInputs;
    
    osInterface->swapBuffers = swapBuffers;
    
    osInterface->newGpuCtx = newGpuCtx;
    osInterface->delGpuCtx = delGpuCtx;
    
    osInterface->loadSymbol = loadSymbol;
    osInterface->loadLibrary = loadLibrary;
    osInterface->unloadLibrary = unloadLibrary;

    r3_log_stdout(SUCCESS_LOG, "[UFOS] Initialized\n");
    return 1;
}

u8 ufExitOS(UFOSInterface* osInterface) {
    if (!osInterface) {
        r3_log_stdout(WARN_LOG, "[UFOS] Failed to init -- invalid interface pointer\n");
        return 0;
    }

    UFW64.evinPtr->delDevice(UFW64.keyboardDefault);
    UFW64.evinPtr->delDevice(UFW64.mouseDefault);
    UFW64.evinPtr = NULL;

    osInterface->newWindow = NULL;
    osInterface->delWindow = NULL;
    
    osInterface->setWindowFlag = NULL;
    osInterface->getWindowFlag = NULL;
    osInterface->remWindowFlag = NULL;
    
    osInterface->getEvents = NULL;
    osInterface->getInputs = NULL;
    
    osInterface->swapBuffers = NULL;
    
    osInterface->newGpuCtx = NULL;
    osInterface->delGpuCtx = NULL;
    
    osInterface->loadSymbol = NULL;
    osInterface->loadLibrary = NULL;
    osInterface->unloadLibrary = NULL;
    
    r3_log_stdout(SUCCESS_LOG, "[UFOS] Deinitialized\n");
    return 1;
}

#endif // _UF_PLATFORM_WINDOWS_