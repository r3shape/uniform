// lotus.PLUG/include/lotus_plug.h

/*
    LOTUS ENGINE PLUGIN/MODDING API
*/ 

#pragma once

#include "../../lotus.CORE/include/defines.h"
#include "../../lotus.CORE/include/platform/lotus_platform.h"

#define LOTUS_ENGINE_PLUG_MAX ((1U << 8) - 1)

// plugin API configuration
#define LOTUS_DEFINE_PLUG_API_FN(ret, name, ...) ret (*name)(__VA_ARGS__)
#define LOTUS_DEFINE_PLUG_API(name, ...)   \
    typedef struct name##_API { \
        __VA_ARGS__           \
    } name##_API

#define LOTUS_DEFINE_PLUG_INIT(...) \
    void plug_init(void) {          \
        __VA_ARGS__                 \
    }

#define LOTUS_DEFINE_PLUG_SHUTDOWN(...) \
    void plug_shutdown(void) {          \
        __VA_ARGS__                     \
    }

#define LOTUS_EXPORT_PLUG_API(API_TYPE, ...)        \
    void* plug_export_api(void) {                   \
        static API_TYPE api = { __VA_ARGS__ };      \
        return &api;                                \
    }

typedef void (*Lotus_Function_Pointer)(void);
typedef struct Lotus_Plug_Function {
    const char* name;
    Lotus_Function_Pointer ptr;
} Lotus_Plug_Function;

typedef ubyte (*Lotus_Plug_Initialize)(void);
typedef void (*Lotus_Plug_Shutdown)(void);
typedef void* (*Lotus_Plug_Export_API)(void);
typedef struct Lotus_Plug {
    ubyte id;
    const char* name;
    Lotus_DyLib library;
    
    void* api;
    Lotus_Plug_Shutdown shutdown;
    Lotus_Plug_Initialize initialize;
    Lotus_Plug_Export_API export_api;
} Lotus_Plug;

typedef struct Lotus_Plug_Manager {
    ubyte plug_count;
    Lotus_Plug plugs[LOTUS_ENGINE_PLUG_MAX];
} Lotus_Plug_Manager;

typedef struct Lotus_Plug_API {
    Lotus_Plug_Manager* (*initialize)(void);
    void (*shutdown)(Lotus_Platform_API* platform_api);

    Lotus_Plug* (*load_plug)(Lotus_Platform_API* platform_api, const char* path, const char* name);
    Lotus_Plug* (*get_plug)(const char* name);
    ubyte (*unload_plug)(Lotus_Platform_API* platform_api, const char* name);
} Lotus_Plug_API;

Lotus_Plug_API* lotus_init_plug(void);
