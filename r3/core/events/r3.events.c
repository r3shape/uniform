#include <r3/core/events/r3.events.h>

static struct internal_event_state {
    u16 event_count;
    u8 events[R3_EVENT_CODE_MAX];
    R3_Event_Callback* callbacks[R3_EVENT_CODE_MAX];
} internal_event_state = {0};

u8 _register_event_impl(u16 event_code) {
    if (event_code >= R3_EVENT_CODE_MAX || internal_event_state.events[event_code]) 
        return LIBX_FALSE;

    internal_event_state.events[event_code] = LIBX_TRUE;
    internal_event_state.callbacks[event_code] = libx->dsx.array.create_array(sizeof(R3_Event_Callback), R3_CALLBACK_MAX);

    internal_event_state.event_count++;
    return LIBX_TRUE;
}

u8 _unregister_event_impl(u16 event_code) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LIBX_FALSE;

    internal_event_state.events[event_code] = LIBX_FALSE;
    
    if (internal_event_state.callbacks[event_code]) {
        libx->dsx.array.destroy_array(internal_event_state.callbacks[event_code]);
        internal_event_state.callbacks[event_code] = NULL;
    }

    internal_event_state.event_count--;
    return LIBX_TRUE;
}

u8 _push_event_impl(u16 event_code, R3_Event data) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LIBX_FALSE;

    if (!internal_event_state.callbacks[event_code]) return LIBX_FALSE;

    u8 result = LIBX_FALSE;
    Array_Head head = libx->dsx.array.get_array_head(internal_event_state.callbacks[event_code]);
    LIBX_FORI(0, head.count, 1) {
        result = internal_event_state.callbacks[event_code][i](event_code, data);
    }

    return result;
}

u8 _register_callback_impl(u16 event_code, R3_Event_Callback callback) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LIBX_FALSE;

    if (!internal_event_state.callbacks[event_code]) 
        return LIBX_FALSE;

    libx->dsx.array.push_array(internal_event_state.callbacks[event_code], &callback);
    return LIBX_TRUE;
}

u8 _unregister_callback_impl(u16 event_code, R3_Event_Callback callback) {
    if (event_code >= R3_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return LIBX_FALSE;

    Array_Head head = libx->dsx.array.get_array_head(internal_event_state.callbacks[event_code]);
    if (head.count == 0) return LIBX_FALSE;

    LIBX_FORI(0, head.count, 1) {
        if (internal_event_state.callbacks[event_code][i] == callback) {
            R3_Event_Callback callback;
            libx->dsx.array.pull_array(internal_event_state.callbacks[event_code], i, &callback);
            return LIBX_TRUE;
        }
    }
    return LIBX_FALSE;
}


u8 _r3_init_events(_r3_events_api* api) {
    if (!api) return LIBX_FALSE;

    api->push_event = _push_event_impl;
    api->register_event = _register_event_impl;
    api->unregister_event = _unregister_event_impl;
    api->register_callback = _register_callback_impl;
    api->unregister_callback = _unregister_callback_impl;

    for (u16 i = 0; i < R3_EVENT_CODES; i++) {
        if (!api->register_event(i)) {
            _r3_cleanup_events(api);
            return LIBX_FALSE;
        }
    }

    return LIBX_TRUE;
}

u8 _r3_cleanup_events(_r3_events_api* api) {
    if (!api) return LIBX_FALSE;
    
    for (u16 i = 0; i < R3_EVENT_CODE_MAX; ++i) {
        if (internal_event_state.events[i]) {
            api->unregister_event(i);
        }
    }

    api->push_event = NULL;
    api->register_event = NULL;
    api->unregister_event = NULL;
    api->register_callback = NULL;
    api->unregister_callback = NULL;

    return LIBX_TRUE;
}
