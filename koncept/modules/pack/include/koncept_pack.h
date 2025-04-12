#pragma once

#include <koncept/core/koncept_core.h>

#define KC_PACK_MAX ((1U << 8) - 1)

#define KC_PACK_METHOD(returns, name, ...) returns (*name)(__VA_ARGS__)
#define KC_PACK_API(name, ...)      \
    typedef struct name##_api {     \
        __VA_ARGS__                 \
    } name##_api                    \

#define KC_PACK_INIT(...)           \
    u8 _pack_init_impl(void) {      \
        __VA_ARGS__                 \
    }

#define KC_PACK_CLEANUP(...)            \
    u8 _pack_cleanup_impl(void) {       \
        __VA_ARGS__                     \
    }

#define KC_PACK_EXPORT(pack_api, ...)               \
    void main(void) {}                              \
    void* _pack_export_impl(void) {                 \
        static pack_api _api = { __VA_ARGS__ };     \
        return &_api;                               \
    }

typedef struct _koncept_pack_api {
    void* (*load_pack)(char* name, char* path);
    void* (*get_pack)(char* name);
    u8 (*unload_pack)(char* name);
} _koncept_pack_api;
extern _koncept_pack_api* koncept_pack;

u8 koncept_init_pack(void* koncept_core);
u8 koncept_cleanup_pack(void);
