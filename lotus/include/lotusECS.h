#ifndef LOTUSECS_H
#define LOTUSECS_H

#include "lotus.h"
#include "../lotus.ECS/lotusECStypes.h"
#include "../lotus.ECS/register/ltcregister.h"
#include "../lotus.ECS/component/ltcomponent.h"

LOTUS_API_ENTRY b8 ltECSInit(void);
LOTUS_API_ENTRY void ltECSExit(void);

LOTUS_API_ENTRY Entity MakeEntity(char* tag);

LOTUS_API_ENTRY b8 HasComponent(ComponentID id, Entity* e);
LOTUS_API_ENTRY b8 SetComponent(ComponentID id, Entity* e);
LOTUS_API_ENTRY b8 RemComponent(ComponentID id, Entity* e);
LOTUS_API_ENTRY void* GetComponent(ComponentID id, Entity* e);
LOTUS_API_ENTRY void* GetCompRegister(ComponentID id, Entity* e);

#endif