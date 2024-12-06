#include "ltcomponent.h"

b8 ltComponentsInit(LTscene* scene) {
    if (!ltMeshDataInit(scene)) return LOTUS_FALSE;
    if (!ltTransformDataInit(scene)) return LOTUS_FALSE;

    return LOTUS_TRUE;
}

void ltComponentsExit(LTscene* scene) {
    ltMeshDataExit(scene);
    ltTransformDataExit(scene);
}

