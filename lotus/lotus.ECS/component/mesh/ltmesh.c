#include "ltmesh.h"

#include "../ltcomponent.h"
#include "../../../lotus.core/utility/ltmath.h"
#include "../../../lotus.core/graphics/gl/ltglapi.h"

static LTmeshData meshInternal = {0};

b8 ltMeshDataInit(LTscene* scene) {
    if (!scene->componentManager.registerComponent(
        LOTUS_MESH_COMPONENT,
        &meshInternal,
        ltAddMesh,
        ltRemMesh,
        ltSetMesh,
        ltGetMesh,
        ltMeshDataExit
    )) return LOTUS_FALSE;
    
    meshInternal.scene = scene;
    meshInternal.init = LOTUS_TRUE;
    return LOTUS_TRUE;
}

void ltMeshDataExit(LTscene* scene) {
    scene->componentManager.unregisterComponent(LOTUS_MESH_COMPONENT);
    meshInternal.scene = (void*)0;
    meshInternal.init = LOTUS_FALSE;
}

b8 ltAddMesh(LTscene* scene, LTentity entity) {
    scene->componentManager.component[LOTUS_MESH_COMPONENT].active[entity] = LOTUS_TRUE;
    ((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->color[entity] = ltNewVec3(0, 0, 0);
    ((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->vdata[entity] = (LTvertexData){0};
    return LOTUS_TRUE;
}

b8 ltSetMesh(LTscene* scene, LTentity entity) {
    ((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->color[entity] = ltNewVec3(0, 0, 0);
    ((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->vdata[entity] = (LTvertexData){0};
    return LOTUS_TRUE;
}

b8 ltRemMesh(LTscene* scene, LTentity entity) {
    scene->componentManager.handler[LOTUS_MESH_COMPONENT].setComponent(scene, entity);
    scene->componentManager.component[LOTUS_MESH_COMPONENT].active[entity] = LOTUS_FALSE;
    return LOTUS_TRUE;
}

LTcomponent ltGetMesh(LTscene* scene, LTentity entity) {
    LTcomponent interface = { .type=LOTUS_MESH_COMPONENT };
    interface.data.mesh.color = &((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->color[entity];
    interface.data.mesh.vdata = &((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->vdata[entity];
    return interface;
}

b8 ltMeshUpload(LTentity entity, LTvertexBuffer buffer) {
    LTscene* scene = meshInternal.scene;
    if (!scene->componentManager.hasComponent(entity, LOTUS_MESH_COMPONENT)) return LOTUS_FALSE;
    ((LTmeshData*)scene->componentManager.component[LOTUS_MESH_COMPONENT].data)->vdata[entity] = ltglVertexDataBuffer(buffer);
    return LOTUS_TRUE;
}

