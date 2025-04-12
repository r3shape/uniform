#pragma once

#include <koncept/core/koncept_defines.h>

#define KC_EVENT_MAX     ((1U << 12) - 1)
#define KC_CALLBACK_MAX  (1U << 5)
typedef enum KC_Event_Code {
    KC_EVENT_QUIT,
    KC_EVENT_KEY_PRESSED,
    KC_EVENT_KEY_RELEASED,
    KC_EVENT_BUTTON_PRESSED,
    KC_EVENT_BUTTON_RELEASED,
    KC_EVENT_MOUSE_MOVE,
    KC_EVENT_MOUSE_WHEEL,
    KC_EVENT_RESIZE,
    KC_EVENT_CODES,
    KC_EVENT_CODE_MAX = ((1U << 8) - 1)
} KC_Event_Code;

typedef union KC_Event {
    // 128 byte maximum
    i64 i64[2];
    u64 u64[2];
    f64 f64[2];

    i32 i32[4];
    u32 u32[4];
    f32 f32[4];

    i16 i16[8];
    u16 u16[8];
    
    i8 i8[16];
    u8 u8[16];
    char c[16];
} KC_Event;
typedef u8 (*KC_Event_Callback)(u16 event_code, KC_Event data);

typedef struct _koncept_events_api {
    u8 (*register_event)(u16 event_code);
    u8 (*unregister_event)(u16 event_code);
    u8 (*push_event)(u16 event_code, KC_Event data);
    u8 (*register_callback)(u16 event_code, KC_Event_Callback callback);
    u8 (*unregister_callback)(u16 event_code, KC_Event_Callback callback);
} _koncept_events_api;

u8 _koncept_init_events(_koncept_events_api* api);
u8 _koncept_cleanup_events(_koncept_events_api* api);