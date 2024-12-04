#ifndef LOTUSECSTYPES_H
#define LOTUSECSTYPES_H

#include "../lotus.core/defines.h"
#include "../lotus.core/lotustypes.h"

#define COMPONENT_MAX 0b11111111

typedef enum tagComponentID {
    COMP2D_MESH =       (u32)1 << 0,
    COMP2D_TEXTURE =    (u32)1 << 1,
    COMP2D_TRANSFORM =  (u32)1 << 2,

    COMPONENT_ID_COUNT
} ComponentID;

typedef struct Entity {
    u32 id;
    u32 cids;
    char* tag;
    void* crptr[COMPONENT_MAX];
} Entity;

#endif