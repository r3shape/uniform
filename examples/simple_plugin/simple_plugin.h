#pragma once

#define R3_NEXT
#include "../../r3engine/include/r3engine.h"

R3_PLUG_API(Simple_Plugin,
    R3_PLUG_API_FUNC(void, hello_plugin, void);
    R3_PLUG_API_FUNC(int, add_numbers, int, int);
    R3_PLUG_API_FUNC(void, goodbye_plugin, void);
);
