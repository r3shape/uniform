#pragma once

#define LOTUS_PLUG
#include "../../lotus/include/lotus.h"

LOTUS_DEFINE_PLUG_API(Test_Plug,
    LOTUS_DEFINE_PLUG_API_FN(void, hello_plug, void);
    LOTUS_DEFINE_PLUG_API_FN(void, goodbye_plug, void);
    LOTUS_DEFINE_PLUG_API_FN(int, add_numbers, int, int);
);