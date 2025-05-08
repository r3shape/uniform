#ifndef __SANEDEFINES_H__
#define __SANEDEFINES_H__

#ifndef SSDK_NO_STDLIB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#endif

typedef char*               str;
typedef const char*         cstr;

typedef signed char         i8, sbyte;
typedef unsigned char       u8, ubyte, byte;

typedef float               f32;
typedef double              f64;

typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64, iptr;

typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64, uptr;

typedef void*               ptr;
typedef void                none;

#define SSDK_TRUE   1
#define SSDK_FALSE  0

#define SSDK_PI 3.14159265358979323846

#define SSDK_SWAP(t, a, b) { t temp = a; (a) = (b); (b) = temp; }

#define SSDK_STACK_ARR(t, a, ...) t a[__VA_ARGS__]

#define SSDK_IP2(x) ((x & (x - 1)) == 0)
#define SSDK_ALIGN(p, a) (((p) + ((a) - 1)) & ~((a) - 1))

#define SSDK_IGNORE(value)              ((void)v)
#define SSDK_DEFAULT(type)              (type){0}
#define SSDK_LITERAL(type, value)       (type){value}
#define SSDK_DEREF(type, value)         *(type*)value
#define SSDK_LITERAL_PTR(type, value)   &(type){value}

#define SSDK_WRAPI(value, limit) ((value) % (limit))
#define SSDK_WRAPF(value, limit) (fmod((f64)(value), (limit)))

#define SSDK_MAX(a, b) ((a) > (b) ? a : b)
#define SSDK_MIN(a, b) ((a) < (b) ? a : b)
#define SSDK_CLAMP(value, min, max) ((value) > (max) ? (max) : (value) < (min) ? (min) : (value))

#define SSDK_FORI(start, stop, step) for (u32 i = start; i < stop; i += step)
#define SSDK_FORJ(start, stop, step) for (u32 j = start; j < stop; j += step)
#define SSDK_FORK(start, stop, step) for (u32 k = start; k < stop; k += step)
#define SSDK_FOR(type, iter, start, stop, step) for (type iter = start; iter < stop; iter += step)

#define SSDK_KB  1024
#define SSDK_MB  (SSDK_KB * 1024)
#define SSDK_GB  (SSDK_MB * 1024)

#define SSDK_FLIP_BIT(v, b)  ((v) ^ (1<<(b)))
#define SSDK_SET_BITS(v, b)  ((v) | (1<<(b)))
#define SSDK_GET_BITS(v, b)  ((v) & (1<<(b)))
#define SSDK_REM_BITS(v, b)  ((v) & ~(1<<(b)))

#define SSDK_FNPTR(t, n, ...) t (*n)(__VA_ARGS__)

#ifdef SSDK_BUILD_SOURCE
    #ifdef _MSC_VER
        #define SSDK_FUNC __declspec(dllexport)
    #elif #defined (__GNUC__) || defined (__clang__)
        #define SSDK_FUNC __attribute__((visibility("default")))
    #else
        #define SSDK_FUNC
    #endif
#else
    #ifdef _MSC_VER
        #define SSDK_FUNC __declspec(dllimport)
    #elif defined(__GNUC__) || defined(__clang__)
        #define SSDK_FUNC __attribute__((visibility("default")))
    #else
        #define SSDK_FUNC
    #endif
#endif

#endif  // __SANEDEFINES_H__