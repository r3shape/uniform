#pragma once

#include "../../core/include/lotus_core.h"
#include "../../core/include/lotus_core_defines.h"

typedef struct Lotus2D_Primitive {
    Lotus_Vertex_Data vertexData;
    Lotus_Vec3 color;
    Lotus_Vec2 size;
} Lotus2D_Primitive;

LOTUS_API Lotus2D_Primitive lotus2d_create_circle(Lotus_Vec2 size, Lotus_Vec3 color);
LOTUS_API Lotus2D_Primitive lotus2d_create_triangle(Lotus_Vec2 size, Lotus_Vec3 color);
LOTUS_API Lotus2D_Primitive lotus2d_create_rectangle(Lotus_Vec2 size, Lotus_Vec3 color);
