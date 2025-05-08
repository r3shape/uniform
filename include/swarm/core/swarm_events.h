#ifndef __SWARM_EVENT_H__
#define __SWARM_EVENT_H__

#include <include/swarm/core/globals.h>

#define SWARM_CALLBACK_MAX  (1U << 5)
#define SWARM_EVENT_MAX     ((1U << 12) - 1)
typedef enum SwarmEventCode {
    SWARM_EVENT_QUIT,
    SWARM_EVENT_KEY_PRESSED,
    SWARM_EVENT_KEY_RELEASED,
    SWARM_EVENT_BUTTON_PRESSED,
    SWARM_EVENT_BUTTON_RELEASED,
    SWARM_EVENT_MOUSE_MOVE,
    SWARM_EVENT_MOUSE_WHEEL,
    SWARM_EVENT_RESIZE,
    SWARM_EVENT_CODES,
    SWARM_EVENT_CODE_MAX = ((1U << 8) - 1)
} SwarmEventCode;

typedef union SwarmEvent {
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
} SwarmEvent;
typedef u8 (*SwarmCallback)(u16 eventCode, SwarmEvent data);

typedef struct SwarmEvents {
    none (*init)(none);
    none (*shutdown)(none);

    u8 (*registerEvent)(u16 eventCode);
    u8 (*unregisterEvent)(u16 eventCode);
    u8 (*pushEvent)(u16 eventCode, SwarmEvent data);
    
    u8 (*registerCallback)(u16 eventCode, SwarmCallback callback);
    u8 (*unregisterCallback)(u16 eventCode, SwarmCallback callback);
} SwarmEvents;
extern SwarmEvents* swarmEvents;

#endif  // __SWARM_EVENT_H__