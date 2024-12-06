#pragma once

#include "../../lotusECStypes.h"

typedef struct LTtransformData {
    b8 init;
    LTscene* scene;
    LTmat4 model[LOTUS_ENTITY_MAX];
    LTvec3 scale[LOTUS_ENTITY_MAX];
    LTvec3 rotation[LOTUS_ENTITY_MAX];
    LTvec3 location[LOTUS_ENTITY_MAX];
} LTtransformData;

typedef struct tagTransformComponent {
    LTmat4* model;
    LTvec3* scale;
    LTvec3* rotation;
    LTvec3* location;
} LTtransformComponent;

b8 ltTransformDataInit(LTscene* scene);
void ltTransformDataExit(LTscene* scene);

b8 ltAddTransform(LTscene* scene, LTentity e);
b8 ltSetTransform(LTscene* scene, LTentity e);
b8 ltRemTransform(LTscene* scene, LTentity e);
LTcomponent ltGetTransform(LTscene* scene, LTentity entity);
