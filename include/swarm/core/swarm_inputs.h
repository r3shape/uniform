#ifndef __SWARM_INPUT_H__
#define __SWARM_INPUT_H__

#include <include/swarm/core/globals.h>

#define SWARM_DEFINE_KEY(name, code) SWARM_KEY_##name = code

typedef enum SwarmMouseButton {
    SWARM_MBUTTON_LEFT,
    SWARM_MBUTTON_RIGHT,
    SWARM_MBUTTON_MIDDLE,
    SWARM_MBUTTON_SIDE1,
    SWARM_MBUTTON_SIDE2,
    SWARM_MOUSE_MAX_BUTTONS
} SwarmMouseButton;

typedef enum SwarmKeyboardKey {
    SWARM_DEFINE_KEY(BACKSPACE, 0x08),
    SWARM_DEFINE_KEY(ENTER, 0x0D),
    SWARM_DEFINE_KEY(TAB, 0x09),
    SWARM_DEFINE_KEY(SHIFT, 0x10),
    SWARM_DEFINE_KEY(CONTROL, 0x11),
    SWARM_DEFINE_KEY(PAUSE, 0x13),
    SWARM_DEFINE_KEY(CAPITAL, 0x14),
    SWARM_DEFINE_KEY(ESCAPE, 0x1B),
    SWARM_DEFINE_KEY(CONVERT, 0x1C),
    SWARM_DEFINE_KEY(NONCONVERT, 0x1D),
    SWARM_DEFINE_KEY(ACCEPT, 0x1E),
    SWARM_DEFINE_KEY(MODECHANGE, 0x1F),
    SWARM_DEFINE_KEY(SPACE, 0x20),
    SWARM_DEFINE_KEY(PRIOR, 0x21),
    SWARM_DEFINE_KEY(NEXT, 0x22),
    SWARM_DEFINE_KEY(END, 0x23),
    SWARM_DEFINE_KEY(HOME, 0x24),
    SWARM_DEFINE_KEY(LEFT, 0x25),
    SWARM_DEFINE_KEY(UP, 0x26),
    SWARM_DEFINE_KEY(RIGHT, 0x27),
    SWARM_DEFINE_KEY(DOWN, 0x28),
    SWARM_DEFINE_KEY(SELECT, 0x29),
    SWARM_DEFINE_KEY(PRINT, 0x2A),
    SWARM_DEFINE_KEY(EXECUTE, 0x2B),
    SWARM_DEFINE_KEY(SNAPSHOT, 0x2C),
    SWARM_DEFINE_KEY(INSERT, 0x2D),
    SWARM_DEFINE_KEY(DELETE, 0x2E),
    SWARM_DEFINE_KEY(HELP, 0x2F),
    SWARM_DEFINE_KEY(A, 0x41),
    SWARM_DEFINE_KEY(B, 0x42),
    SWARM_DEFINE_KEY(C, 0x43),
    SWARM_DEFINE_KEY(D, 0x44),
    SWARM_DEFINE_KEY(E, 0x45),
    SWARM_DEFINE_KEY(F, 0x46),
    SWARM_DEFINE_KEY(G, 0x47),
    SWARM_DEFINE_KEY(H, 0x48),
    SWARM_DEFINE_KEY(I, 0x49),
    SWARM_DEFINE_KEY(J, 0x4A),
    SWARM_DEFINE_KEY(K, 0x4B),
    SWARM_DEFINE_KEY(L, 0x4C),
    SWARM_DEFINE_KEY(M, 0x4D),
    SWARM_DEFINE_KEY(N, 0x4E),
    SWARM_DEFINE_KEY(O, 0x4F),
    SWARM_DEFINE_KEY(P, 0x50),
    SWARM_DEFINE_KEY(Q, 0x51),
    SWARM_DEFINE_KEY(R, 0x52),
    SWARM_DEFINE_KEY(S, 0x53),
    SWARM_DEFINE_KEY(T, 0x54),
    SWARM_DEFINE_KEY(U, 0x55),
    SWARM_DEFINE_KEY(V, 0x56),
    SWARM_DEFINE_KEY(W, 0x57),
    SWARM_DEFINE_KEY(X, 0x58),
    SWARM_DEFINE_KEY(Y, 0x59),
    SWARM_DEFINE_KEY(Z, 0x5A),
    SWARM_DEFINE_KEY(LWIN, 0x5B),
    SWARM_DEFINE_KEY(RWIN, 0x5C),
    SWARM_DEFINE_KEY(APPS, 0x5D),
    SWARM_DEFINE_KEY(SLEEP, 0x5F),
    SWARM_DEFINE_KEY(NUMPAD0, 0x60),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD1, 0x61),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD2, 0x62),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD3, 0x63),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD4, 0x64),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD5, 0x65),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD6, 0x66),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD7, 0x67),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD8, 0x68),    // wrong on win32????
    SWARM_DEFINE_KEY(NUMPAD9, 0x69),    // wrong on win32????
    SWARM_DEFINE_KEY(MULTIPLY, 0x6A),
    SWARM_DEFINE_KEY(ADD, 0x6B),
    SWARM_DEFINE_KEY(SEPARATOR, 0x6C),
    SWARM_DEFINE_KEY(SUBTRACT, 0x6D),
    SWARM_DEFINE_KEY(DECIMAL, 0x6E),
    SWARM_DEFINE_KEY(DIVIDE, 0x6F),
    SWARM_DEFINE_KEY(F1, 0x70),
    SWARM_DEFINE_KEY(F2, 0x71),
    SWARM_DEFINE_KEY(F3, 0x72),
    SWARM_DEFINE_KEY(F4, 0x73),
    SWARM_DEFINE_KEY(F5, 0x74),
    SWARM_DEFINE_KEY(F6, 0x75),
    SWARM_DEFINE_KEY(F7, 0x76),
    SWARM_DEFINE_KEY(F8, 0x77),
    SWARM_DEFINE_KEY(F9, 0x78),
    SWARM_DEFINE_KEY(F10, 0x79),
    SWARM_DEFINE_KEY(F11, 0x7A),
    SWARM_DEFINE_KEY(F12, 0x7B),
    SWARM_DEFINE_KEY(F13, 0x7C),
    SWARM_DEFINE_KEY(F14, 0x7D),
    SWARM_DEFINE_KEY(F15, 0x7E),
    SWARM_DEFINE_KEY(F16, 0x7F),
    SWARM_DEFINE_KEY(F17, 0x80),
    SWARM_DEFINE_KEY(F18, 0x81),
    SWARM_DEFINE_KEY(F19, 0x82),
    SWARM_DEFINE_KEY(F20, 0x83),
    SWARM_DEFINE_KEY(F21, 0x84),
    SWARM_DEFINE_KEY(F22, 0x85),
    SWARM_DEFINE_KEY(F23, 0x86),
    SWARM_DEFINE_KEY(F24, 0x87),
    SWARM_DEFINE_KEY(NUMLOCK, 0x90),
    SWARM_DEFINE_KEY(SCROLL, 0x91),
    SWARM_DEFINE_KEY(NUMPAD_EQUAL, 0x92),
    // SWARM_DEFINE_KEY(LSHIFT, 0xA0),
    // SWARM_DEFINE_KEY(RSHIFT, 0xA1),
    SWARM_DEFINE_KEY(LCONTROL, 0xA2),
    SWARM_DEFINE_KEY(RCONTROL, 0xA3),
    SWARM_DEFINE_KEY(LMENU, 0xA4),
    SWARM_DEFINE_KEY(RMENU, 0xA5),
    SWARM_DEFINE_KEY(SEMICOLON, 0xBA),
    SWARM_DEFINE_KEY(PLUS, 0xBB),
    SWARM_DEFINE_KEY(COMMA, 0xBC),
    SWARM_DEFINE_KEY(MINUS, 0xBD),
    SWARM_DEFINE_KEY(PERIOD, 0xBE),
    SWARM_DEFINE_KEY(SLASH, 0xBF),
    SWARM_DEFINE_KEY(GRAVE, 0xC0),
    SWARM_MAX_KEYS = 256
} SwarmKeyboardKey;

typedef struct _koncept_input_api {
    none (*init)(none);
    none (*shutdown)(none);

    none (*processMouseWheelInput)(i8 z);
    none (*processMouseMoveInput)(i16 x, i16 y);
    none (*processKeyInput)(SwarmKeyboardKey key, u8 pressed);
    none (*processMouseButtonInput)(SwarmMouseButton button, u8 pressed);

    none (*reset)(none);
    none (*update)(none);

    u8 (*keyIsUp)(SwarmKeyboardKey key);
    u8 (*keyWasUp)(SwarmKeyboardKey key);
    u8 (*keyIsDown)(SwarmKeyboardKey key);
    u8 (*keyWasDown)(SwarmKeyboardKey key);

    u8 (*buttonIsUp)(SwarmMouseButton button);
    u8 (*buttonWasUp)(SwarmMouseButton button);
    u8 (*buttonIsDown)(SwarmMouseButton button);
    u8 (*buttonWasDown)(SwarmMouseButton button);

    none (*mouseGetPosition)(i16* x, i16* y);
    none (*mouseGetLastPosition)(i16* x, i16* y);
} SwarmInputs;
extern SwarmInputs* swarmInputs;

#endif  // __SWARM_INPUT_H__