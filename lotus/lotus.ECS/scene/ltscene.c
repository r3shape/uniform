#include "ltscene.h"

#include "../../lotus.core/memory/ltmemory.h"
#include "../../lotus.core/platform/ltlogger.h"

static LTscene* self;

/* SCENE FUNCTION POINTERS */
LTentity _makeEntityPTR(void) {
    if (self->entityManager.count + 1 > LOTUS_ENTITY_MAX) return -1;
    return self->entityManager.count++;
}

b8 _validEntityPTR(LTentity entity) {
    if (
        entity < 0                           ||
        entity >= LOTUS_ENTITY_MAX           ||
        self->entityManager.dead[entity]     ||
        entity > self->entityManager.count
    ) return LOTUS_FALSE;
    return LOTUS_TRUE;
}

b8 _killEntityPTR(LTentity entity) {
    if (!self->entityManager.validEntity(entity)) return LOTUS_FALSE;
    
    self->entityManager.count--;
    self->entityManager.dead[entity] = LOTUS_TRUE;
    return LOTUS_TRUE;
}

b8 _registerComponentPTR(
    LTcomponentType type,
    void* data,
    b8 (*add)(LTscene* scene, LTentity entity),
    b8 (*rem)(LTscene* scene, LTentity entity),
    b8 (*set)(LTscene* scene, LTentity entity),
    LTcomponent (*get)(LTscene* scene, LTentity entity),
    void (*destroy)(LTscene* scene)
) {
    if (type >= LOTUS_COMPONENT_TYPES) return LOTUS_FALSE;
    
    self->componentManager.handler[type] = (LTcomponentHandler){
        .addComponent=add,
        .remComponent=rem,
        .setComponent=set,
        .getComponent=get,
        .destroyComponent=destroy
    };
    self->componentManager.component[type] = (LTcomponentRegister){.data=data};
    
    return LOTUS_TRUE;
}

b8 _unregisterComponentPTR(LTcomponentType type) {
    if (type >= LOTUS_COMPONENT_TYPES) return LOTUS_FALSE;
    
    self->componentManager.handler[type] = (LTcomponentHandler){
        .addComponent=(void*)0,
        .remComponent=(void*)0,
        .setComponent=(void*)0,
        .getComponent=(void*)0,
        .destroyComponent=(void*)0
    };
    self->componentManager.component[type] = (LTcomponentRegister){.data=(void*)0};
    
    return LOTUS_TRUE;
}

b8 _hasComponentPTR(LTentity entity, LTcomponentType type) {
    if (
        !self->entityManager.validEntity(entity) ||
        !self->componentManager.component[type].active[entity]
    ) return LOTUS_FALSE;
    return LOTUS_TRUE;
}

b8 _addComponentPTR(LTentity entity, LTcomponentType type) {
    if (self->componentManager.hasComponent(entity, type)) return LOTUS_FALSE;
    return self->componentManager.handler[type].addComponent(self, entity);
}

b8 _remComponentPTR(LTentity entity, LTcomponentType type) {
    if (!self->componentManager.hasComponent(entity, type)) return LOTUS_FALSE;
    return self->componentManager.handler[type].remComponent(self, entity);
}

b8 _setComponentPTR(LTentity entity, LTcomponentType type) {
    if (!self->componentManager.hasComponent(entity, type)) return LOTUS_FALSE;
    return self->componentManager.handler[type].setComponent(self, entity);
}

LTcomponent _getComponentPTR(LTentity entity, LTcomponentType type) {
    if (!self->componentManager.hasComponent(entity, type)) return (LTcomponent){0};
    return self->componentManager.handler[type].getComponent(self, entity);
}

LTscene* ltMakeScene(char* name) {
    LTscene* scene = (LTscene*)ltMemAlloc(sizeof(LTscene), LOTUS_MEMTAG_SCENE);
    if (!scene) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to allocate scene structure!\n");
        return (void*)0;
    }
    
    self = scene;
    scene->info.name = (name != NULL) ? name : "Lotus Scene";

    scene->entityManager.count = 0;
    scene->entityManager.makeEntity = _makeEntityPTR;
    scene->entityManager.killEntity = _killEntityPTR;
    scene->entityManager.validEntity = _validEntityPTR;
    
    scene->componentManager.registerComponent = _registerComponentPTR;
    scene->componentManager.unregisterComponent = _unregisterComponentPTR;
    
    if (!ltComponentsInit(scene)) {
        printf("FAILED INIT COMPONENTS\n");
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("Failed to allocate scene structure!\n");
        ltMemFree(scene, sizeof(LTscene), LOTUS_MEMTAG_SCENE);
        return (void*)0;
    }

    scene->componentManager.hasComponent = _hasComponentPTR;
    scene->componentManager.addComponent = _addComponentPTR;
    scene->componentManager.setComponent = _setComponentPTR;
    scene->componentManager.remComponent = _remComponentPTR;
    scene->componentManager.getComponent = _getComponentPTR;

    return scene;
}

void ltDestroyScene(LTscene* scene) {
    if (!scene) return;

    for (int i = 0; i < LOTUS_COMPONENT_TYPES; i++) {
        if (scene->componentManager.component[i].data) {
            scene->componentManager.handler[i].destroyComponent(scene);
            scene->componentManager.component[i].data = NULL;
        }
        scene->componentManager.handler[i] = (LTcomponentHandler){0};
    }

    scene->info.name = "Lotus Scene";

    scene->entityManager.count = 0;
    scene->entityManager.makeEntity = (void*)0;
    scene->entityManager.killEntity = (void*)0;
    scene->entityManager.validEntity = (void*)0;
    
    scene->componentManager.registerComponent = (void*)0;
    scene->componentManager.unregisterComponent = (void*)0;
    
    scene->componentManager.hasComponent = (void*)0;
    scene->componentManager.addComponent = (void*)0;
    scene->componentManager.setComponent = (void*)0;
    scene->componentManager.remComponent = (void*)0;
    scene->componentManager.getComponent = (void*)0;

    ltMemFree(scene, sizeof(LTscene), LOTUS_MEMTAG_SCENE);
    self = NULL;
}


