#define SWARM_CORE
#include <include/swarm/swarm.h>

none swarmInit(GPUBackend backend) {
    ssdkInitMemory();
    ssdkInitMath();
    ssdkInitLog();
    ssdkInitDS();

    swarmEvents->init();
    swarmInputs->init();
    swarmPlatform->init(backend);
}

none swarmShutdown(none) {
    swarmPlatform->shutdown();
    swarmInputs->shutdown();
    swarmEvents->shutdown();
    
    ssdkExitDS();
    ssdkExitLog();
    ssdkExitMath();
    ssdkExitMemory();
}
