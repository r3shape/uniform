#include "ltinput.h"
#include "../ltlogger.h"
#include "../event/ltevent.h"
#include "../../memory/ltmemory.h"

typedef struct tagInputState {
    i16 mouse[2];
    b8 keys[256];
    u8 buttons[LOTUS_MBUTTON_MAX_BUTTONS];
} LTinputState;

typedef struct tagInputInternal {
    b8 init;
    LTinputState current;
    LTinputState previous;
} LTinputInternal;
static LTinputInternal state = {0};

void ltInputExit(void) {
    state.init = LOTUS_FALSE;
}

b8 ltInputInit(void) {
    ltMemZero(&state, sizeof(LTinputInternal));
    state.init = LOTUS_TRUE;

    return state.init;
}

void ltInputUpdate(f64 delta_time) {
    if (!state.init) return;
    ltMemCopy(&state.previous, &state.current, sizeof(LTinputState));
}

b8 ltIsKeyUp(LTkeyboardKey key) {
    if (!state.init) return LOTUS_FALSE;
    return state.current.keys[key] == LOTUS_FALSE;
}

b8 ltWasKeyUp(LTkeyboardKey key) {
    if (!state.init) return LOTUS_FALSE;
    return state.previous.keys[key] == LOTUS_FALSE;
}

b8 ltIsKeyDown(LTkeyboardKey key) {
    if (!state.init) return LOTUS_FALSE;
    return state.current.keys[key] == LOTUS_TRUE;
}

b8 ltWasKeyDown(LTkeyboardKey key) {
    if (!state.init) return LOTUS_FALSE;
    return state.previous.keys[key] == LOTUS_TRUE;
}

b8 ltIsButtonUp(LTmouseButton button) {
    if (!state.init) return LOTUS_FALSE;
    return state.current.buttons[button] == LOTUS_FALSE;
}

b8 ltWasButtonUp(LTmouseButton button) {
    if (!state.init) return LOTUS_FALSE;
    return state.previous.buttons[button] == LOTUS_FALSE;
}

b8 ltIsButtonDown(LTmouseButton button) {
    if (!state.init) return LOTUS_FALSE;
    return state.current.buttons[button] == LOTUS_TRUE;
}

b8 ltWasButtonDown(LTmouseButton button) {
    if (!state.init) return LOTUS_FALSE;
    return state.previous.buttons[button] == LOTUS_TRUE;
}


void ltGetMousePosition(i32* x, i32* y) {
    if (!state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = state.current.mouse[0]; *y = state.current.mouse[1];
}

void ltGetPreviousMousePosition(i32* x, i32* y) {
    if (!state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = state.previous.mouse[0]; *y = state.previous.mouse[1];
}


void ltInputProcessMouseWheel(i8 z_delta) {
    LTeventData event;
    event.data.u8[0] = z_delta;
    ltPushEvent(event, LOTUS_EVENT_MOUSE_WHEEL, 0);
}

void ltInputProcessMouseMove(i16 x, i16 y) {
    if (state.current.mouse[0] != x || state.current.mouse[1] != y) {
        state.current.mouse[0] = x;
        state.current.mouse[1] = y;

        LTeventData event;
        event.data.u16[0] = x;
        event.data.u16[1] = y;
        ltPushEvent(event, LOTUS_EVENT_MOUSE_MOVE, 0);
    }
}

void ltInputProcessKey(LTkeyboardKey key, b8 pressed) {
    if (state.current.keys[key] != pressed) {
        state.current.keys[key] = pressed;

        LTeventData event;
        event.data.u16[0] = key;
        ltPushEvent(event, pressed ? LOTUS_EVENT_KEY_PRESSED : LOTUS_EVENT_KEY_RELEASED, 0);
    }
}

void ltInputProcessButton(LTmouseButton button, b8 pressed) {
    if (state.current.buttons[button] != pressed) {
        state.current.buttons[button] = pressed;

        LTeventData event;
        event.data.u16[0] = button;
        ltPushEvent(event, pressed ? LOTUS_EVENT_BUTTON_PRESSED : LOTUS_EVENT_BUTTON_RELEASED, 0);
    }
}
