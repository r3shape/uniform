#pragma once

#include <r3/core/input/r3.input.h>
#include <r3/core/events/r3.events.h>
#include <r3/core/platform/r3.platform.h>
#include <r3/core/graphics/r3.graphics.h>

#define R3_CORE (1U << 0)
#define R3_PACK (1U << 1)
#define R3_2D (1U << 2)
#define R3_3D (1U << 3)

typedef struct _r3_core_api {
    u8 modules;
    _r3_input_api input;
    _r3_events_api events;
    _r3_platform_api platform;
    _r3_graphics_api graphics;
} _r3_core_api;
extern _r3_core_api* r3_core;

u8 r3_init_core(u8 modules);
u8 r3_cleanup_core(void);

#ifdef R3_MODULES
    #if ((R3_MODULES & R3_PACK))
        #include <r3/modules/pack/include/r3.pack.h>
    #endif
    #if ((R3_MODULES & R3_2D))
        #include <r3/modules/2D/include/r3.2D.h>
    #endif
    #if ((R3_MODULES & R3_3D))
        #include <r3/modules/3D/include/r3.3D.h>
    #endif
#else
    #define R3_MODULES R3_CORE
#endif
