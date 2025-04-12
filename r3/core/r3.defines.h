#pragma once

#include <libx/include/libx.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define R3_PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
    #define R3_PLATFORM_LINUX 1
#endif
