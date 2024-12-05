#ifndef LTSCENE_H
#define LTSCENE_H

#include "../lotusECStypes.h"
#include "../component/ltcomponent.h"

LTentity _makeEntityPTR(void);
b8 _killEntityPTR(LTentity entity);
b8 _validEntityPTR(LTentity entity);

LOTUS_API_ENTRY LTscene* ltMakeScene(char* name);
LOTUS_API_ENTRY void ltDestroyScene(LTscene* scene);

#endif