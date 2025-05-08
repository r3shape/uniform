#ifndef __SWARM_RUNTIME_H__
#define __SWARM_RUNTIME_H__

#include <include/swarm/core/swarm_inputs.h>
#include <include/swarm/core/swarm_events.h>
#include <include/swarm/core/swarm_renderer.h>

typedef struct RuntimeConfig {
    cstr title;
    Vec2 windowPos;
    Vec2 windowSize;
    GPUBackend backend;
} RuntimeConfig;

typedef none (*RuntimeInitFunc)(none);
typedef none (*RuntimeUpdateFunc)(f32);
typedef none (*RuntimeRenderFunc)(none);
typedef none (*RuntimeShutdownFunc)(none);
typedef none (*RuntimeConfigureFunc)(RuntimeConfig config);

typedef struct RuntimeAPI {
    RuntimeInitFunc init;
    RuntimeUpdateFunc update;
    RuntimeRenderFunc render;
    RuntimeShutdownFunc shutdown;
    RuntimeConfigureFunc configure;
} RuntimeAPI;
typedef none (*RuntimeExportFunc)(RuntimeAPI* api);

#endif  // __SWARM_RUNTIME_H__