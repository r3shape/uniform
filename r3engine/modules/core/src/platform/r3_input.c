#include "../../include/platform/r3_input.h"
#include "../../include/platform/r3_event.h"
#include "../../include/memory/r3_memory.h"

static R3_Input_State internal_input_state = {0};

R3_Input_State* r3_init_input(void) {
    internal_input_state.init = R3_TRUE;
    return &internal_input_state;
}

void r3_shutdown_input(void) {
    internal_input_state.init = R3_FALSE;
}

void r3_update_input(f64 delta_time) {
    if (!internal_input_state.init) return;

    // TODO: r3_memory_api->copy() needed!
    memcpy(&internal_input_state.previous, &internal_input_state.current, sizeof(Lr3_Input_State));
    
    // reset current rawinput mouse deltas
    internal_input_state.current.mouse[0] = 0.0;
    internal_input_state.current.mouse[1] = 0.0;
}

ubyte r3_key_is_up(R3_Keyboard_Key key) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.current.keys[key] == R3_FALSE;
}

ubyte r3_key_was_up(R3_Keyboard_Key key) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.previous.keys[key] == R3_FALSE;
}

ubyte r3_key_is_down(R3_Keyboard_Key key) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.current.keys[key] == R3_TRUE;
}

ubyte r3_key_was_down(R3_Keyboard_Key key) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.previous.keys[key] == R3_TRUE;
}

ubyte r3_button_is_up(R3_Mouse_Button button) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.current.buttons[button] == R3_FALSE;
}

ubyte r3_button_was_up(R3_Mouse_Button button) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.previous.buttons[button] == R3_FALSE;
}

ubyte r3_button_is_down(R3_Mouse_Button button) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.current.buttons[button] == R3_TRUE;
}

ubyte r3_button_was_down(R3_Mouse_Button button) {
    if (!internal_input_state.init) return R3_FALSE;
    return internal_input_state.previous.buttons[button] == R3_TRUE;
}

void r3_mouse_get_position(sbyte2* x, sbyte2* y) {
    if (!internal_input_state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = internal_input_state.current.mouse[0]; *y = internal_input_state.current.mouse[1];
}

void r3_mouse_get_last_position(sbyte2* x, sbyte2* y) {
    if (!internal_input_state.init) {
        *x = 0; *y = 0;
        return;
    }; *x = internal_input_state.previous.mouse[0]; *y = internal_input_state.previous.mouse[1];
}

void r3_process_mouse_wheel_input(sbyte z_delta) {
    R3_Event event;
    event.sbyte[0] = z_delta;
    r3_event_api->push_event(event, R3_EVENT_MOUSE_WHEEL);
}

void r3_process_mouse_move_input(sbyte2 x, sbyte2 y) {
    if (x != 0 || y != 0) {
        internal_input_state.previous.mouse[0] = internal_input_state.current.mouse[0];
        internal_input_state.previous.mouse[1] = internal_input_state.current.mouse[1];

        internal_input_state.current.mouse[0] += x; // WM_INPUT returns mouse deltas so just accumulate them
        internal_input_state.current.mouse[1] += y;

        R3_Event event;
        event.ubyte2[0] = internal_input_state.current.mouse[0];
        event.ubyte2[1] = internal_input_state.current.mouse[1];
        r3_event_api->push_event(event, R3_EVENT_MOUSE_MOVE);
    }
}

void r3_process_key_input(R3_Keyboard_Key key, ubyte pressed) {
    if (internal_input_state.current.keys[key] != pressed) {
        internal_input_state.current.keys[key] = pressed;

        R3_Event event;
        event.ubyte2[0] = key;
        r3_event_api->push_event(event, pressed ? R3_EVENT_KEY_PRESSED : R3_EVENT_KEY_RELEASED);
    }
}

void r3_process_mouse_button_input(R3_Mouse_Button button, ubyte pressed) {
    if (internal_input_state.current.buttons[button] != pressed) {
        internal_input_state.current.buttons[button] = pressed;

        R3_Event event;
        event.ubyte2[0] = button;
        r3_event_api->push_event(event, pressed ? R3_EVENT_BUTTON_PRESSED : R3_EVENT_BUTTON_RELEASED);
    }
}
