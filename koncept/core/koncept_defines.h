#pragma once

#include <corex/include/corex.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define KC_PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
    #define KC_PLATFORM_LINUX 1
#endif
