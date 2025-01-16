#pragma once

#include "../lotus_core_defines.h"
#include "../memory/lotus_memory.h"

#define LOTUS_EVENT_CODE_MAX 16384
typedef enum Lotus_Event_Code {
    LOTUS_EVENT_APP_QUIT = 0x01,
    LOTUS_EVENT_KEY_PRESSED = 0x02,
    LOTUS_EVENT_KEY_RELEASED = 0x03,
    LOTUS_EVENT_BUTTON_PRESSED = 0x04,
    LOTUS_EVENT_BUTTON_RELEASED = 0x05,
    LOTUS_EVENT_MOUSE_MOVE = 0x06,
    LOTUS_EVENT_MOUSE_WHEEL = 0x07,
    LOTUS_EVENT_RESIZE = 0x08,
    LOTUS_EVENT_MAX = 0xFF
} Lotus_Event_Code;

typedef struct Lotus_Event Lotus_Event;
typedef ubyte (*Lotus_Event_Callback)(Lotus_Event data, ubyte2 event_code);;

struct Lotus_Event {
    // 128 byte maximum
    union event_data {
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
    } event_data;
    Lotus_Event_Callback callback;
};

typedef struct Lotus_Event_Register {
    Lotus_Event* events;
} Lotus_Event_Register;

typedef struct Lotus_Event_State {
    ubyte init;
    // event code lookup-table.
    Lotus_Event_Register registered[LOTUS_EVENT_CODE_MAX];
} Lotus_Event_State;

Lotus_Event_State* lotus_init_event();
void lotus_shutdown_event();

/**
 * Push an event to listeners of the specified event code. If an event handler returns
 * `LOTUS_TRUE`, the event is considered handled and is passed no further.
 * @param data The event data.
 * @param event_code The event code to push.
 * @param sender A pointer to the sender can be 0/NULL.
 * @returns `LOTUS_TRUE` if handles; otherwise `LOTUS_FALSE`
 */
LOTUS_API ubyte lotus_push_event(Lotus_Event data, ubyte2 event_code);

/**
 * Register to listen for when events are sent with the specified event code.
 * @param event_code The event code to listen for.
 * @param callback The callback function pointer to be invoked when the event code is pushed.
 * @returns `LOTUS_TRUE` if the event is successfully registered; otherwise `LOTUS_FALSE`.
 */
LOTUS_API ubyte lotus_register_event_callback(ubyte2 event_code, Lotus_Event_Callback callback);

/**
 * Unregister from listening for when events are sent with the specified event code. If no matching
 * registration is found, this function returns `LOTUS_FALSE`.
 * @param event_code The event code to stop listening for.
 * @param callback The callback function pointer to be unregistered.
 * @returns `LOTUS_TRUE` if the event is successfully unregistered; otherwise `LOTUS_FALSE`.
 */
LOTUS_API ubyte lotus_unregister_event_callback(ubyte2 event_code, Lotus_Event_Callback callback);
