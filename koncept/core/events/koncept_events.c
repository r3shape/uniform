#include <koncept/core/events/koncept_events.h>

static struct internal_event_state {
    u16 event_count;
    u8 events[KC_EVENT_CODE_MAX];
    KC_Event_Callback* callbacks[KC_EVENT_CODE_MAX];
} internal_event_state = {0};

u8 _register_event_impl(u16 event_code) {
    if (event_code >= KC_EVENT_CODE_MAX || internal_event_state.events[event_code]) 
        return COREX_FALSE;

    internal_event_state.events[event_code] = COREX_TRUE;
    internal_event_state.callbacks[event_code] = corex->dsx.array.create_array(sizeof(KC_Event_Callback), KC_CALLBACK_MAX);

    internal_event_state.event_count++;
    return COREX_TRUE;
}

u8 _unregister_event_impl(u16 event_code) {
    if (event_code >= KC_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return COREX_FALSE;

    internal_event_state.events[event_code] = COREX_FALSE;
    
    if (internal_event_state.callbacks[event_code]) {
        corex->dsx.array.destroy_array(internal_event_state.callbacks[event_code]);
        internal_event_state.callbacks[event_code] = NULL;
    }

    internal_event_state.event_count--;
    return COREX_TRUE;
}

u8 _push_event_impl(u16 event_code, KC_Event data) {
    if (event_code >= KC_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return COREX_FALSE;

    if (!internal_event_state.callbacks[event_code]) return COREX_FALSE;

    u8 result = COREX_FALSE;
    Array_Head head = corex->dsx.array.get_array_head(internal_event_state.callbacks[event_code]);
    COREX_FORI(0, head.count, 1) {
        result = internal_event_state.callbacks[event_code][i](event_code, data);
    }

    return result;
}

u8 _register_callback_impl(u16 event_code, KC_Event_Callback callback) {
    if (event_code >= KC_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return COREX_FALSE;

    if (!internal_event_state.callbacks[event_code]) 
        return COREX_FALSE;

    corex->dsx.array.push_array(internal_event_state.callbacks[event_code], &callback);
    return COREX_TRUE;
}

u8 _unregister_callback_impl(u16 event_code, KC_Event_Callback callback) {
    if (event_code >= KC_EVENT_CODE_MAX || !internal_event_state.events[event_code]) 
        return COREX_FALSE;

    Array_Head head = corex->dsx.array.get_array_head(internal_event_state.callbacks[event_code]);
    if (head.count == 0) return COREX_FALSE;

    COREX_FORI(0, head.count, 1) {
        if (internal_event_state.callbacks[event_code][i] == callback) {
            KC_Event_Callback callback;
            corex->dsx.array.pull_array(internal_event_state.callbacks[event_code], i, &callback);
            return COREX_TRUE;
        }
    }
    return COREX_FALSE;
}


u8 _koncept_init_events(_koncept_events_api* api) {
    if (!api) return COREX_FALSE;

    api->push_event = _push_event_impl;
    api->register_event = _register_event_impl;
    api->unregister_event = _unregister_event_impl;
    api->register_callback = _register_callback_impl;
    api->unregister_callback = _unregister_callback_impl;

    for (u16 i = 0; i < KC_EVENT_CODES; i++) {
        if (!api->register_event(i)) {
            _koncept_cleanup_events(api);
            return COREX_FALSE;
        }
    }

    return COREX_TRUE;
}

u8 _koncept_cleanup_events(_koncept_events_api* api) {
    if (!api) return COREX_FALSE;
    
    for (u16 i = 0; i < KC_EVENT_CODE_MAX; ++i) {
        if (internal_event_state.events[i]) {
            api->unregister_event(i);
        }
    }

    api->push_event = NULL;
    api->register_event = NULL;
    api->unregister_event = NULL;
    api->register_callback = NULL;
    api->unregister_callback = NULL;

    return COREX_TRUE;
}
