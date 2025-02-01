#pragma once

#include "../modules/core/include/lotus_core.h"

#if defined(LOTUS_2D)
#include "../modules/2D/include/lotus_2D.h"
#elif defined(LOTUS_3D)
#include "../modules/3D/include/lotus_3D.h"
#elif defined(LOTUS_NEXT)
#include "../modules/lotus_next/include/lotus_plug.h"
#endif
