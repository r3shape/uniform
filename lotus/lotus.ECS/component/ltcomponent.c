#include "ltcomponent.h"

b8 ltComponentsInit(LTscene* scene) {
    if (!ltTransformDataInit(scene)) return LOTUS_FALSE;

    return LOTUS_TRUE;
}

void ltComponentsExit(LTscene* scene) {
    ltTransformDataExit(scene);
}

