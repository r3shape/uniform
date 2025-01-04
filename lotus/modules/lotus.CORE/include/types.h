#pragma once

#include "defines.h"

typedef enum Lotus_Error_Type {
    LOTUS_ERR_NONE=0,
    LOTUS_ERR_INIT,
    LOTUS_ERR_TYPE,
    LOTUS_ERR_FUNC,
    LOTUS_ERR_MALLOC,
    LOTUS_ERR_TYPES
} Lotus_Error_Type;

