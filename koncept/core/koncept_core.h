#pragma once

#include <koncept/core/input/koncept_input.h>
#include <koncept/core/events/koncept_events.h>
#include <koncept/core/platform/koncept_platform.h>
#include <koncept/core/graphics/koncept_graphics.h>

#define KC_CORE (1U << 0)
#define KC_PACK (1U << 1)
#define KC_2D (1U << 2)
#define KC_3D (1U << 3)

typedef struct _koncept_core_api {
    u8 modules;
    _koncept_input_api input;
    _koncept_events_api events;
    _koncept_platform_api platform;
    _koncept_graphics_api graphics;
} _koncept_core_api;
extern _koncept_core_api* koncept_core;

u8 koncept_init_core(u8 modules);
u8 koncept_cleanup_core(void);

#ifdef KC_MODULES
    #if ((KC_MODULES & KC_PACK))
        #include <koncept/modules/pack/include/koncept_pack.h>
    #endif
    #if ((KC_MODULES & KC_2D))
        #include <koncept/modules/2D/include/koncept_2D.h>
    #endif
    #if ((KC_MODULES & KC_3D))
        #include <koncept/modules/3D/include/koncept_3D.h>
    #endif
#else
    #define KC_MODULES KC_CORE
#endif
