#pragma once

#include <koncept/core/koncept_defines.h>
#include <koncept/core/events/koncept_events.h>

typedef enum KC_Mouse_Button {
    KC_MBUTTON_LEFT,
    KC_MBUTTON_RIGHT,
    KC_MBUTTON_MIDDLE,
    KC_MOUSE_MAX_BUTTONS
} KC_Mouse_Button;

#define KC_DEFINE_KEY(name, code) \
    KC_KEY_##name = code

typedef enum KC_Keyboard_Key {
    KC_DEFINE_KEY(BACKSPACE, 0x08),
    KC_DEFINE_KEY(ENTER, 0x0D),
    KC_DEFINE_KEY(TAB, 0x09),
    KC_DEFINE_KEY(SHIFT, 0x10),
    KC_DEFINE_KEY(CONTROL, 0x11),
    KC_DEFINE_KEY(PAUSE, 0x13),
    KC_DEFINE_KEY(CAPITAL, 0x14),
    KC_DEFINE_KEY(ESCAPE, 0x1B),
    KC_DEFINE_KEY(CONVERT, 0x1C),
    KC_DEFINE_KEY(NONCONVERT, 0x1D),
    KC_DEFINE_KEY(ACCEPT, 0x1E),
    KC_DEFINE_KEY(MODECHANGE, 0x1F),
    KC_DEFINE_KEY(SPACE, 0x20),
    KC_DEFINE_KEY(PRIOR, 0x21),
    KC_DEFINE_KEY(NEXT, 0x22),
    KC_DEFINE_KEY(END, 0x23),
    KC_DEFINE_KEY(HOME, 0x24),
    KC_DEFINE_KEY(LEFT, 0x25),
    KC_DEFINE_KEY(UP, 0x26),
    KC_DEFINE_KEY(RIGHT, 0x27),
    KC_DEFINE_KEY(DOWN, 0x28),
    KC_DEFINE_KEY(SELECT, 0x29),
    KC_DEFINE_KEY(PRINT, 0x2A),
    KC_DEFINE_KEY(EXECUTE, 0x2B),
    KC_DEFINE_KEY(SNAPSHOT, 0x2C),
    KC_DEFINE_KEY(INSERT, 0x2D),
    KC_DEFINE_KEY(DELETE, 0x2E),
    KC_DEFINE_KEY(HELP, 0x2F),
    KC_DEFINE_KEY(A, 0x41),
    KC_DEFINE_KEY(B, 0x42),
    KC_DEFINE_KEY(C, 0x43),
    KC_DEFINE_KEY(D, 0x44),
    KC_DEFINE_KEY(E, 0x45),
    KC_DEFINE_KEY(F, 0x46),
    KC_DEFINE_KEY(G, 0x47),
    KC_DEFINE_KEY(H, 0x48),
    KC_DEFINE_KEY(I, 0x49),
    KC_DEFINE_KEY(J, 0x4A),
    KC_DEFINE_KEY(K, 0x4B),
    KC_DEFINE_KEY(L, 0x4C),
    KC_DEFINE_KEY(M, 0x4D),
    KC_DEFINE_KEY(N, 0x4E),
    KC_DEFINE_KEY(O, 0x4F),
    KC_DEFINE_KEY(P, 0x50),
    KC_DEFINE_KEY(Q, 0x51),
    KC_DEFINE_KEY(R, 0x52),
    KC_DEFINE_KEY(S, 0x53),
    KC_DEFINE_KEY(T, 0x54),
    KC_DEFINE_KEY(U, 0x55),
    KC_DEFINE_KEY(V, 0x56),
    KC_DEFINE_KEY(W, 0x57),
    KC_DEFINE_KEY(X, 0x58),
    KC_DEFINE_KEY(Y, 0x59),
    KC_DEFINE_KEY(Z, 0x5A),
    KC_DEFINE_KEY(LWIN, 0x5B),
    KC_DEFINE_KEY(RWIN, 0x5C),
    KC_DEFINE_KEY(APPS, 0x5D),
    KC_DEFINE_KEY(SLEEP, 0x5F),
    KC_DEFINE_KEY(NUMPAD0, 0x60),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD1, 0x61),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD2, 0x62),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD3, 0x63),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD4, 0x64),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD5, 0x65),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD6, 0x66),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD7, 0x67),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD8, 0x68),    // wrong on win32????
    KC_DEFINE_KEY(NUMPAD9, 0x69),    // wrong on win32????
    KC_DEFINE_KEY(MULTIPLY, 0x6A),
    KC_DEFINE_KEY(ADD, 0x6B),
    KC_DEFINE_KEY(SEPARATOR, 0x6C),
    KC_DEFINE_KEY(SUBTRACT, 0x6D),
    KC_DEFINE_KEY(DECIMAL, 0x6E),
    KC_DEFINE_KEY(DIVIDE, 0x6F),
    KC_DEFINE_KEY(F1, 0x70),
    KC_DEFINE_KEY(F2, 0x71),
    KC_DEFINE_KEY(F3, 0x72),
    KC_DEFINE_KEY(F4, 0x73),
    KC_DEFINE_KEY(F5, 0x74),
    KC_DEFINE_KEY(F6, 0x75),
    KC_DEFINE_KEY(F7, 0x76),
    KC_DEFINE_KEY(F8, 0x77),
    KC_DEFINE_KEY(F9, 0x78),
    KC_DEFINE_KEY(F10, 0x79),
    KC_DEFINE_KEY(F11, 0x7A),
    KC_DEFINE_KEY(F12, 0x7B),
    KC_DEFINE_KEY(F13, 0x7C),
    KC_DEFINE_KEY(F14, 0x7D),
    KC_DEFINE_KEY(F15, 0x7E),
    KC_DEFINE_KEY(F16, 0x7F),
    KC_DEFINE_KEY(F17, 0x80),
    KC_DEFINE_KEY(F18, 0x81),
    KC_DEFINE_KEY(F19, 0x82),
    KC_DEFINE_KEY(F20, 0x83),
    KC_DEFINE_KEY(F21, 0x84),
    KC_DEFINE_KEY(F22, 0x85),
    KC_DEFINE_KEY(F23, 0x86),
    KC_DEFINE_KEY(F24, 0x87),
    KC_DEFINE_KEY(NUMLOCK, 0x90),
    KC_DEFINE_KEY(SCROLL, 0x91),
    KC_DEFINE_KEY(NUMPAD_EQUAL, 0x92),
    // KC_DEFINE_KEY(LSHIFT, 0xA0),
    // KC_DEFINE_KEY(RSHIFT, 0xA1),
    KC_DEFINE_KEY(LCONTROL, 0xA2),
    KC_DEFINE_KEY(RCONTROL, 0xA3),
    KC_DEFINE_KEY(LMENU, 0xA4),
    KC_DEFINE_KEY(RMENU, 0xA5),
    KC_DEFINE_KEY(SEMICOLON, 0xBA),
    KC_DEFINE_KEY(PLUS, 0xBB),
    KC_DEFINE_KEY(COMMA, 0xBC),
    KC_DEFINE_KEY(MINUS, 0xBD),
    KC_DEFINE_KEY(PERIOD, 0xBE),
    KC_DEFINE_KEY(SLASH, 0xBF),
    KC_DEFINE_KEY(GRAVE, 0xC0),
    KC_MAX_KEYS
} KC_Keyboard_Key;

typedef struct _koncept_input_api {
    void (*_process_mouse_wheel_input)(i8 z_delta);
    void (*_process_mouse_move_input)(i16 x, i16 y);
    void (*_process_key_input)(KC_Keyboard_Key key, u8 pressed);
    void (*_process_mouse_button_input)(KC_Mouse_Button button, u8 pressed);

    void (*reset)(void);
    void (*update)(void);

    u8 (*key_is_up)(KC_Keyboard_Key key);
    u8 (*key_was_up)(KC_Keyboard_Key key);
    u8 (*key_is_down)(KC_Keyboard_Key key);
    u8 (*key_was_down)(KC_Keyboard_Key key);

    u8 (*button_is_up)(KC_Mouse_Button button);
    u8 (*button_was_up)(KC_Mouse_Button button);
    u8 (*button_is_down)(KC_Mouse_Button button);
    u8 (*button_was_down)(KC_Mouse_Button button);

    void (*mouse_get_position)(i16* x, i16* y);
    void (*mouse_get_last_position)(i16* x, i16* y);
} _koncept_input_api;

u8 _koncept_init_input(_koncept_events_api* events_api, _koncept_input_api* api);
u8 _koncept_cleanup_input(_koncept_input_api* api);