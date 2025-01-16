#pragma once

#include "../../core/include/lotus_core.h"
#include "../../core/include/lotus_core_defines.h"

typedef struct Lotus3D_Primitive {
    Lotus_Vertex_Data vertexData;
    Lotus_Vec3 color;
    Lotus_Vec3 size;
} Lotus3D_Primitive;

LOTUS_API Lotus3D_Primitive lotus3d_create_cube(Lotus_Vec3 size, Lotus_Vec3 color);
