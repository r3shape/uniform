#ifndef __SWARM_H__
#define __SWARM_H__

#include <include/swarm/core/swarm_inputs.h>
#include <include/swarm/core/swarm_events.h>
#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/core/swarm_renderer.h>

SSDK_FUNC none swarmInit(GPUBackend backend);
SSDK_FUNC none swarmShutdown(none);

#endif  // __SWARM_H__