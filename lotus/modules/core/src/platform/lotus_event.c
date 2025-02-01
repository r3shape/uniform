#include "../../include/platform/lotus_event.h"
#include "../../include/utility/lotus_array.h"

Lotus_Event_API* lotus_event_api = NULL;
static struct internal_event_state {
    ubyte2 event_count;
    ubyte events[LOTUS_EVENT_CODE_MAX];
    Lotus_Event_Callback* callbacks[LOTUS_EVENT_CODE_MAX];
} internal_event_state = {0};


ubyte _register_event_impl(ubyte2 event_code) {
    if (event_code >= LOTUS_EVENT_CODE_MAX || internal_event_state.events[event_code]) 
        return LOTUS_FALSE;

    internal_event_state.events[event_code] = LOTUS_TRUE;
    internal_event_state.callbacks[event_code] = lotus_create_array(sizeof(Lotus_Event_Callback), 2);

    internal_event_state.event_count++;
    return LOTUS_TRUE;
}

ubyte _unregister_event_impl(ubyte2 event_code) {
    if (event_code >= LOTUS_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LOTUS_FALSE;

    internal_event_state.events[event_code] = LOTUS_FALSE;
    
    if (internal_event_state.callbacks[event_code]) {
        lotus_destroy_array(internal_event_state.callbacks[event_code]);
        internal_event_state.callbacks[event_code] = NULL;
    }

    internal_event_state.event_count--;
    return LOTUS_TRUE;
}

ubyte _push_event_impl(Lotus_Event data, ubyte2 event_code) {
    if (event_code >= LOTUS_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LOTUS_FALSE;

    if (!internal_event_state.callbacks[event_code]) return LOTUS_FALSE;

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.callbacks[event_code], LOTUS_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        if (internal_event_state.callbacks[event_code][i](data, event_code)) {
            return LOTUS_TRUE;
        }
    }

    return LOTUS_FALSE;
}

ubyte _register_callback_impl(ubyte2 event_code, Lotus_Event_Callback callback) {
    if (event_code >= LOTUS_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LOTUS_FALSE;

    if (!internal_event_state.callbacks[event_code]) 
        return LOTUS_FALSE;

    lotus_push_array(internal_event_state.callbacks[event_code], &callback);
    return LOTUS_TRUE;
}

ubyte _unregister_callback_impl(ubyte2 event_code, Lotus_Event_Callback callback) {
    if (event_code >= LOTUS_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LOTUS_FALSE;

    ubyte8 count = LOTUS_ARRAY_GET_HEADER_FIELD(internal_event_state.callbacks[event_code], LOTUS_ARRAY_LENGTH_FIELD);
    if (count == 0) return LOTUS_FALSE;

    Lotus_Event_Callback* callbacks = internal_event_state.callbacks[event_code];

    for (ubyte8 i = 0; i < count; ++i) {
        if (callbacks[i] == callback) {
            for (ubyte8 j = i; j < count - 1; ++j) {
                callbacks[j] = callbacks[j + 1];
            }

            Lotus_Event_Callback callback;
            lotus_pop_array(internal_event_state.callbacks[event_code], callback);

            return LOTUS_TRUE;
        }
    }
    return LOTUS_FALSE;
}


ubyte lotus_init_events(void) {
    lotus_event_api = (Lotus_Event_API*)lotus_memory_api->alloc(sizeof(Lotus_Event_API), 16);
    if (!lotus_event_api) return LOTUS_FALSE;

    lotus_event_api->push_event = _push_event_impl;
    lotus_event_api->register_event = _register_event_impl;
    lotus_event_api->unregister_event = _unregister_event_impl;
    lotus_event_api->register_callback = _register_callback_impl;
    lotus_event_api->unregister_callback = _unregister_callback_impl;

    for (ubyte2 i = 0; i < LOTUS_EVENT_CODES; i++) {
        if (!lotus_event_api->register_event(i)) {
            lotus_shutdown_events();
            return LOTUS_FALSE;
        }
    }

    return LOTUS_TRUE;
}

void lotus_shutdown_events(void) {
    for (ubyte2 i = 0; i < LOTUS_EVENT_CODE_MAX; ++i) {
        if (internal_event_state.events[i]) {
            lotus_event_api->unregister_event(i);
        }
    }

    lotus_memory_api->free(lotus_event_api);
}
