#pragma once

#include "mesh/ltmesh.h"
#include "texture/lttexture.h"
#include "transform/lttransform.h"

union LTcomponentData {
    LTtransform_itf transform;
};

struct LTcomponent {
    LTcomponentType type;
    LTcomponentData data;
};

b8 ltComponentsInit(LTscene* scene);
void ltComponentsExit(LTscene* scene);

b8 _registerComponentPTR(
    LTcomponentType type,
    void* data,
    b8 (*add)(LTscene* scene, LTentity entity),
    b8 (*rem)(LTscene* scene, LTentity entity),
    b8 (*set)(LTscene* scene, LTentity entity),
    LTcomponent (*get)(LTscene* scene, LTentity entity),
    void (*destroy)(LTscene* scene)
);
b8 _unregisterComponentPTR(LTcomponentType type);

b8 _hasComponentPTR(LTentity entity, LTcomponentType type);
b8 _addComponentPTR(LTentity entity, LTcomponentType type);
b8 _remComponentPTR(LTentity entity, LTcomponentType type);
b8 _setComponentPTR(LTentity entity, LTcomponentType type);
LTcomponent _getComponentPTR(LTentity entity, LTcomponentType type);
