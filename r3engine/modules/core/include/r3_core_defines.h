#pragma once

#include <stddef.h>

typedef unsigned char       ubyte;
typedef unsigned short      ubyte2;
typedef unsigned int        ubyte4;
typedef unsigned long long  ubyte8;

typedef signed char       sbyte;
typedef signed short      sbyte2;
typedef signed int        sbyte4;
typedef signed long long  sbyte8;

typedef float   f32;
typedef double  f64;
typedef long double f80, f96, f128;

#define R3_TRUE  1
#define R3_FALSE 0

#define R3_C_LITERAL(t, v)        (t)(v)
#define R3_C_LITERAL_PTR(t, v)   &(t)(v)

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define R3_PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
    #define R3_PLATFORM_LINUX 1
#endif

#ifdef R3_DLL_EXPORT
    #ifdef _MSC_VER
        #define R3_API __declspec(dllexport)
    #elif #defined (__GNUC__) || defined (__clang__)
        #define R3_API __attribute__((visibility("default")))
    #else
        #define R3_API
    #endif
#else
    #ifdef _MSC_VER
        #define R3_API __declspec(dllimport)
    #elif defined(__GNUC__) || defined(__clang__)
        #define R3_API __attribute__((visibility("default")))
    #else
        #define R3_API
    #endif
#endif

#ifndef _R3_PRIVATE
    #if defined(__GNUC__) || defined(__clang__)
        #define _R3_PRIVATE __attribute__((unused)) static
    #else
        #define _R3_PRIVATE static
    #endif
#endif
