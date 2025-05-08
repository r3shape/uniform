#include <include/swarm/core/swarm_events.h>

static struct _EventInternal {
    u16 events;
    u8 eventv[SWARM_EVENT_CODE_MAX];
    SwarmCallback* callbackv[SWARM_EVENT_CODE_MAX];
} _EventInternal = {0};

u8 _registerEventImpl(u16 eventCode) {
    if (eventCode >= SWARM_EVENT_CODE_MAX || _EventInternal.eventv[eventCode]) 
        return SSDK_FALSE;

    _EventInternal.eventv[eventCode] = SSDK_TRUE;
    _EventInternal.callbackv[eventCode] = saneData->array.create(sizeof(SwarmCallback), SWARM_CALLBACK_MAX);

    _EventInternal.events++;
    return SSDK_TRUE;
}

u8 _unregisterEventImpl(u16 eventCode) {
    if (eventCode >= SWARM_EVENT_CODE_MAX || !_EventInternal.eventv[eventCode]) 
        return SSDK_FALSE;

    _EventInternal.eventv[eventCode] = SSDK_FALSE;
    
    if (_EventInternal.callbackv[eventCode]) {
        saneData->array.destroy(_EventInternal.callbackv[eventCode]);
        _EventInternal.callbackv[eventCode] = NULL;
    }

    _EventInternal.events--;
    return SSDK_TRUE;
}

u8 _pushEventImpl(u16 eventCode, SwarmEvent data) {
    if (eventCode >= SWARM_EVENT_CODE_MAX || !_EventInternal.eventv[eventCode]) 
        return SSDK_FALSE;

    if (!_EventInternal.callbackv[eventCode]) return SSDK_FALSE;

    u8 result = SSDK_FALSE;
    ArrayHeader head = saneData->array.getHeader(_EventInternal.callbackv[eventCode]);
    SSDK_FORI(0, head.count, 1) {
        result = _EventInternal.callbackv[eventCode][i](eventCode, data);
    }

    return result;
}

u8 _registerCallbackImpl(u16 eventCode, SwarmCallback callback) {
    if (eventCode >= SWARM_EVENT_CODE_MAX || !_EventInternal.eventv[eventCode]) 
        return SSDK_FALSE;

    if (!_EventInternal.callbackv[eventCode]) 
        return SSDK_FALSE;

    saneData->array.push(_EventInternal.callbackv[eventCode], &callback);
    return SSDK_TRUE;
}

u8 _unregisterCallbackImpl(u16 eventCode, SwarmCallback callback) {
    if (eventCode >= SWARM_EVENT_CODE_MAX || !_EventInternal.eventv[eventCode]) 
        return SSDK_FALSE;

    ArrayHeader head = saneData->array.getHeader(_EventInternal.callbackv[eventCode]);
    if (head.count == 0) return SSDK_FALSE;

    SSDK_FORI(0, head.count, 1) {
        if (_EventInternal.callbackv[eventCode][i] == callback) {
            SwarmCallback callback;
            saneData->array.pull(_EventInternal.callbackv[eventCode], i, &callback);
            return SSDK_TRUE;
        }
    }
    return SSDK_FALSE;
}


none _initEvents(none) {
    SSDK_FORI(0, SWARM_EVENT_CODES, 1) {
        if (!swarmEvents->registerEvent(i)) {
            saneLog->logFmt(SANE_LOG_ERROR, "[Events] Failed to register: %u", i);
        } else saneLog->logFmt(SANE_LOG_SUCCESS, "[Events] Registered: %u", i);
    }
    saneLog->log(SANE_LOG_SUCCESS, "[Events] Initialized");
}

none _shutdownEvents(none) {
    SSDK_FORI(0, SWARM_EVENT_CODE_MAX, 1) {
        if (_EventInternal.eventv[i]) {
            swarmEvents->unregisterEvent(i);
        }
    }
    saneLog->log(SANE_LOG_SUCCESS, "[Events] Shutdown");
}

SwarmEvents* swarmEvents = &(SwarmEvents){
    .init = _initEvents,
    .shutdown = _shutdownEvents,
    
    .pushEvent = _pushEventImpl,
    .registerEvent = _registerEventImpl,
    .unregisterEvent = _unregisterEventImpl,
    
    .registerCallback = _registerCallbackImpl,
    .unregisterCallback = _unregisterCallbackImpl
};
