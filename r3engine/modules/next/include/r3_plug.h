#pragma once

#include "../../core/include/r3_core_defines.h"

#define R3_ENGINE_PLUG_MAX ((1U << 8) - 1)

#define R3_PLUG_API_FUNC(returns, name, ...) returns (*name)(__VA_ARGS__)
#define R3_PLUG_API(name, ...)   \
    typedef struct name##_API {     \
        __VA_ARGS__                 \
    } name##_API                    \

#define R3_PLUG_INIT(...)        \
    ubyte _plug_init_impl(void) {   \
        __VA_ARGS__                 \
    }

#define R3_PLUG_SHUTDOWN(...)        \
    void _plug_shutdown_impl(void) {    \
        __VA_ARGS__                     \
    }

#define R3_PLUG_EXPORT(plug_api, ...)            \
    void* _plug_export_api_impl(void) {             \
        static plug_api _api = { __VA_ARGS__ };     \
        return &_api;                               \
    }

typedef struct R3_Plug_API {
    void* (*load_plug)(char* name, char* path);
    void* (*get_plug)(char* name);
    ubyte (*unload_plug)(char* name);
} R3_Plug_API;

extern R3_Plug_API* r3_plug_api;

ubyte r3_init_plug(void);
void r3_shutdown_plug(void);
