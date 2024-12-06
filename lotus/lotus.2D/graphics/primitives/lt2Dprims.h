#ifndef LT2DPRIMS_H
#define LT2DPRIMS_H

#include "../../lt2Dtypes.h"
#include "../../../lotus.ECS/lotusECStypes.h"

LOTUS_API_ENTRY void ltDrawMesh2D(LTentity entity);
LOTUS_API_ENTRY b8 ltTriangleMesh2D(LTentity entity, f32 width, f32 height, f32 r, f32 g, f32 b);
LOTUS_API_ENTRY b8 ltRectangleMesh2D(LTentity entity, f32 width, f32 height, f32 r, f32 g, f32 b);
LOTUS_API_ENTRY b8 ltCircleMesh2D(LTentity entity, f32 radius, u32 segments, f32 r, f32 g, f32 b);

#endif