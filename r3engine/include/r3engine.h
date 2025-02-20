#pragma once

#include "../modules/core/include/r3_core.h"

#if defined(R3_2D)
#include "../modules/2D/include/r3_2D.h"
#elif defined(R3_3D)
#include "../modules/3D/include/r3_3D.h"
#elif defined(R3_NEXT)
#include "../modules/next/include/r3_plug.h"
#endif
