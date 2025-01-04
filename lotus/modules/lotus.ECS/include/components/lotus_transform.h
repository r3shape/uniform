#pragma once

#include "../lotus_ecs_types.h"

#include "../../../lotus.CORE/include/utility/lotus_math.h"

typedef struct Lotus_Transform {
    Lotus_Mat4* model;
    Lotus_Vec3* scale;
    Lotus_Vec3* rotation;
    Lotus_Vec3* location;
} Lotus_Transform;

typedef struct Lotus_Transform_Data {
    Lotus_Mat4* model;
    Lotus_Vec3* scale;
    Lotus_Vec3* rotation;
    Lotus_Vec3* location;
} Lotus_Transform_Data;

ubyte _lotus_init_transform_data(Lotus_Transform_Data* data);
void _lotus_destroy_transform_data(Lotus_Transform_Data* data);

void _lotus_add_transform(void* data, Lotus_Entity entity);
void _lotus_rem_transform(void* data, Lotus_Entity entity);
void _lotus_set_transform(void* data, Lotus_Component component, Lotus_Entity entity);
Lotus_Component _lotus_get_transform(void* data, Lotus_Entity entity);
