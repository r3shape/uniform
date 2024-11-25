#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef signed char       i8;
typedef signed short      i16;
typedef signed int        i32;
typedef signed long long  i64;

typedef float   f32;
typedef double  f64;

typedef char    b8;
typedef int     b32;

typedef void LTvoid;
typedef float LTfloat;
typedef bool LTboolean;
typedef void *LTvoidptr;
typedef float *LTfloatptr;
typedef const char *LTstring;
typedef unsigned char LTubyte;
typedef int LTsizeiptr, *LTintptr;
typedef unsigned int LTuint, LTenum;
typedef int LTsizei, LTint, LTbitfield;

#if defined(__clang__) || defined(__gcc__)
    #define ltStaticAssert static_assert
#else
    #define ltStaticAssert _Static_assert
#endif

typedef enum LTvalueType {
    LOTUS_NONE=0,
    LOTUS_INT,
    LOTUS_CHAR,
    LOTUS_FLOAT,
    LOTUS_STRING,
    LOTUS_VALUES
} LTvalueType;

typedef enum LTerrorType {
    LOTUS_ERR_NONE=0,
    LOTUS_ERR_INIT,
    LOTUS_ERR_TYPE,
    LOTUS_ERR_MALLOC,
    LOTUS_ERR_PROCESS,
    LOTUS_ERR_TYPES,
} LTerrorType;

ltStaticAssert(sizeof(u8) == 1, "expected u8 to be 1 byte");
ltStaticAssert(sizeof(u16) == 2, "expected u16 to be 2 bytes");
ltStaticAssert(sizeof(u32) == 4, "expected u32 to be 4 bytes");
ltStaticAssert(sizeof(u64) == 8, "expected u64 to be 8 bytes");

ltStaticAssert(sizeof(i8) == 1, "expected i8 to be 1 byte");
ltStaticAssert(sizeof(i16) == 2, "expected i16 to be 2 bytes");
ltStaticAssert(sizeof(i32) == 4, "expected i32 to be 4 bytes");
ltStaticAssert(sizeof(i64) == 8, "expected i64 to be 8 bytes");

ltStaticAssert(sizeof(f32) == 4, "expected f32 to be 4 bytes");
ltStaticAssert(sizeof(f64) == 8, "expected f64 to be 8 bytes");

ltStaticAssert(sizeof(b8) == 1, "expected b8 to be 1 byte");
ltStaticAssert(sizeof(b32) == 4, "expected b32 to be 4 bytes");

#define LOTUS_TRUE     1
#define LOTUS_FALSE    0

#if defined(WIN32) || defined(_WIN32) | defined(__WIN32__)
    #define LOTUS_PLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "64-bits required on Windows"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    #define LOTUS_PLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define LOTUS_PLATFORM_ANDROID 1
    #endif
#endif

#ifdef LOTUS_EXPORT
    #ifdef _MSC_VER
        #define LOTUS_API __declspec(dllexport)
    #else 
        #define LOTUS_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define LOTUS_API __declspec(dllimport)
    #else 
        #define LOTUS_API
    #endif
#endif


#endif