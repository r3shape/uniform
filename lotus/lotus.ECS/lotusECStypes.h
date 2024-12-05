#ifndef LOTUSECSTYPES_H
#define LOTUSECSTYPES_H

#include "../lotus.core/defines.h"
#include "../lotus.core/lotustypes.h"

#define LOTUS_ENTITY_MAX        (u16)1 << 16

typedef i16 LTentity;
typedef struct LTscene LTscene;

typedef enum LTcomponentType {
    LOTUS_MESH_COMPONENT,
    LOTUS_TEXTURE_COMPONENT,
    LOTUS_TRANSFORM_COMPONENT,
    LOTUS_COMPONENT_TYPES,
    LOTUS_COMPONENT_MAX = (u8)1 << 8
} LTcomponentType;

typedef struct LTcomponent LTcomponent;
typedef union LTcomponentData LTcomponentData;

typedef struct LTcomponentRegister {
    void* data;
    u8 active[LOTUS_ENTITY_MAX];
} LTcomponentRegister;

typedef struct LTcomponentHandler {
    void (*destroyComponent)(LTscene* scene);
    b8 (*addComponent)(LTscene* scene, LTentity entity);
    b8 (*remComponent)(LTscene* scene, LTentity entity);
    b8 (*setComponent)(LTscene* scene, LTentity entity);
    LTcomponent (*getComponent)(LTscene* scene, LTentity entity);
} LTcomponentHandler;

struct LTscene {
    struct info {
        char* name;
    } info;

    struct entityManager {
        u16 count;
        u8 dead[LOTUS_ENTITY_MAX];
    
        LTentity(*makeEntity)(void);
        b8(*killEntity)(LTentity entity);
        b8(*validEntity)(LTentity entity);
    } entityManager;

    struct componentManager {
        LTcomponentHandler handler[LOTUS_COMPONENT_TYPES];
        LTcomponentRegister component[LOTUS_COMPONENT_TYPES];

        b8 (*registerComponent)(
            LTcomponentType type,
            void* data,
            b8 (*add)(LTscene* scene, LTentity entity),
            b8 (*rem)(LTscene* scene, LTentity entity),
            b8 (*set)(LTscene* scene, LTentity entity),
            LTcomponent (*get)(LTscene* scene, LTentity entity),
            void (*destroy)(LTscene* scene)
        );
        b8 (*unregisterComponent)(LTcomponentType type);

        b8 (*hasComponent)(LTentity entity, LTcomponentType type);
        b8 (*addComponent)(LTentity entity, LTcomponentType type);
        b8 (*remComponent)(LTentity entity, LTcomponentType type);
        b8 (*setComponent)(LTentity entity, LTcomponentType type);
        LTcomponent (*getComponent)(LTentity entity, LTcomponentType type);
        
    } componentManager;
    
};

#endif