#ifndef __SWARM_PLATFORM_H__
#define __SWARM_PLATFORM_H__

#include <include/swarm/core/globals.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define SWARM_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__gnu_linux__)
    #define SWARM_PLATFORM_LINUX
#endif

typedef enum SwarmWindowFlag {
    WINDOW_FOCUSED =         (1 << 0),
    WINDOW_SHOW_CURSOR =     (1 << 1),
    WINDOW_BIND_CURSOR =     (1 << 2),
    WINDOW_CENTER_CURSOR =   (1 << 3)
} SwarmWindowFlag;

typedef struct SwarmWindow {
    cstr title;             // Window title
    f32 aspect_ratio;       // Aspect ratio (width / height)
    Vec2 location;          // Window x, y position
    Vec2 size;              // Window width and height
    u32 flags;              // Window flags
} SwarmWindow;

typedef struct SwarmLib {
    ptr handle;
    cstr name;
} SwarmLib;

typedef struct SwarmPlatform {
    none (*init)(u32 backend);
    none (*shutdown)(none);

    byte (*createWindow)(cstr title, u32 width, u32 height);
    none (*destroyWindow)(none);
    
    none (*swapBuffers)(none);
    byte (*createGLContext)(none);
    byte (*attachGLContext)(none);
    none (*destroyGLContext)(none);

    none (*pollEvents)(none);
    none (*pollInputs)(none);

    byte (*getWindowFlag)(u32 flag);
    byte (*setWindowFlag)(u32 flag);
    byte (*remWindowFlag)(u32 flag);
    
    byte (*unloadLib)(SwarmLib* library);
    SwarmLib (*loadLib)(cstr path, cstr name);
    ptr (*loadSymbol)(cstr name, SwarmLib* library);
} SwarmPlatform;
extern SwarmPlatform* swarmPlatform;

#endif  // __SWARM_PLATFORM_H__