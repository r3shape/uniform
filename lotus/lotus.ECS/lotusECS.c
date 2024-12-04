#include "../include/lotusECS.h"

b8 ltECSInit(void) {
    // TODO: ECS initialization routine
    // populate top-level ECS state struct
    // initialize internal component registers

    return LOTUS_TRUE;
}

void ltECSExit(void) {
    // TODO: ECS shutdown routine
    // de-allocate component register memory
    // de-allocate top-level state memory
}

Entity MakeEntity(char* tag) {
    Entity e = {
        .id=0,  // generate entity ID (e.id = state.entityCount++;)
        .cids=0,
        .tag=tag
    };
    for (i32 i = 0; i < COMPONENT_MAX; i++) { e.crptr[i] = (void*)0; };
    return e;
}

b8 HasComponent(ComponentID id, Entity* e) {
    return (e->cids & id) != 0;
}

b8 SetComponent(ComponentID id, Entity* e) {
    if (HasComponent(id, e)) return LOTUS_FALSE;

    i32 crindex = ltGetBitPosition(id);
    if (e->crptr[crindex] != (void*)0) {
        printf("Component already set at index %d\n", crindex);
        return LOTUS_FALSE;
    }

    e->cids |= id;
    
    // TODO:
    // assign component register pointer from top-level structure
    // ( register pointers are assigned during lotusECS init routine )
    e->crptr[crindex]; // = lotusECS.registers[crindex]

    return LOTUS_TRUE;
}

void* GetCompRegister(ComponentID id, Entity* e) {
    i32 crindex = ltGetBitPosition(id);
    return e->crptr[crindex];
}

b8 RemComponent(ComponentID id, Entity* e) {
    if (!HasComponent(id, e)) return LOTUS_FALSE;
    e->cids &= ~id;

    // TODO:
    // cast register pointer and invalidate entity specific component data using sentinnel values
    void* rptr = GetCompRegister(id, e);

    return LOTUS_TRUE;
}

void* GetComponent(ComponentID id, Entity* e) {
    i32 crindex = ltGetBitPosition(id);
    
    // TODO:
    // cast component register pointer to specified register, and return component interface
    void* rptr = GetCompRegister(id, e);
    
    return (void*)0;
}

