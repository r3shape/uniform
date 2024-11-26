#ifndef LTEVENT_H
#define LTEVENT_H

#include "../../defines.h"

// Lotus internal event codes. Application should use codes > 255.
typedef enum tagEventCodes {
    // shuts down the application on the next frame.
    LOTUS_EVENT_APP_QUIT = 0x01,

    // keyboard key pressed.
    /* Event Data Usage:
        * u16 keyCode = data.data.u16[0];
    */
    LOTUS_EVENT_KEY_PRESSED = 0x02,
    

    // keyboard key released.
    /* Event Data Usage:
        * u16 keyCode = data.data.u16[0];
    */
    LOTUS_EVENT_KEY_RELEASED = 0x03,

    // mouse button pressed.
    /* Event Data Usage:
        * u16 button = data.data.u16[0];
    */
    LOTUS_EVENT_BUTTON_PRESSED = 0x04,

    // mouse button released.
    /* Event Data Usage:
        * u16 button = data.data.u16[0];
    */
    LOTUS_EVENT_BUTTON_RELEASED = 0x05,

    // mouse movement.
    /* Event Data Usage:
        * u16 x = data.data.u16[0];
        * u16 y = data.data.u16[1];
    */
    LOTUS_EVENT_MOUSE_MOVE = 0x06,

    // mouse movement.
    /* Event Data Usage:
        * u8 zDelta = data.data.u8[0];
    */
    LOTUS_EVENT_MOUSE_WHEEL = 0x07,

    /* OS resize/resolution change. (deprecated)
    * Event Data Usage:
        * u16 width = data.data.u16[0];
        * u16 height = data.data.u16[1];
    */
    LOTUS_EVENT_RESIZE = 0x08,

    LOTUS_EVENT_MAX = 0xFF
} LTeventCode;

typedef struct tagEventData {
    // 128 byte maximum
    union data {
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
    } data;

} LTeventData;

b8 ltEventInit(void);
void ltEventExit(void);

typedef b8 (*onEvent_FPTR)(LTeventData data, u16 eventCode, void* sender, void* listener);

/**
 * Push an event to listeners of the specified event code. If an event handler returns
 * `LOTUS_TRUE`, the event is considered handled and is passed no further.
 * @param data The event data.
 * @param eventCode The event code to push.
 * @param sender A pointer to the sender can be 0/NULL.
 * @returns `LOTUS_TRUE` if handles; otherwise `LOTUS_FALSE`
 */
LOTUS_API_ENTRY b8 ltPushEvent(LTeventData data, u16 eventCode, void* sender);

/**
 * Register to listen for when events are sent with the specified event code. Events with duplicate
 * listener/callback combinations will not be re-registered and will cause this function to return `LOTUS_FALSE`.
 * @param eventCode The event code to listen for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param onEvent The callback function pointer to be invoked when the event code is pushed.
 * @returns `LOTUS_TRUE` if the event is successfully registered; otherwise `LOTUS_FALSE`.
 */
LOTUS_API_ENTRY b8 ltRegisterEvent(u16 eventCode, void* listener, onEvent_FPTR onEvent);

/**
 * Unregister from listening for when events are sent with the specified event code. If no matching
 * registration is found, this function returns `LOTUS_FALSE`.
 * @param eventCode The event code to stop listening for.
 * @param listener A pointer to a listener instance. Can be 0/NULL.
 * @param onEvent The callback function pointer to be unregistered.
 * @returns `LOTUS_TRUE` if the event is successfully unregistered; otherwise `LOTUS_FALSE`.
 */
LOTUS_API_ENTRY b8 ltUnregisterEvent(u16 eventCode, void* listener, onEvent_FPTR onEvent);

#endif