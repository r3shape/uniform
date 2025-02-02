#pragma once

#include "../lotus_core_defines.h"

typedef enum Lotus_Mouse_Button {
    LOTUS_MBUTTON_LEFT,
    LOTUS_MBUTTON_RIGHT,
    LOTUS_MBUTTON_MIDDLE,
    LOTUS_MBUTTON_MAX_BUTTONS
} Lotus_Mouse_Button;

#define LOTUS_DEFINE_KEY(name, code) \
    LOTUS_KEY_##name = code

typedef enum Lotus_Keyboard_Key {
    LOTUS_DEFINE_KEY(BACKSPACE, 0x08),
    LOTUS_DEFINE_KEY(ENTER, 0x0D),
    LOTUS_DEFINE_KEY(TAB, 0x09),
    LOTUS_DEFINE_KEY(SHIFT, 0x10),
    LOTUS_DEFINE_KEY(CONTROL, 0x11),
    LOTUS_DEFINE_KEY(PAUSE, 0x13),
    LOTUS_DEFINE_KEY(CAPITAL, 0x14),
    LOTUS_DEFINE_KEY(ESCAPE, 0x1B),
    LOTUS_DEFINE_KEY(CONVERT, 0x1C),
    LOTUS_DEFINE_KEY(NONCONVERT, 0x1D),
    LOTUS_DEFINE_KEY(ACCEPT, 0x1E),
    LOTUS_DEFINE_KEY(MODECHANGE, 0x1F),
    LOTUS_DEFINE_KEY(SPACE, 0x20),
    LOTUS_DEFINE_KEY(PRIOR, 0x21),
    LOTUS_DEFINE_KEY(NEXT, 0x22),
    LOTUS_DEFINE_KEY(END, 0x23),
    LOTUS_DEFINE_KEY(HOME, 0x24),
    LOTUS_DEFINE_KEY(LEFT, 0x25),
    LOTUS_DEFINE_KEY(UP, 0x26),
    LOTUS_DEFINE_KEY(RIGHT, 0x27),
    LOTUS_DEFINE_KEY(DOWN, 0x28),
    LOTUS_DEFINE_KEY(SELECT, 0x29),
    LOTUS_DEFINE_KEY(PRINT, 0x2A),
    LOTUS_DEFINE_KEY(EXECUTE, 0x2B),
    LOTUS_DEFINE_KEY(SNAPSHOT, 0x2C),
    LOTUS_DEFINE_KEY(INSERT, 0x2D),
    LOTUS_DEFINE_KEY(DELETE, 0x2E),
    LOTUS_DEFINE_KEY(HELP, 0x2F),
    LOTUS_DEFINE_KEY(A, 0x41),
    LOTUS_DEFINE_KEY(B, 0x42),
    LOTUS_DEFINE_KEY(C, 0x43),
    LOTUS_DEFINE_KEY(D, 0x44),
    LOTUS_DEFINE_KEY(E, 0x45),
    LOTUS_DEFINE_KEY(F, 0x46),
    LOTUS_DEFINE_KEY(G, 0x47),
    LOTUS_DEFINE_KEY(H, 0x48),
    LOTUS_DEFINE_KEY(I, 0x49),
    LOTUS_DEFINE_KEY(J, 0x4A),
    LOTUS_DEFINE_KEY(K, 0x4B),
    LOTUS_DEFINE_KEY(L, 0x4C),
    LOTUS_DEFINE_KEY(M, 0x4D),
    LOTUS_DEFINE_KEY(N, 0x4E),
    LOTUS_DEFINE_KEY(O, 0x4F),
    LOTUS_DEFINE_KEY(P, 0x50),
    LOTUS_DEFINE_KEY(Q, 0x51),
    LOTUS_DEFINE_KEY(R, 0x52),
    LOTUS_DEFINE_KEY(S, 0x53),
    LOTUS_DEFINE_KEY(T, 0x54),
    LOTUS_DEFINE_KEY(U, 0x55),
    LOTUS_DEFINE_KEY(V, 0x56),
    LOTUS_DEFINE_KEY(W, 0x57),
    LOTUS_DEFINE_KEY(X, 0x58),
    LOTUS_DEFINE_KEY(Y, 0x59),
    LOTUS_DEFINE_KEY(Z, 0x5A),
    LOTUS_DEFINE_KEY(LWIN, 0x5B),
    LOTUS_DEFINE_KEY(RWIN, 0x5C),
    LOTUS_DEFINE_KEY(APPS, 0x5D),
    LOTUS_DEFINE_KEY(SLEEP, 0x5F),
    LOTUS_DEFINE_KEY(NUMPAD0, 0x60),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD1, 0x61),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD2, 0x62),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD3, 0x63),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD4, 0x64),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD5, 0x65),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD6, 0x66),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD7, 0x67),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD8, 0x68),    // wrong on win32????
    LOTUS_DEFINE_KEY(NUMPAD9, 0x69),    // wrong on win32????
    LOTUS_DEFINE_KEY(MULTIPLY, 0x6A),
    LOTUS_DEFINE_KEY(ADD, 0x6B),
    LOTUS_DEFINE_KEY(SEPARATOR, 0x6C),
    LOTUS_DEFINE_KEY(SUBTRACT, 0x6D),
    LOTUS_DEFINE_KEY(DECIMAL, 0x6E),
    LOTUS_DEFINE_KEY(DIVIDE, 0x6F),
    LOTUS_DEFINE_KEY(F1, 0x70),
    LOTUS_DEFINE_KEY(F2, 0x71),
    LOTUS_DEFINE_KEY(F3, 0x72),
    LOTUS_DEFINE_KEY(F4, 0x73),
    LOTUS_DEFINE_KEY(F5, 0x74),
    LOTUS_DEFINE_KEY(F6, 0x75),
    LOTUS_DEFINE_KEY(F7, 0x76),
    LOTUS_DEFINE_KEY(F8, 0x77),
    LOTUS_DEFINE_KEY(F9, 0x78),
    LOTUS_DEFINE_KEY(F10, 0x79),
    LOTUS_DEFINE_KEY(F11, 0x7A),
    LOTUS_DEFINE_KEY(F12, 0x7B),
    LOTUS_DEFINE_KEY(F13, 0x7C),
    LOTUS_DEFINE_KEY(F14, 0x7D),
    LOTUS_DEFINE_KEY(F15, 0x7E),
    LOTUS_DEFINE_KEY(F16, 0x7F),
    LOTUS_DEFINE_KEY(F17, 0x80),
    LOTUS_DEFINE_KEY(F18, 0x81),
    LOTUS_DEFINE_KEY(F19, 0x82),
    LOTUS_DEFINE_KEY(F20, 0x83),
    LOTUS_DEFINE_KEY(F21, 0x84),
    LOTUS_DEFINE_KEY(F22, 0x85),
    LOTUS_DEFINE_KEY(F23, 0x86),
    LOTUS_DEFINE_KEY(F24, 0x87),
    LOTUS_DEFINE_KEY(NUMLOCK, 0x90),
    LOTUS_DEFINE_KEY(SCROLL, 0x91),
    LOTUS_DEFINE_KEY(NUMPAD_EQUAL, 0x92),
    LOTUS_DEFINE_KEY(LSHIFT, 0xA0),
    LOTUS_DEFINE_KEY(RSHIFT, 0xA1),
    LOTUS_DEFINE_KEY(LCONTROL, 0xA2),
    LOTUS_DEFINE_KEY(RCONTROL, 0xA3),
    LOTUS_DEFINE_KEY(LMENU, 0xA4),
    LOTUS_DEFINE_KEY(RMENU, 0xA5),
    LOTUS_DEFINE_KEY(SEMICOLON, 0xBA),
    LOTUS_DEFINE_KEY(PLUS, 0xBB),
    LOTUS_DEFINE_KEY(COMMA, 0xBC),
    LOTUS_DEFINE_KEY(MINUS, 0xBD),
    LOTUS_DEFINE_KEY(PERIOD, 0xBE),
    LOTUS_DEFINE_KEY(SLASH, 0xBF),
    LOTUS_DEFINE_KEY(GRAVE, 0xC0),
    LOTUS_MAX_KEYS
} Lotus_Keyboard_Key;

typedef struct Llotus_Input_State {
    sbyte2 mouse[2];
    ubyte keys[256];
    ubyte buttons[LOTUS_MBUTTON_MAX_BUTTONS];
} Llotus_Input_State;

#define LOTUS_INPUT_STATE_PREVIOUS 0
#define LOTUS_INPUT_STATE_CURRENT 1
typedef struct Lotus_Input_State {
    ubyte init;
    Llotus_Input_State current;
    Llotus_Input_State previous;
} Lotus_Input_State;

Lotus_Input_State* lotus_init_input(void);
void lotus_shutdown_input(void);

void lotus_process_key_input(Lotus_Keyboard_Key key, ubyte pressed);
void lotus_process_mouse_wheel_input(sbyte z_delta);
void lotus_process_mouse_move_input(sbyte2 x, sbyte2 y);
void lotus_process_mouse_button_input(Lotus_Mouse_Button button, ubyte pressed);

LOTUS_API void lotus_update_input(f64 delta_time);

LOTUS_API ubyte lotus_key_is_up(Lotus_Keyboard_Key key);
LOTUS_API ubyte lotus_key_was_up(Lotus_Keyboard_Key key);
LOTUS_API ubyte lotus_key_is_down(Lotus_Keyboard_Key key);
LOTUS_API ubyte lotus_key_was_down(Lotus_Keyboard_Key key);

LOTUS_API ubyte lotus_button_is_up(Lotus_Mouse_Button button);
LOTUS_API ubyte lotus_button_was_up(Lotus_Mouse_Button button);
LOTUS_API ubyte lotus_button_is_down(Lotus_Mouse_Button button);
LOTUS_API ubyte lotus_button_was_down(Lotus_Mouse_Button button);

LOTUS_API void lotus_mouse_get_position(sbyte2* x, sbyte2* y);
LOTUS_API void lotus_mouse_get_last_position(sbyte2* x, sbyte2* y);
