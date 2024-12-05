#include "lttransform.h"

#include "../ltcomponent.h"
#include "../../../lotus.core/utility/ltmath.h"

static LTtransformData transformInternal;

b8 ltTransformDataInit(LTscene* scene) {
    if (!scene->componentManager.registerComponent(
        LOTUS_TRANSFORM_COMPONENT,
        &transformInternal,
        ltAddTransform,
        ltRemTransform,
        ltSetTransform,
        ltGetTransform,
        ltTransformDataExit
    )) return LOTUS_FALSE;
    
    transformInternal.init = LOTUS_TRUE;
    return LOTUS_TRUE;
}

void ltTransformDataExit(LTscene* scene) {
    scene->componentManager.unregisterComponent(LOTUS_TRANSFORM_COMPONENT);
    transformInternal.init = LOTUS_FALSE;
}

b8 ltAddTransform(LTscene* scene, LTentity entity) {
    scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].active[entity] = LOTUS_TRUE;
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->model[entity] = ltIdentity();
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->scale[entity] = ltNewVec3(0, 0, 0);
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->rotation[entity] = ltNewVec3(0, 0, 0);
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->location[entity] = ltNewVec3(0, 0, 0);
    return LOTUS_TRUE;
}

b8 ltSetTransform(LTscene* scene, LTentity entity) {
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->model[entity] = ltIdentity();
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->scale[entity] = ltNewVec3(0, 0, 0);
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->rotation[entity] = ltNewVec3(0, 0, 0);
    ((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->location[entity] = ltNewVec3(0, 0, 0);
    return LOTUS_TRUE;
}

b8 ltRemTransform(LTscene* scene, LTentity entity) {
    scene->componentManager.handler[LOTUS_TRANSFORM_COMPONENT].setComponent(scene, entity);
    scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].active[entity] = LOTUS_FALSE;
    return LOTUS_TRUE;
}

LTcomponent ltGetTransform(LTscene* scene, LTentity entity) {
    LTcomponent interface = { .type=LOTUS_TRANSFORM_COMPONENT };
    interface.data.transform.model = &((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->model[entity];
    interface.data.transform.scale = &((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->scale[entity];
    interface.data.transform.rotation = &((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->rotation[entity];
    interface.data.transform.location = &((LTtransformData*)scene->componentManager.component[LOTUS_TRANSFORM_COMPONENT].data)->location[entity];
    return interface;
}

