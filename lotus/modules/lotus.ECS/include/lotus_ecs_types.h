#pragma once

#include "../../lotus.CORE/include/defines.h"

typedef unsigned int Lotus_Entity;
#define LOTUS_ENTITY_MAX ((1U << 16) - 1)

typedef struct Lotus_Component Lotus_Component;
#define LOTUS_DEFINE_COMPONENT_TYPE(t) t##_COMPONENT
typedef enum Lotus_Component_Type {
    LOTUS_DEFINE_COMPONENT_TYPE(LOTUS_MESH),
    LOTUS_DEFINE_COMPONENT_TYPE(LOTUS_TEXTURE),
    LOTUS_DEFINE_COMPONENT_TYPE(LOTUS_TRANSFORM),
    /* user defined components should have IDs < 255 */
    LOTUS_COMPONENT_TYPES,
    LOTUS_COMPONENT_MAX = ((1U << 8) - 1)
} Lotus_Component_Type;

