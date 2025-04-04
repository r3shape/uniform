#pragma once

#include "../../../external/libx/include/libx_def.h"
#include "../../../external/libx/include/libx_fio.h"
#include "../../../external/libx/include/libx_ecs.h"
#include "../../../external/libx/include/libx_math.h"
#include "../../../external/libx/include/libx_memory.h"
#include "../../../external/libx/include/libx_structs.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define R3_PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
    #define R3_PLATFORM_LINUX 1
#endif
