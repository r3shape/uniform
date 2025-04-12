#pragma once

#include <r3/core/r3.defines.h>

#define R3_EVENT_MAX     ((1U << 12) - 1)
#define R3_CALLBACK_MAX  (1U << 5)
typedef enum R3_Event_Code {
    R3_EVENT_QUIT,
    R3_EVENT_KEY_PRESSED,
    R3_EVENT_KEY_RELEASED,
    R3_EVENT_BUTTON_PRESSED,
    R3_EVENT_BUTTON_RELEASED,
    R3_EVENT_MOUSE_MOVE,
    R3_EVENT_MOUSE_WHEEL,
    R3_EVENT_RESIZE,
    R3_EVENT_CODES,
    R3_EVENT_CODE_MAX = ((1U << 8) - 1)
} R3_Event_Code;

typedef union R3_Event {
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
} R3_Event;
typedef u8 (*R3_Event_Callback)(u16 event_code, R3_Event data);

typedef struct _r3_events_api {
    u8 (*register_event)(u16 event_code);
    u8 (*unregister_event)(u16 event_code);
    u8 (*push_event)(u16 event_code, R3_Event data);
    u8 (*register_callback)(u16 event_code, R3_Event_Callback callback);
    u8 (*unregister_callback)(u16 event_code, R3_Event_Callback callback);
} _r3_events_api;

u8 _r3_init_events(_r3_events_api* api);
u8 _r3_cleanup_events(_r3_events_api* api);