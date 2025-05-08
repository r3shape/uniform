#include <include/swarm/core/swarm_inputs.h>
#include <include/swarm/core/swarm_events.h>

static struct _InputsInternal {
    struct devices {
        u8 mouseButtons[SWARM_MOUSE_MAX_BUTTONS];
        u8 keyboard[SWARM_MAX_KEYS];
        i16 mouseDelta[2];
    } devices[2];
} _InputsInternal = {0};


void _updateImpl(void) {
    memcpy(&_InputsInternal.devices[0], &_InputsInternal.devices[1], sizeof(_InputsInternal.devices[1]));
    
    // reset current rawinput mouse deltas
    _InputsInternal.devices[1].mouseDelta[0] = 0.0;
    _InputsInternal.devices[1].mouseDelta[1] = 0.0;
}

void _resetImpl(void) {
    SSDK_FORJ(0, 2, 1) {
        _InputsInternal.devices[j].mouseDelta[0] = 0;
        _InputsInternal.devices[j].mouseDelta[1] = 0;
    }
    
    SSDK_FORJ(0, 2, 1)
        SSDK_FORI(0, 256, 1)
            _InputsInternal.devices[j].keyboard[i] = 0;
    SSDK_FORJ(0, 2, 1)
        SSDK_FORI(0, SWARM_MOUSE_MAX_BUTTONS, 1)
            _InputsInternal.devices[j].mouseButtons[i] = 0;
}

u8 _keyIsUpImpl(SwarmKeyboardKey key) {
    return _InputsInternal.devices[1].keyboard[key] == SSDK_FALSE;
}

u8 _keyWasUpImpl(SwarmKeyboardKey key) {
    return _InputsInternal.devices[0].keyboard[key] == SSDK_FALSE;
}

u8 _keyIsDownImpl(SwarmKeyboardKey key) {
    return _InputsInternal.devices[1].keyboard[key] == SSDK_TRUE;
}

u8 _keyWasDownImpl(SwarmKeyboardKey key) {
    return _InputsInternal.devices[0].keyboard[key] == SSDK_TRUE;
}

u8 _buttonIsUpImpl(SwarmMouseButton button) {
    return _InputsInternal.devices[1].mouseButtons[button] == SSDK_FALSE;
}

u8 _buttonWasUpImpl(SwarmMouseButton button) {
    return _InputsInternal.devices[0].mouseButtons[button] == SSDK_FALSE;
}

u8 _buttonIsDownImpl(SwarmMouseButton button) {
    return _InputsInternal.devices[1].mouseButtons[button] == SSDK_TRUE;
}

u8 _buttonWasDownImpl(SwarmMouseButton button) {
    return _InputsInternal.devices[0].mouseButtons[button] == SSDK_TRUE;
}

void _mouseGetPositionImpl(i16* x, i16* y) {
    *x = _InputsInternal.devices[1].mouseDelta[0]; *y = _InputsInternal.devices[1].mouseDelta[1];
}

void _mouseGetLastPositionImpl(i16* x, i16* y) {
    *x = _InputsInternal.devices[0].mouseDelta[0]; *y = _InputsInternal.devices[0].mouseDelta[1];
}

void _processMouseWheelInputImpl(i8 z_delta) {
    SwarmEvent event;
    event.i8[0] = z_delta;
    swarmEvents->pushEvent(SWARM_EVENT_MOUSE_WHEEL, event);
}

void _processMouseMoveInputImpl(i16 x, i16 y) {
    if (x != 0 || y != 0) {
        _InputsInternal.devices[0].mouseDelta[0] = _InputsInternal.devices[1].mouseDelta[0];
        _InputsInternal.devices[0].mouseDelta[1] = _InputsInternal.devices[1].mouseDelta[1];

        _InputsInternal.devices[1].mouseDelta[0] += x; // WM_INPUT returns mouse deltas so just accumulate them
        _InputsInternal.devices[1].mouseDelta[1] += y;

        SwarmEvent event;
        event.u16[0] = _InputsInternal.devices[1].mouseDelta[0];
        event.u16[1] = _InputsInternal.devices[1].mouseDelta[1];
        swarmEvents->pushEvent(SWARM_EVENT_MOUSE_MOVE, event);
    }
}

void _processKeyInputImpl(SwarmKeyboardKey key, u8 pressed) {
    if (_InputsInternal.devices[1].keyboard[key] != pressed) {
        _InputsInternal.devices[1].keyboard[key] = pressed;

        SwarmEvent event;
        event.u16[0] = key;
        swarmEvents->pushEvent(pressed ? SWARM_EVENT_KEY_PRESSED : SWARM_EVENT_KEY_RELEASED, event);
    }
}

void _processMouseButtonInputImpl(SwarmMouseButton button, u8 pressed) {
    if (_InputsInternal.devices[1].mouseButtons[button] != pressed) {
        _InputsInternal.devices[1].mouseButtons[button] = pressed;

        SwarmEvent event;
        event.u16[0] = button;
        swarmEvents->pushEvent(pressed ? SWARM_EVENT_BUTTON_PRESSED : SWARM_EVENT_BUTTON_RELEASED, event);
    }
}

none _initInputs(none) {
    saneLog->log(SANE_LOG_SUCCESS, "[Inputs] Initialized");
}

none _shutdownInputs(none) {
    saneLog->log(SANE_LOG_SUCCESS, "[Inputs] Shutdown");
}

SwarmInputs* swarmInputs = &(SwarmInputs) {
    .init = _initInputs,
    .shutdown = _shutdownInputs,

    .reset = _resetImpl,
    .update = _updateImpl,
    
    .keyIsUp = _keyIsUpImpl,
    .keyWasUp = _keyWasUpImpl,
    .keyIsDown = _keyIsDownImpl,
    .keyWasDown = _keyWasDownImpl,
    
    .buttonIsUp = _buttonIsUpImpl,
    .buttonWasUp = _buttonWasUpImpl,
    .buttonIsDown = _buttonIsDownImpl,
    .buttonWasDown = _buttonWasDownImpl,
    .mouseGetPosition = _mouseGetPositionImpl,
    .mouseGetLastPosition = _mouseGetLastPositionImpl,

    .processKeyInput = _processKeyInputImpl,
    .processMouseMoveInput = _processMouseMoveInputImpl,
    .processMouseWheelInput = _processMouseWheelInputImpl,
    .processMouseButtonInput = _processMouseButtonInputImpl
};


