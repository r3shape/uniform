#pragma once

#include "../modules/core/include/lotus_core.h"

#if defined(LOTUS_2D)
#include "../modules/2d/include/lotus_2d.h"
#elif defined(LOTUS_3D)
#include "../modules/3d/include/lotus_3d.h"
#endif

#if defined(LOTUS_NEXT)
#include "../modules/lotus_next/include/lotus_plug.h"
#endif
