#pragma once

#define LOTUS_NEXT
#include "../../lotus/include/lotus.h"

LOTUS_PLUG_API(Simple_Plugin,
    LOTUS_PLUG_API_FUNC(void, hello_plugin, void);
    LOTUS_PLUG_API_FUNC(int, add_numbers, int, int);
    LOTUS_PLUG_API_FUNC(void, goodbye_plugin, void);
);
