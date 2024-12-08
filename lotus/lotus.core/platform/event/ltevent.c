#include "ltevent.h"
#include "../../memory/ltmemory.h"
#include "../../utility/ltarray.h"

#define LOTUS_EVENT_CODE_MAX 16384

typedef struct tagRegisteredEvent {
    void* listener;
    onEvent_FPTR callback;
} LTregisteredEvent;

typedef struct tagEventCodeEntry {
    LTregisteredEvent* events;
} LTeventCodeEntry;

typedef struct tagEventInternal {
    b8 init;
    // event code lookup-table.
    LTeventCodeEntry registered[LOTUS_EVENT_CODE_MAX];
} LTeventInternal;
static LTeventInternal state = {0};


b8 ltEventInit(void) {
    ltMemZero(&state, sizeof(state));
    state.init = LOTUS_TRUE;
    return LOTUS_TRUE;
}

void ltEventExit(void) {
    for (u16 i = 0; i < LOTUS_EVENT_CODE_MAX; ++i) {
        if (state.registered[i].events != 0) {
            ltDestroyDArray(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

b8 ltPushEvent(LTeventData data, u16 eventCode, void* sender) {
    if (!state.init) return LOTUS_FALSE;
    if (state.registered[eventCode].events == 0) return LOTUS_FALSE;

    u64 count = ltGetDArrayField(LOTUS_ARRFIELD_LENGTH, state.registered[eventCode].events);
    for (u64 i = 0; i < count; ++i) {
        LTregisteredEvent e = state.registered[eventCode].events[i];
        if(e.callback(data, eventCode, sender, e.listener)) {
            // event handled
            return LOTUS_TRUE;
        }
    }

    // not found.
    return LOTUS_FALSE;
}

b8 ltRegisterEvent(u16 eventCode, void* listener, onEvent_FPTR onEvent) {
    if (!state.init) return LOTUS_FALSE;

    if (state.registered[eventCode].events == 0) {
        state.registered[eventCode].events = ltMakeDArray(LOTUS_EVENT_CODE_MAX, sizeof(LTregisteredEvent));
    }

    u64 count = ltGetDArrayField(LOTUS_ARRFIELD_LENGTH, state.registered[eventCode].events);
    for (u64 i = 0; i < count; ++i) {
        if(state.registered[eventCode].events[i].listener == listener) {
            // TODO: warn
            return LOTUS_FALSE;
        }
    }

    // no duplicate listener found, continue with registration
    u64 index = (count > 0) ? count : 0;
    LTregisteredEvent event = state.registered[eventCode].events[index];
    event.callback = onEvent;
    event.listener = listener;
    ltPushDArray(&event, state.registered[eventCode].events);

    return LOTUS_TRUE;
}

b8 ltUnregisterEvent(u16 eventCode, void* listener, onEvent_FPTR onEvent) {
    if (!state.init) return LOTUS_FALSE;

    if (state.registered[eventCode].events == 0) {
        // TODO: warn
        return LOTUS_FALSE;
    }

    u64 count = ltGetDArrayField(LOTUS_ARRFIELD_LENGTH, state.registered[eventCode].events);
    for (u64 i = 0; i < count; ++i) {
        LTregisteredEvent e = state.registered[eventCode].events[i];
        if (e.listener == listener &&  e.callback == onEvent) {
            // found an event code listener, remove it
            LTregisteredEvent popped;
            ltPopAtDArray(i, &popped, state.registered[eventCode].events);
            return LOTUS_TRUE;
        }
    }

    // not found.
    return LOTUS_FALSE;
}
