#pragma once

#include "../r3_core_defines.h"

typedef enum R3_Mouse_Button {
    R3_MBUTTON_LEFT,
    R3_MBUTTON_RIGHT,
    R3_MBUTTON_MIDDLE,
    R3_MBUTTON_MAX_BUTTONS
} R3_Mouse_Button;

#define R3_DEFINE_KEY(name, code) \
    R3_KEY_##name = code

typedef enum R3_Keyboard_Key {
    R3_DEFINE_KEY(BACKSPACE, 0x08),
    R3_DEFINE_KEY(ENTER, 0x0D),
    R3_DEFINE_KEY(TAB, 0x09),
    R3_DEFINE_KEY(SHIFT, 0x10),
    R3_DEFINE_KEY(CONTROL, 0x11),
    R3_DEFINE_KEY(PAUSE, 0x13),
    R3_DEFINE_KEY(CAPITAL, 0x14),
    R3_DEFINE_KEY(ESCAPE, 0x1B),
    R3_DEFINE_KEY(CONVERT, 0x1C),
    R3_DEFINE_KEY(NONCONVERT, 0x1D),
    R3_DEFINE_KEY(ACCEPT, 0x1E),
    R3_DEFINE_KEY(MODECHANGE, 0x1F),
    R3_DEFINE_KEY(SPACE, 0x20),
    R3_DEFINE_KEY(PRIOR, 0x21),
    R3_DEFINE_KEY(NEXT, 0x22),
    R3_DEFINE_KEY(END, 0x23),
    R3_DEFINE_KEY(HOME, 0x24),
    R3_DEFINE_KEY(LEFT, 0x25),
    R3_DEFINE_KEY(UP, 0x26),
    R3_DEFINE_KEY(RIGHT, 0x27),
    R3_DEFINE_KEY(DOWN, 0x28),
    R3_DEFINE_KEY(SELECT, 0x29),
    R3_DEFINE_KEY(PRINT, 0x2A),
    R3_DEFINE_KEY(EXECUTE, 0x2B),
    R3_DEFINE_KEY(SNAPSHOT, 0x2C),
    R3_DEFINE_KEY(INSERT, 0x2D),
    R3_DEFINE_KEY(DELETE, 0x2E),
    R3_DEFINE_KEY(HELP, 0x2F),
    R3_DEFINE_KEY(A, 0x41),
    R3_DEFINE_KEY(B, 0x42),
    R3_DEFINE_KEY(C, 0x43),
    R3_DEFINE_KEY(D, 0x44),
    R3_DEFINE_KEY(E, 0x45),
    R3_DEFINE_KEY(F, 0x46),
    R3_DEFINE_KEY(G, 0x47),
    R3_DEFINE_KEY(H, 0x48),
    R3_DEFINE_KEY(I, 0x49),
    R3_DEFINE_KEY(J, 0x4A),
    R3_DEFINE_KEY(K, 0x4B),
    R3_DEFINE_KEY(L, 0x4C),
    R3_DEFINE_KEY(M, 0x4D),
    R3_DEFINE_KEY(N, 0x4E),
    R3_DEFINE_KEY(O, 0x4F),
    R3_DEFINE_KEY(P, 0x50),
    R3_DEFINE_KEY(Q, 0x51),
    R3_DEFINE_KEY(R, 0x52),
    R3_DEFINE_KEY(S, 0x53),
    R3_DEFINE_KEY(T, 0x54),
    R3_DEFINE_KEY(U, 0x55),
    R3_DEFINE_KEY(V, 0x56),
    R3_DEFINE_KEY(W, 0x57),
    R3_DEFINE_KEY(X, 0x58),
    R3_DEFINE_KEY(Y, 0x59),
    R3_DEFINE_KEY(Z, 0x5A),
    R3_DEFINE_KEY(LWIN, 0x5B),
    R3_DEFINE_KEY(RWIN, 0x5C),
    R3_DEFINE_KEY(APPS, 0x5D),
    R3_DEFINE_KEY(SLEEP, 0x5F),
    R3_DEFINE_KEY(NUMPAD0, 0x60),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD1, 0x61),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD2, 0x62),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD3, 0x63),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD4, 0x64),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD5, 0x65),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD6, 0x66),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD7, 0x67),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD8, 0x68),    // wrong on win32????
    R3_DEFINE_KEY(NUMPAD9, 0x69),    // wrong on win32????
    R3_DEFINE_KEY(MULTIPLY, 0x6A),
    R3_DEFINE_KEY(ADD, 0x6B),
    R3_DEFINE_KEY(SEPARATOR, 0x6C),
    R3_DEFINE_KEY(SUBTRACT, 0x6D),
    R3_DEFINE_KEY(DECIMAL, 0x6E),
    R3_DEFINE_KEY(DIVIDE, 0x6F),
    R3_DEFINE_KEY(F1, 0x70),
    R3_DEFINE_KEY(F2, 0x71),
    R3_DEFINE_KEY(F3, 0x72),
    R3_DEFINE_KEY(F4, 0x73),
    R3_DEFINE_KEY(F5, 0x74),
    R3_DEFINE_KEY(F6, 0x75),
    R3_DEFINE_KEY(F7, 0x76),
    R3_DEFINE_KEY(F8, 0x77),
    R3_DEFINE_KEY(F9, 0x78),
    R3_DEFINE_KEY(F10, 0x79),
    R3_DEFINE_KEY(F11, 0x7A),
    R3_DEFINE_KEY(F12, 0x7B),
    R3_DEFINE_KEY(F13, 0x7C),
    R3_DEFINE_KEY(F14, 0x7D),
    R3_DEFINE_KEY(F15, 0x7E),
    R3_DEFINE_KEY(F16, 0x7F),
    R3_DEFINE_KEY(F17, 0x80),
    R3_DEFINE_KEY(F18, 0x81),
    R3_DEFINE_KEY(F19, 0x82),
    R3_DEFINE_KEY(F20, 0x83),
    R3_DEFINE_KEY(F21, 0x84),
    R3_DEFINE_KEY(F22, 0x85),
    R3_DEFINE_KEY(F23, 0x86),
    R3_DEFINE_KEY(F24, 0x87),
    R3_DEFINE_KEY(NUMLOCK, 0x90),
    R3_DEFINE_KEY(SCROLL, 0x91),
    R3_DEFINE_KEY(NUMPAD_EQUAL, 0x92),
    R3_DEFINE_KEY(LSHIFT, 0xA0),
    R3_DEFINE_KEY(RSHIFT, 0xA1),
    R3_DEFINE_KEY(LCONTROL, 0xA2),
    R3_DEFINE_KEY(RCONTROL, 0xA3),
    R3_DEFINE_KEY(LMENU, 0xA4),
    R3_DEFINE_KEY(RMENU, 0xA5),
    R3_DEFINE_KEY(SEMICOLON, 0xBA),
    R3_DEFINE_KEY(PLUS, 0xBB),
    R3_DEFINE_KEY(COMMA, 0xBC),
    R3_DEFINE_KEY(MINUS, 0xBD),
    R3_DEFINE_KEY(PERIOD, 0xBE),
    R3_DEFINE_KEY(SLASH, 0xBF),
    R3_DEFINE_KEY(GRAVE, 0xC0),
    R3_MAX_KEYS
} R3_Keyboard_Key;

typedef struct Lr3_Input_State {
    sbyte2 mouse[2];
    ubyte keys[256];
    ubyte buttons[R3_MBUTTON_MAX_BUTTONS];
} Lr3_Input_State;

#define R3_INPUT_STATE_PREVIOUS 0
#define R3_INPUT_STATE_CURRENT 1
typedef struct R3_Input_State {
    ubyte init;
    Lr3_Input_State current;
    Lr3_Input_State previous;
} R3_Input_State;

R3_Input_State* r3_init_input(void);
void r3_shutdown_input(void);

void r3_process_key_input(R3_Keyboard_Key key, ubyte pressed);
void r3_process_mouse_wheel_input(sbyte z_delta);
void r3_process_mouse_move_input(sbyte2 x, sbyte2 y);
void r3_process_mouse_button_input(R3_Mouse_Button button, ubyte pressed);

R3_API void r3_update_input(f64 delta_time);

R3_API ubyte r3_key_is_up(R3_Keyboard_Key key);
R3_API ubyte r3_key_was_up(R3_Keyboard_Key key);
R3_API ubyte r3_key_is_down(R3_Keyboard_Key key);
R3_API ubyte r3_key_was_down(R3_Keyboard_Key key);

R3_API ubyte r3_button_is_up(R3_Mouse_Button button);
R3_API ubyte r3_button_was_up(R3_Mouse_Button button);
R3_API ubyte r3_button_is_down(R3_Mouse_Button button);
R3_API ubyte r3_button_was_down(R3_Mouse_Button button);

R3_API void r3_mouse_get_position(sbyte2* x, sbyte2* y);
R3_API void r3_mouse_get_last_position(sbyte2* x, sbyte2* y);
