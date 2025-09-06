#include <ufcore/UFEVIN.h>

typedef struct UFDevice {
    UFDeviceType type;
    ptr buffers[2];
} UFDevice;

static struct UFEVINInternal {
    Array hook;
    Array event;
    Array device;
} UFEVINInternal = {0};

static inline u64 _ufDeviceBufferSize(UFDeviceType type) {
    // compute buffer size
    switch (type) {
        case (UF_DEVICE_MOUSE): return (sizeof(s16) * 2) + (sizeof(u8) * UF_MAX_MOUSE_BUTTONS);
        case (UF_DEVICE_KEYBOARD): return (sizeof(u8) * UF_MAX_KEYBOARD_KEYS);
        case (UF_DEVICE_CONTROLLER):    // fall-through to default
        default: {
            r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `_ufDeviceBufferSize` -- device type not supported: %d\n", type);
            return 0;
        }
    }
}

u8 newEvent(UFEventCode code) {
    if (code >= UF_EVENT_CODE_MAX || ((u8*)UFEVINInternal.event.data)[code]) {
        r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `newEvent` -- event code already exists: %d\n", code);
        return 0;
    }

    if (!r3_arr_write(code, &(u8){1}, &UFEVINInternal.event)) {
        r3_log_stdout(ERROR_LOG, "[UFEVIN] Failed `newEvent` -- event code array write failed\n");
        return 0;
    }

    r3_log_stdoutf(SUCCESS_LOG, "[UFEVIN] New event code: %d\n", code);
    return 1;
}

u8 delEvent(UFEventCode code) {
    if (code >= UF_EVENT_CODE_MAX || !((u8*)UFEVINInternal.event.data)[code]) {
        r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `delEvent` -- event code does not exist: %d\n", code);
        return 0;
    }

    if (!r3_arr_write(code, &(u8){0}, &UFEVINInternal.event)) {
        r3_log_stdout(ERROR_LOG, "[UFEVIN] Failed `delEvent` -- event code array write failed\n");
        return 0;
    }

    return 1;
}

u8 sendEvent(UFEventCode code, UFEvent event) {
    if (code >= UF_EVENT_CODE_MAX || !((u8*)UFEVINInternal.event.data)[code]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `sendEvent` -- event code not found: %d\n", code);
        return 0;
    }

    if (!((Array*)(UFEVINInternal.hook.data))[code].data) {
        r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `sendEvent` -- event code has no hooks: %d\n", code);
        return 0;
    }
    
    u16 count = r3_arr_count(&((Array*)(UFEVINInternal.hook.data))[code]);
    FOR_I(0, count, 1) {
        UFEventHook hook = 0;
        if (r3_arr_read(i, &hook, &((Array*)(UFEVINInternal.hook.data))[code]) && !hook) {
            r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `sendEvent` -- event hook array read failed\n");
            return 0;
        } else { hook(code, event); }
    }

    return 1;
}


u8 newHook(UFEventCode code, UFEventHook hook) {
    if (code >= UF_EVENT_CODE_MAX || !((u8*)UFEVINInternal.event.data)[code]) {
        r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `newHook` -- event code not found: %d\n", code);
        return 0;
    }

    if (!r3_arr_alloc(UF_EVENT_HOOK_MAX, sizeof(UFEventHook), &((Array*)(UFEVINInternal.hook.data))[code])) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `newHook` -- event hook array alloc failed: %d\n", code);
        return 0;
    }

    u16 count = r3_arr_count(&((Array*)(UFEVINInternal.hook.data))[code]);
    if (!r3_arr_assign(count, hook, &((Array*)(UFEVINInternal.hook.data))[code])) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `newHook` -- event hook array push failed: %d\n", code);
        return 0;
    }

    r3_log_stdoutf(SUCCESS_LOG, "[UFEVIN] New event hook: %d\n", code);
    return 1;
}

u8 delHook(UFEventCode code, UFEventHook hook) {
    if (code >= UF_EVENT_CODE_MAX || !((u8*)UFEVINInternal.event.data)[code]) {
        r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `delHook` -- event code not found: %d\n", code);
        return 0;
    }

    u16 count = r3_arr_count(&((Array*)UFEVINInternal.hook.data)[code]);
    if (!count) {
        r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `delHook` -- event code has no hooks: %d\n", code);
        return 0;
    }

    FOR_I(0, count, 1) {
        UFEventHook temp = {0}; // temp hook for pull
        if (((UFEventHook*)((Array*)(UFEVINInternal.hook.data))[code].data)[i] == hook) {
            if (!r3_arr_pull(i, &temp, &((Array*)(UFEVINInternal.hook.data))[code])) {   // shifts hook array left
                r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `newHook` -- event hook array pull failed: %d\n", code);
                return 0;
            } return 1;   // hook found
        }
    }

    r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delHook` -- event hook not found: %d\n", code);
    return 0;   // hook not found
}


UFResource newDevice(UFDeviceType type) {
    if (type >= UF_DEVICE_COUNT) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `newDevice` -- invalid device type\n");
    }
    
    // write to device array
    u64 size =_ufDeviceBufferSize(type);
    u8 count = r3_arr_count(&UFEVINInternal.device);
    if (!r3_arr_write(count, &(UFDevice){
        .type = type,
        .buffers[0] = NULL,
        .buffers[1] = NULL
    }, &UFEVINInternal.device)) {
        r3_log_stdout(ERROR_LOG, "[UFEVIN] Failed `newDevice` -- device array assign failed\n");
        return 0;
    }

    // alloc device buffers
    ((UFDevice*)UFEVINInternal.device.data)[count].buffers[0] = r3_mem_alloc(size, 8);
    ((UFDevice*)UFEVINInternal.device.data)[count].buffers[1] = r3_mem_alloc(size, 8);
    
    if (!((UFDevice*)UFEVINInternal.device.data)[count].buffers[0] ||
        !((UFDevice*)UFEVINInternal.device.data)[count].buffers[1]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `newDevice` -- device buffer alloc failed: %p\t%p\n", ((UFDevice*)UFEVINInternal.device.data)[count].buffers[0], ((UFDevice*)UFEVINInternal.device.data)[count].buffers[1]);
        return 0;
    }

    return r3_arr_count(&UFEVINInternal.device);
}

none delDevice(UFResource device) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    if (!device || device > count || device > UF_DEVICE_MAX || !((ptr*)UFEVINInternal.device.data)[device - 1]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delDevice` -- device not found: %d\n", device);
        return;
    }

    // pull from device array
    UFDevice devicePull  = {0};
    if (!r3_arr_pull(device - 1, &devicePull, &UFEVINInternal.device)) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delDevice` -- device array pull failed: %d\n", device);
        return;
    }
    
    // dealloc device buffers
    r3_mem_dealloc(devicePull.buffers[0]);
    r3_mem_dealloc(devicePull.buffers[1]);
}


u8 setDeviceState(UFDeviceState state, UFDeviceBuffer buffer, UFResource device) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    if (!device || device > count || device > UF_DEVICE_MAX || !((ptr*)UFEVINInternal.device.data)[device - 1]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `setDeviceState` -- device not found: %d\n", device);
        return 0;
    } if (buffer >= UF_DEVICE_BUFFER_COUNT) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `setDeviceState` -- device buffer not found: %d\n", buffer);
        return 0;
    } if (state >= UF_DEVICE_STATE_COUNT) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `setDeviceState` -- device state not found: %d\n", state);
        return 0;
    }

    UFDevice deviceRead  = {0};
    if (!r3_arr_read(device - 1, &deviceRead, &UFEVINInternal.device)) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delDevice` -- device array read failed: %d\n", device);
        return 0;
    }
    
    // mem set entire buffer to the passed state
    u64 size =_ufDeviceBufferSize(deviceRead.type);
    r3_mem_set(size, state, ((UFDevice*)UFEVINInternal.device.data)[device - 1].buffers[buffer]);

    return 1;
}

UFDeviceState getKeyboardState(UFKeyboardKey key, UFDeviceBuffer buffer, UFResource device) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    if (!device || device > count || device > UF_DEVICE_MAX || !((ptr*)UFEVINInternal.device.data)[device - 1]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `getKeyboardState` -- device not found: %d\n", device);
        return 0;
    } if (buffer >= UF_DEVICE_BUFFER_COUNT) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `getKeyboardState` -- device buffer not found: %d\n", buffer);
        return 0;
    } if (key >= UF_MAX_KEYBOARD_KEYS) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `getKeyboardState` -- keyboard key not found: %d\n", key);
        return 0;
    }

    UFDevice deviceRead  = {0};
    if (!r3_arr_read(device - 1, &deviceRead, &UFEVINInternal.device)) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delDevice` -- device array read failed: %d\n", device);
        return 0;
    }

    return ((u8*)deviceRead.buffers[buffer])[key];
}

UFDeviceState getMouseState(UFMouseButton button, UFDeviceBuffer buffer, UFResource device) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    if (!device || device > count || device > UF_DEVICE_MAX || !((ptr*)UFEVINInternal.device.data)[device - 1]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `getKeyboardState` -- device not found: %d\n", device);
        return 0;
    } if (buffer >= UF_DEVICE_BUFFER_COUNT) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `getKeyboardState` -- device buffer not found: %d\n", buffer);
        return 0;
    } if (button >= UF_MAX_MOUSE_BUTTONS) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `getKeyboardState` -- mouse button not found: %d\n", button);
        return 0;
    }

    UFDevice deviceRead  = {0};
    if (!r3_arr_read(device - 1, &deviceRead, &UFEVINInternal.device)) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delDevice` -- device array read failed: %d\n", device);
        return 0;
    }

    return ((u8*)deviceRead.buffers[buffer] + (sizeof(u16) * 2))[button];
}

u8 resetDeviceState(UFDeviceBuffer buffer, UFResource device) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    if (!device || device > count || device > UF_DEVICE_MAX || !((ptr*)UFEVINInternal.device.data)[device - 1]) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `resetDeviceState` -- device not found: %d\n", device);
        return 0;
    } if (buffer >= UF_DEVICE_BUFFER_COUNT) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `resetDeviceState` -- device buffer not found: %d\n", buffer);
        return 0;
    }

    UFDevice deviceRead  = {0};
    if (!r3_arr_read(device - 1, &deviceRead, &UFEVINInternal.device)) {
        r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `delDevice` -- device array read failed: %d\n", device);
        return 0;
    }

    // mem set entire buffer to the inactive state
    u64 size =_ufDeviceBufferSize(deviceRead.type);
    r3_mem_set(size, UF_DEVICE_STATE_INACTIVE, ((UFDevice*)UFEVINInternal.device.data)[device - 1].buffers[buffer]);

    return 1;
}



u8 resetDevices(none) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    FOR_I(1, count + 1, 1) {
        resetDeviceState(UF_DEVICE_BUFFER_NOW, i);
        resetDeviceState(UF_DEVICE_BUFFER_LAST, i);
    }

    return 1;
}

u8 updateDevices(none) {
    u8 count = r3_arr_count(&UFEVINInternal.device);
    FOR_I(0, count, 1) {
        UFDevice deviceRead  = {0};
        if (!r3_arr_read(i, &deviceRead, &UFEVINInternal.device)) {
            r3_log_stdoutf(ERROR_LOG, "[UFEVIN] Failed `updateDevices` -- device array read failed: %d\n", i);
            return 0;
        }
        
        // write now buffer to last buffer
        u64 size =_ufDeviceBufferSize(deviceRead.type);
        if (!r3_mem_write(size, deviceRead.buffers[UF_DEVICE_BUFFER_NOW], deviceRead.buffers[UF_DEVICE_BUFFER_LAST])) {
            r3_log_stdoutf(WARN_LOG, "[UFEVIN] Failed `updateDevices` -- device array mem write failed: %d\n", deviceRead.type);
            return 0;
        }
    }

    return 1;
}

u8 ufInitEVIN(UFEVINInterface* evinInterface) {
    if (!r3_arr_alloc(UF_EVENT_CODE_MAX, sizeof(u8), &UFEVINInternal.event)     ||
        !r3_arr_alloc(UF_DEVICE_MAX, sizeof(UFDevice), &UFEVINInternal.device)  ||
        !r3_arr_alloc(UF_EVENT_CODE_MAX, sizeof(UFEventHook), &UFEVINInternal.hook)) {
            r3_log_stdout(ERROR_LOG, "[UFEVIN] Failed to init -- internal array alloc failed\n");
            return 0;
    }

    evinInterface->newEvent = newEvent;
    evinInterface->delEvent = delEvent;
    evinInterface->sendEvent = sendEvent;

    evinInterface->newHook = newHook;
    evinInterface->delHook = delHook;
    
    evinInterface->newDevice = newDevice;
    evinInterface->delDevice = delDevice;
    
    evinInterface->resetDevices = resetDevices;
    evinInterface->updateDevices = updateDevices;
    
    evinInterface->setDeviceState = setDeviceState;
    evinInterface->getMouseState = getMouseState;
    evinInterface->getKeyboardState = getKeyboardState;
    evinInterface->resetDeviceState = resetDeviceState;

    r3_log_stdout(SUCCESS_LOG, "[UFEVIN] Initialized\n");
    return 1;
}

u8 ufExitEVIN(UFEVINInterface* evinInterface) {
    if (!r3_arr_dealloc(&UFEVINInternal.device) ||
        !r3_arr_dealloc(&UFEVINInternal.event)  ||
        !r3_arr_dealloc(&UFEVINInternal.hook)) {
            r3_log_stdout(ERROR_LOG, "[UFEVIN] Failed to exit -- internal array alloc failed\n");
            return 0;
    }

    evinInterface->newEvent = NULL;
    evinInterface->delEvent = NULL;
    evinInterface->sendEvent = NULL;

    evinInterface->newHook = NULL;
    evinInterface->delHook = NULL;
    
    evinInterface->newDevice = NULL;
    evinInterface->delDevice = NULL;
    
    evinInterface->resetDevices = NULL;
    evinInterface->updateDevices = NULL;
    
    evinInterface->setDeviceState = NULL;
    evinInterface->getMouseState = NULL;
    evinInterface->getKeyboardState = NULL;
    evinInterface->resetDeviceState = NULL;

    r3_log_stdout(SUCCESS_LOG, "[UFEVIN] Deinitialized\n");
    return 1;
}

