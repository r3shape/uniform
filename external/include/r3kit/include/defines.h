#ifndef __R3_DEF_H__
#define __R3_DEF_H__

typedef void 			none;
typedef void* 			ptr;

typedef float 			f32, f4b;
typedef double 			f64, f8b;

typedef signed char 		s8,  s1b;
typedef signed short 		s16, s2b;
typedef signed int 		    s32, s4b;
typedef signed long long 	s64, s8b, sptr;

typedef unsigned char 		u8,  u1b;
typedef unsigned short 		u16, u2b;
typedef unsigned int 		u32, u4b;
typedef unsigned long long 	u64, u8b, uptr;

#ifdef KB
    #undef KB
#endif
#ifdef MB
    #undef MB
#endif
#ifdef GB
    #undef GB
#endif
#define KB 1000
#define MB (KB * 1000)
#define GB (MB * 1000)

#ifdef KiB
    #undef KiB
#endif
#ifdef MiB
    #undef MiB
#endif
#ifdef GiB
    #undef GiB
#endif
#define KiB 1024
#define MiB (KiB * 1024)
#define GiB (MiB * 1024)

#define I8_MAX  ((1 << 8) - 1)
#define I16_MAX ((1 << 16) - 1)
#define I32_MAX ((1ULL << 32) - 1)
#define I64_MAX ((1ULL << 64) - 1)

#define PI 3.14159265358979323846

#define FPTR(r, n, ...) r (*n)(__VA_ARGS__)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MAX3(a, b, c) MAX((a), MAX((b), (c)))
#define MIN3(a, b, c) MIN((a), MIN((b), (c)))

#define MAX4(a, b, c, d) MAX((a), MAX3((b), (c), (d)))
#define MIN4(a, b, c, d) MIN((a), MIN3((b), (c), (d)))

#define SWAP(t, a, b) { t tmp = a; (a) = (b); (b) = tmp; }

#define CLAMP(v, l, h) (((v) > (h)) ? (h) : ((v) < (l)) ? (l) : (v))

#define FOR_I(start, stop, step) for (u32 i = start; i < stop; i += step)
#define FOR_J(start, stop, step) for (u32 j = start; j < stop; j += step)
#define FOR_K(start, stop, step) for (u32 k = start; k < stop; k += step)

#define FOR(type, iter, start, stop, step) for (type iter = start; iter < stop; iter += step)

#ifndef NULL
    #ifndef _WIN64
        #define NULL 0
    #else
        #define NULL 0LL
    #endif  /* W64 */
    #else
        #define NULL ((void *)0)
#endif

#ifdef R3_BUILD_SOURCE
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

#endif // __R3_DEF_H__