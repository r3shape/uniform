#include "../../include/platform/r3_event.h"
#include "../../include/utility/r3_array.h"

R3_Event_API* r3_event_api = NULL;
static struct internal_event_state {
    ubyte2 event_count;
    ubyte events[R3_EVENT_CODE_MAX];
    R3_Event_Callback* callbacks[R3_EVENT_CODE_MAX];
} internal_event_state = {0};


ubyte _register_event_impl(ubyte2 event_code) {
    if (event_code >= R3_EVENT_CODE_MAX || internal_event_state.events[event_code]) 
        return R3_FALSE;

    internal_event_state.events[event_code] = R3_TRUE;
    internal_event_state.callbacks[event_code] = r3_create_array(sizeof(R3_Event_Callback), 2);

    internal_event_state.event_count++;
    return R3_TRUE;
}

ubyte _unregister_event_impl(ubyte2 event_code) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return R3_FALSE;

    internal_event_state.events[event_code] = R3_FALSE;
    
    if (internal_event_state.callbacks[event_code]) {
        r3_destroy_array(internal_event_state.callbacks[event_code]);
        internal_event_state.callbacks[event_code] = NULL;
    }

    internal_event_state.event_count--;
    return R3_TRUE;
}

ubyte _push_event_impl(R3_Event data, ubyte2 event_code) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return R3_FALSE;

    if (!internal_event_state.callbacks[event_code]) return R3_FALSE;

    ubyte8 count = R3_ARRAY_GET_HEADER_FIELD(internal_event_state.callbacks[event_code], R3_ARRAY_LENGTH_FIELD);
    for (ubyte8 i = 0; i < count; ++i) {
        if (internal_event_state.callbacks[event_code][i](data, event_code)) {
            return R3_TRUE;
        }
    }

    return R3_FALSE;
}

ubyte _register_callback_impl(ubyte2 event_code, R3_Event_Callback callback) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return R3_FALSE;

    if (!internal_event_state.callbacks[event_code]) 
        return R3_FALSE;

    r3_push_array(internal_event_state.callbacks[event_code], &callback);
    return R3_TRUE;
}

ubyte _unregister_callback_impl(ubyte2 event_code, R3_Event_Callback callback) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return R3_FALSE;

    ubyte8 count = R3_ARRAY_GET_HEADER_FIELD(internal_event_state.callbacks[event_code], R3_ARRAY_LENGTH_FIELD);
    if (count == 0) return R3_FALSE;

    R3_Event_Callback* callbacks = internal_event_state.callbacks[event_code];

    for (ubyte8 i = 0; i < count; ++i) {
        if (callbacks[i] == callback) {
            for (ubyte8 j = i; j < count - 1; ++j) {
                callbacks[j] = callbacks[j + 1];
            }

            R3_Event_Callback callback;
            r3_pop_array(internal_event_state.callbacks[event_code], callback);

            return R3_TRUE;
        }
    }
    return R3_FALSE;
}


ubyte r3_init_events(void) {
    r3_event_api = (R3_Event_API*)r3_memory_api->alloc(sizeof(R3_Event_API), 16);
    if (!r3_event_api) return R3_FALSE;

    r3_event_api->push_event = _push_event_impl;
    r3_event_api->register_event = _register_event_impl;
    r3_event_api->unregister_event = _unregister_event_impl;
    r3_event_api->register_callback = _register_callback_impl;
    r3_event_api->unregister_callback = _unregister_callback_impl;

    for (ubyte2 i = 0; i < R3_EVENT_CODES; i++) {
        if (!r3_event_api->register_event(i)) {
            r3_shutdown_events();
            return R3_FALSE;
        }
    }

    return R3_TRUE;
}

void r3_shutdown_events(void) {
    for (ubyte2 i = 0; i < R3_EVENT_CODE_MAX; ++i) {
        if (internal_event_state.events[i]) {
            r3_event_api->unregister_event(i);
        }
    }

    r3_memory_api->free(r3_event_api);
}
