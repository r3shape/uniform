#pragma once

#include "../lotus_core_defines.h"
#include "../memory/lotus_memory.h"

#define LOTUS_EVENT_MAX     ((1U << 12) - 1)
#define LOTUS_CALLBACK_MAX  (1U << 5)
typedef enum Lotus_Event_Code {
    LOTUS_EVENT_APP_QUIT = 0,
    LOTUS_EVENT_KEY_PRESSED,
    LOTUS_EVENT_KEY_RELEASED,
    LOTUS_EVENT_BUTTON_PRESSED,
    LOTUS_EVENT_BUTTON_RELEASED,
    LOTUS_EVENT_MOUSE_MOVE,
    LOTUS_EVENT_MOUSE_WHEEL,
    LOTUS_EVENT_RESIZE,
    LOTUS_EVENT_CODES,
    LOTUS_EVENT_CODE_MAX = ((1U << 8) - 1)
} Lotus_Event_Code;

typedef union Lotus_Event {
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
} Lotus_Event;
typedef ubyte (*Lotus_Event_Callback)(Lotus_Event data, ubyte2 event_code);

typedef struct Lotus_Event_API {
    ubyte (*register_event)(ubyte2 event_code);
    ubyte (*unregister_event)(ubyte2 event_code);
    ubyte (*push_event)(Lotus_Event data, ubyte2 event_code);
    ubyte (*register_callback)(ubyte2 event_code, Lotus_Event_Callback callback);
    ubyte (*unregister_callback)(ubyte2 event_code, Lotus_Event_Callback callback);
} Lotus_Event_API;

extern Lotus_Event_API* lotus_event_api;

ubyte lotus_init_events(void);
void lotus_shutdown_events(void);
