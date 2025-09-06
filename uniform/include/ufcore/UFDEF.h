#ifndef _UF_DEFINES_H_
#define _UF_DEFINES_H_

#ifdef _UF_R3DEF_
#include <r3kit/include/defines.h>

#include <r3kit/include/ds/arr.h>

#include <r3kit/include/io/log.h>

#include <r3kit/include/math.h>

#include <r3kit/include/mem/alloc.h>
#include <r3kit/include/mem/arena.h>
#endif

typedef char* CString;

typedef unsigned long long UFResource;
typedef unsigned int UFVersion, UFEventCode;

typedef enum UFGpuApi {
    UF_GPU_INVALID_API,
    UF_GPU_GL_API,  // OpenGL
    UF_GPU_DX_API,  // DirectX
    UF_GPU_VK_API,  // Vulkan
    UF_GPU_MT_API   // Metal
} UFGpuApi;

typedef enum UFGpuCtx {
    UF_GPU_INVALID_CTX,
    UF_GPU_2D_CTX,
    UF_GPU_3D_CTX
} UFGpuCtx;

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
