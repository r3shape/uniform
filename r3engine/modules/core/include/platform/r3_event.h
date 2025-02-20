#pragma once

#include "../r3_core_defines.h"
#include "../memory/r3_memory.h"

#define R3_EVENT_MAX     ((1U << 12) - 1)
#define R3_CALLBACK_MAX  (1U << 5)
typedef enum R3_Event_Code {
    R3_EVENT_APP_QUIT = 0,
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
    sbyte8 sbyte8[2];
    ubyte8 ubyte8[2];
    f64 f64[2];

    sbyte4 sbyte4[4];
    ubyte4 ubyte4[4];
    f32 f32[4];

    sbyte2 sbyte2[8];
    ubyte2 ubyte2[8];
    
    sbyte sbyte[16];
    ubyte ubyte[16];
    char c[16];
} R3_Event;
typedef ubyte (*R3_Event_Callback)(R3_Event data, ubyte2 event_code);

typedef struct R3_Event_API {
    ubyte (*register_event)(ubyte2 event_code);
    ubyte (*unregister_event)(ubyte2 event_code);
    ubyte (*push_event)(R3_Event data, ubyte2 event_code);
    ubyte (*register_callback)(ubyte2 event_code, R3_Event_Callback callback);
    ubyte (*unregister_callback)(ubyte2 event_code, R3_Event_Callback callback);
} R3_Event_API;

extern R3_Event_API* r3_event_api;

ubyte r3_init_events(void);
void r3_shutdown_events(void);
