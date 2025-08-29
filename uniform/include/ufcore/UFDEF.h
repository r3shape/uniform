#ifndef _UF_DEFINES_H_
#define _UF_DEFINES_H_

#ifdef _UF_R3DEF_
#include <r3kit/include/defines.h>
#endif // _UF_R3DEF_

typedef unsigned long long UFResource;
typedef unsigned int UFVersion, UFGpuApi, UFGpuCtx, UFEventMask;

#define UF_API_METHOD(r, n, ...) r (*n)(__VA_ARGS__)

#if defined(WIN32) || defined(_WIN32) || (__WIN32__)
#define _UF_PLATFORM_WINDOWS_
#endif // WIN32 || _WIN32 || __WIN32__

#ifdef _UF_PLATFORM_WINDOWS_
#ifdef _UF_BUILD_DLL_
#ifdef _UF_INTERNAL_
#define UF_API __declspec(dllexport)
#else
#define UF_API
#endif
#else
#ifdef _UF_INTERNAL_
#define UF_API __declspec(dllimport)
#else
#define UF_API
#endif
#endif // _UF_BUILD_DLL_
#else
#error Uniform only supports Windows!
#endif // _UF_PLATFORM_WINDOWS_

#endif // _UF_DEFINES_H_
