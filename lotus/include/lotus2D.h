#ifndef LOTUS2D_H
#define LOTUS2D_H

#include "lotusECS.h"
#include "../lotus.2D/lt2Dtypes.h"
#include "../lotus.2D/graphics/primitives/lt2Dprims.h"

static LTscene* scene2D;

LOTUS_API_ENTRY b8 lt2DInit(void);
LOTUS_API_ENTRY void lt2DExit(void);

LOTUS_API_ENTRY LTscene* ltGetScene2D(void);

#endif