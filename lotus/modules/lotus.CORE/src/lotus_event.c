#include "../include/platform/lotus_event.h"

static Lotus_Event_State internal_event_state = {0};

Lotus_Event_State* lotus_init_event() {
    internal_event_state.init = LOTUS_TRUE;
    return &internal_event_state;
}

void lotus_exit_event() {
    for (ubyte2 i = 0; i < LOTUS_EVENT_CODE_MAX; ++i) {
        lotus_destroy_array(internal_event_state.registered[i].events);
    }
}

ubyte lotus_push_event(Lotus_Event data, ubyte2 event_code) {
    if (!internal_event_state.init) return LOTUS_FALSE;
    if (internal_event_state.registered[event_code].events == NULL) return LOTUS_FALSE;

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[event_code].events, LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        Lotus_Event event = internal_event_state.registered[event_code].events[i];
        if(event.callback(data, event_code)) {
            // event handled
            return LOTUS_TRUE;
        }
    }

    // not found.
    return LOTUS_FALSE;
}

ubyte lotus_register_event_callback(ubyte2 event_code, Lotus_Event_Callback callback) {
    if (!internal_event_state.init) return LOTUS_FALSE;

    if (internal_event_state.registered[event_code].events == NULL) {
        internal_event_state.registered[event_code].events = lotus_make_array(sizeof(Lotus_Event), LOTUS_EVENT_CODE_MAX);
    }

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[event_code].events, LOTUS_ARRAY_LENGTH_FIELD);

    ubyte8 index = (count > 0) ? count : 0;
    Lotus_Event event = internal_event_state.registered[event_code].events[index];
    event.callback = callback;
    lotus_push_array(internal_event_state.registered[event_code].events, &event);

    return LOTUS_TRUE;
}

ubyte lotus_unregister_event_callback(ubyte2 event_code, Lotus_Event_Callback callback) {
    if (!internal_event_state.init) return LOTUS_FALSE;

    if (internal_event_state.registered[event_code].events == NULL) {
        // TODO: warn
        return LOTUS_FALSE;
    }

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.registered[event_code].events, LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        internal_event_state.registered[event_code].events[i].callback = NULL;
        return LOTUS_TRUE;
    }

    // not found.
    return LOTUS_FALSE;
}
