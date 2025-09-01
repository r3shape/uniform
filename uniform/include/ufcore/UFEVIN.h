#ifndef _UFEVIN_H_
#define _UFEVIN_H_

#define _UF_R3DEF_
#include <ufcore/UFDEF.h>

typedef enum UFDeviceType {
    UF_DEVICE_MOUSE,
    UF_DEVICE_KEYBOARD,
    UF_DEVICE_CONTROLLER
} UFDeviceType;

typedef struct UFDevice {
    UFDeviceType type;
    ptr buffers[2];
} UFDevice;

typedef struct UFEvent {
    // 128 byte max
    s64 s64[2];
    u64 u64[2];
    f64 f64[2];

    s32 s32[4];
    u32 u32[4];
    f32 f32[4];

    s16 s16[8];
    u16 u16[8];
    
    s8 s8[16];
    u8 u8[16];
    char c[16];
} UFEvent;

typedef none (*UFEventCallback)(UFEventCode, UFEvent);

typedef struct UFEVINInterface {
    UFVersion ver;

    UF_API_METHOD(none, newEvent, UFEventCode mask);
    UF_API_METHOD(none, delEvent, UFEventCode mask);
    UF_API_METHOD(none, sendEvent, UFEventCode mask, UFEvent event);

    UF_API_METHOD(none, newCallback, none);
    UF_API_METHOD(none, delCallback, none);

    UF_API_METHOD(none, resetDevices, none);
    UF_API_METHOD(none, updateDevices, none);

    UF_API_METHOD(UFResource, newDevice, UFDeviceType type);
    UF_API_METHOD(none, delDevice, UFResource device);
    UF_API_METHOD(none, setDeviceState, none);
    UF_API_METHOD(none, getDeviceState, none);
    UF_API_METHOD(none, resetDeviceState, none);
} UFEVINInterface;

#endif // _UFEVIN_H_
