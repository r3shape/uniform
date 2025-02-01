#pragma once

#include "../../core/include/lotus_core_defines.h"

#define LOTUS_ENGINE_PLUG_MAX ((1U << 8) - 1)

#define LOTUS_PLUG_API_FUNC(returns, name, ...) returns (*name)(__VA_ARGS__)
#define LOTUS_PLUG_API(name, ...)   \
    typedef struct name##_API {     \
        __VA_ARGS__                 \
    } name##_API                    \

#define LOTUS_PLUG_INIT(...)        \
    ubyte _plug_init_impl(void) {   \
        __VA_ARGS__                 \
    }

#define LOTUS_PLUG_SHUTDOWN(...)        \
    void _plug_shutdown_impl(void) {    \
        __VA_ARGS__                     \
    }

#define LOTUS_PLUG_EXPORT(plug_api, ...)            \
    void* _plug_export_api_impl(void) {             \
        static plug_api _api = { __VA_ARGS__ };     \
        return &_api;                               \
    }

typedef struct Lotus_Plug_API {
    void* (*load_plug)(char* name, char* path);
    void* (*get_plug)(char* name);
    ubyte (*unload_plug)(char* name);
} Lotus_Plug_API;

extern Lotus_Plug_API* lotus_plug_api;

ubyte lotus_init_plug(void);
void lotus_shutdown_plug(void);
