#ifndef _UFMRG_H_
#define _UFMRG_H_

#define _UF_R3DEF_
#include <ufcore/UFGPU/UFGPU.h>

typedef struct UFMRGInterface {
    UFVersion ver;

    UF_API_METHOD(none, newFrame, none);
    UF_API_METHOD(none, endFrame, none);

    UF_API_METHOD(none, newCall, UFGPUCall call);

    UF_API_METHOD(UFResource, newPhase, UFGPUPhaseDesc phase);
    UF_API_METHOD(none, delPhase, UFResource phase);

    UF_API_METHOD(UFResource, newPipeline, UFGPUPipelineDesc pipeline);
    UF_API_METHOD(none, delPipeline, UFResource pipeline);
} UFMRGInterface;

#endif // _UFMRG_H_
