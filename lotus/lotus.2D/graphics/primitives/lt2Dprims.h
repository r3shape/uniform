#ifndef LT2DPRIMS_H
#define LT2DPRIMS_H

#include "../../lt2Dtypes.h"

LOTUS_API_ENTRY void lt2dDrawPrimitive(LT2Dprimitive* prim);
LOTUS_API_ENTRY LT2Dprimitive lt2dMakeTriangle(f32 width, f32 height, f32 r, f32 g, f32 b);
LOTUS_API_ENTRY LT2Dprimitive lt2dMakeRectangle(f32 width, f32 height, f32 r, f32 g, f32 b);
LOTUS_API_ENTRY LT2Dprimitive lt2dMakeCircle(f32 radius, u32 segments, f32 r, f32 g, f32 b);

#endif