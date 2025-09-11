#ifndef _UFEVIN_H_
#define _UFEVIN_H_

#define _UF_R3DEF_
#include <ufcore/UFDEF.h>

#define UF_DEVICE_MAX 0x10
#define UF_MAX_KEYBOARD_KEYS 0x100
#define UF_MAX_MOUSE_BUTTONS 0x005

#define UF_EVENT_CODE_MAX 0xFF
#define UF_EVENT_HOOK_MAX 0x3FC0

typedef enum UFDeviceType {
    UF_DEVICE_MOUSE,
    UF_DEVICE_KEYBOARD,
    UF_DEVICE_CONTROLLER,
    UF_DEVICE_COUNT = 3
} UFDeviceType;

typedef enum UFDeviceBuffer {
    UF_DEVICE_BUFFER_LAST,
    UF_DEVICE_BUFFER_NOW,
    UF_DEVICE_BUFFER_COUNT
} UFDeviceBuffer;

#define UF_KEYBOARD_KEY(n, c) UF_KEYBOARD_##n = c
typedef enum UFKeyboardKey {
    UF_KEYBOARD_KEY(BACKSPACE, 0x08),
    UF_KEYBOARD_KEY(ENTER, 0x0D),
    UF_KEYBOARD_KEY(TAB, 0x09),
    UF_KEYBOARD_KEY(SHIFT, 0x10),
    UF_KEYBOARD_KEY(CONTROL, 0x11),
    UF_KEYBOARD_KEY(PAUSE, 0x13),
    UF_KEYBOARD_KEY(CAPITAL, 0x14),
    UF_KEYBOARD_KEY(ESCAPE, 0x1B),
    UF_KEYBOARD_KEY(CONVERT, 0x1C),
    UF_KEYBOARD_KEY(NONCONVERT, 0x1D),
    UF_KEYBOARD_KEY(ACCEPT, 0x1E),
    UF_KEYBOARD_KEY(MODECHANGE, 0x1F),
    UF_KEYBOARD_KEY(SPACE, 0x20),
    UF_KEYBOARD_KEY(PRIOR, 0x21),
    UF_KEYBOARD_KEY(NEXT, 0x22),
    UF_KEYBOARD_KEY(END, 0x23),
    UF_KEYBOARD_KEY(HOME, 0x24),
    UF_KEYBOARD_KEY(LEFT, 0x25),
    UF_KEYBOARD_KEY(UP, 0x26),
    UF_KEYBOARD_KEY(RIGHT, 0x27),
    UF_KEYBOARD_KEY(DOWN, 0x28),
    UF_KEYBOARD_KEY(SELECT, 0x29),
    UF_KEYBOARD_KEY(PRINT, 0x2A),
    UF_KEYBOARD_KEY(EXECUTE, 0x2B),
    UF_KEYBOARD_KEY(SNAPSHOT, 0x2C),
    UF_KEYBOARD_KEY(INSERT, 0x2D),
    UF_KEYBOARD_KEY(DELETE, 0x2E),
    UF_KEYBOARD_KEY(HELP, 0x2F),
    UF_KEYBOARD_KEY(A, 0x41),
    UF_KEYBOARD_KEY(B, 0x42),
    UF_KEYBOARD_KEY(C, 0x43),
    UF_KEYBOARD_KEY(D, 0x44),
    UF_KEYBOARD_KEY(E, 0x45),
    UF_KEYBOARD_KEY(F, 0x46),
    UF_KEYBOARD_KEY(G, 0x47),
    UF_KEYBOARD_KEY(H, 0x48),
    UF_KEYBOARD_KEY(I, 0x49),
    UF_KEYBOARD_KEY(J, 0x4A),
    UF_KEYBOARD_KEY(K, 0x4B),
    UF_KEYBOARD_KEY(L, 0x4C),
    UF_KEYBOARD_KEY(M, 0x4D),
    UF_KEYBOARD_KEY(N, 0x4E),
    UF_KEYBOARD_KEY(O, 0x4F),
    UF_KEYBOARD_KEY(P, 0x50),
    UF_KEYBOARD_KEY(Q, 0x51),
    UF_KEYBOARD_KEY(R, 0x52),
    UF_KEYBOARD_KEY(S, 0x53),
    UF_KEYBOARD_KEY(T, 0x54),
    UF_KEYBOARD_KEY(U, 0x55),
    UF_KEYBOARD_KEY(V, 0x56),
    UF_KEYBOARD_KEY(W, 0x57),
    UF_KEYBOARD_KEY(X, 0x58),
    UF_KEYBOARD_KEY(Y, 0x59),
    UF_KEYBOARD_KEY(Z, 0x5A),
    UF_KEYBOARD_KEY(LWIN, 0x5B),
    UF_KEYBOARD_KEY(RWIN, 0x5C),
    UF_KEYBOARD_KEY(APPS, 0x5D),
    UF_KEYBOARD_KEY(SLEEP, 0x5F),
    UF_KEYBOARD_KEY(NUMPAD0, 0x60),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD1, 0x61),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD2, 0x62),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD3, 0x63),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD4, 0x64),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD5, 0x65),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD6, 0x66),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD7, 0x67),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD8, 0x68),    // wrong on win32????
    UF_KEYBOARD_KEY(NUMPAD9, 0x69),    // wrong on win32????
    UF_KEYBOARD_KEY(MULTIPLY, 0x6A),
    UF_KEYBOARD_KEY(ADD, 0x6B),
    UF_KEYBOARD_KEY(SEPARATOR, 0x6C),
    UF_KEYBOARD_KEY(SUBTRACT, 0x6D),
    UF_KEYBOARD_KEY(DECIMAL, 0x6E),
    UF_KEYBOARD_KEY(DIVIDE, 0x6F),
    UF_KEYBOARD_KEY(F1, 0x70),
    UF_KEYBOARD_KEY(F2, 0x71),
    UF_KEYBOARD_KEY(F3, 0x72),
    UF_KEYBOARD_KEY(F4, 0x73),
    UF_KEYBOARD_KEY(F5, 0x74),
    UF_KEYBOARD_KEY(F6, 0x75),
    UF_KEYBOARD_KEY(F7, 0x76),
    UF_KEYBOARD_KEY(F8, 0x77),
    UF_KEYBOARD_KEY(F9, 0x78),
    UF_KEYBOARD_KEY(F10, 0x79),
    UF_KEYBOARD_KEY(F11, 0x7A),
    UF_KEYBOARD_KEY(F12, 0x7B),
    UF_KEYBOARD_KEY(F13, 0x7C),
    UF_KEYBOARD_KEY(F14, 0x7D),
    UF_KEYBOARD_KEY(F15, 0x7E),
    UF_KEYBOARD_KEY(F16, 0x7F),
    UF_KEYBOARD_KEY(F17, 0x80),
    UF_KEYBOARD_KEY(F18, 0x81),
    UF_KEYBOARD_KEY(F19, 0x82),
    UF_KEYBOARD_KEY(F20, 0x83),
    UF_KEYBOARD_KEY(F21, 0x84),
    UF_KEYBOARD_KEY(F22, 0x85),
    UF_KEYBOARD_KEY(F23, 0x86),
    UF_KEYBOARD_KEY(F24, 0x87),
    UF_KEYBOARD_KEY(NUMLOCK, 0x90),
    UF_KEYBOARD_KEY(SCROLL, 0x91),
    UF_KEYBOARD_KEY(NUMPAD_EQUAL, 0x92),
    // UF_KEYBOARD_KEY(LSHIFT, 0xA0),
    // UF_KEYBOARD_KEY(RSHIFT, 0xA1),
    UF_KEYBOARD_KEY(LCONTROL, 0xA2),
    UF_KEYBOARD_KEY(RCONTROL, 0xA3),
    UF_KEYBOARD_KEY(LMENU, 0xA4),
    UF_KEYBOARD_KEY(RMENU, 0xA5),
    UF_KEYBOARD_KEY(SEMICOLON, 0xBA),
    UF_KEYBOARD_KEY(PLUS, 0xBB),
    UF_KEYBOARD_KEY(COMMA, 0xBC),
    UF_KEYBOARD_KEY(MINUS, 0xBD),
    UF_KEYBOARD_KEY(PERIOD, 0xBE),
    UF_KEYBOARD_KEY(SLASH, 0xBF),
    UF_KEYBOARD_KEY(GRAVE, 0xC0)
} UFKeyboardKey;

typedef enum UFMouseButton {
    UF_MOUSE_LEFT,
    UF_MOUSE_RIGHT,
    UF_MOUSE_MIDDLE,
    UF_MOUSE_SIDE1,
    UF_MOUSE_SIDE2
} UFMouseButton;

typedef enum UFMouseDelta {
    UF_MOUSE_DELTA_X,
    UF_MOUSE_DELTA_Y,
    UF_MOUSE_DELTA_COUNT
} UFMouseDelta;

typedef enum UFDeviceState {
    UF_DEVICE_STATE_INACTIVE,
    UF_DEVICE_STATE_ACTIVE,
    UF_DEVICE_STATE_COUNT
} UFDeviceState;

typedef enum UFDefaultEventCode {
    UF_EVENT_EXIT,
    UF_EVENT_RESIZE,
    UF_EVENT_MOUSE_MOVE,
    UF_EVENT_MOUSE_WHEEL,
    UF_EVENT_KEY_PRESSED,
    UF_EVENT_KEY_RELEASED,
    UF_EVENT_BUTTON_PRESSED,
    UF_EVENT_BUTTON_RELEASED,
    UF_DEFAULT_EVENT_CODE_COUNT
} UFDefaultEventCode;

typedef struct UFEvent {
    // 128 byte max
    s64 i64[2];
    u64 u64[2];
    f64 f64[2];

    s32 s32[4];
    u32 u32[4];
    f32 f32[4];

    s16 s16[8];
    u16 u16[8];
    
    s8 s8[16];
    u8 u8[16];
    char c[16];
} UFEvent;

typedef none (*UFEventHook)(UFEventCode, UFEvent);

typedef struct UFEVINInterface {
    UFVersion ver;
    
    UF_API_METHOD(u8, newEvent, UFEventCode code);
    UF_API_METHOD(u8, delEvent, UFEventCode code);
    UF_API_METHOD(u8, sendEvent, UFEventCode code, UFEvent event);

    UF_API_METHOD(u8, newHook, UFEventCode code, UFEventHook hook);
    UF_API_METHOD(u8, delHook, UFEventCode code, UFEventHook hook);
    
    UF_API_METHOD(UFResource, newDevice, UFDeviceType type);
    UF_API_METHOD(u8, delDevice, UFResource device);

    UF_API_METHOD(u8, resetDevices, none);
    UF_API_METHOD(u8, updateDevices, none);

    UF_API_METHOD(u8, setDeviceState, UFDeviceState state, UFDeviceBuffer buffer, UFResource device);
    UF_API_METHOD(u8, resetDeviceState, UFDeviceBuffer buffer, UFResource device);
    UF_API_METHOD(u8, updateDeviceDelta, s16 dx, s16 dy, UFResource device);

    UF_API_METHOD(UFDeviceState, getKeyboardState, UFKeyboardKey key, UFDeviceBuffer buffer, UFResource device);
    UF_API_METHOD(UFDeviceState, getMouseState, UFMouseButton button, UFDeviceBuffer buffer, UFResource device);
    
    UF_API_METHOD(u8, setKeyboardState, UFDeviceState state, UFKeyboardKey key, UFDeviceBuffer buffer, UFResource device);
    UF_API_METHOD(u8, setMouseState, UFDeviceState state, UFMouseButton button, UFDeviceBuffer buffer, UFResource device);
} UFEVINInterface;

#endif // _UFEVIN_H_
