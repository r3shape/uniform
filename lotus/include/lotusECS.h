#ifndef LOTUSECS_H
#define LOTUSECS_H

#include "lotus.h"
#include "../lotus.ECS/lotusECStypes.h"
#include "../lotus.ECS/scene/ltscene.h"
#include "../lotus.ECS/component/ltcomponent.h"

LOTUS_API_ENTRY b8 ltECSInit(void);
LOTUS_API_ENTRY void ltECSExit(void);

#endif