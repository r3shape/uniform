#pragma once

#include <r3/r3.core/include/r3.input.h>
#include <r3/r3.core/include/r3.events.h>
#include <r3/r3.core/include/r3.platform.h>
#include <r3/r3.core/include/r3.graphics.h>

typedef struct _r3_core_api {
    _r3_input_api input;
    _r3_events_api events;
    _r3_platform_api platform;
    _r3_graphics_api graphics;
} _r3_core_api;
extern _r3_core_api* r3_core;

u8 r3_init_core(void);
u8 r3_cleanup_core(void);
