#pragma once

#include "../../lotusECStypes.h"

typedef struct tagMeshData {
    b8 init;
    LTscene* scene;
    LTvec3 color[LOTUS_ENTITY_MAX];
    LTvertexData vdata[LOTUS_ENTITY_MAX];
} LTmeshData;

typedef struct LTmeshComponent {
    LTvec3* color;
    LTvertexData* vdata;
} LTmeshComponent;

b8 ltMeshDataInit(LTscene* scene);
void ltMeshDataExit(LTscene* scene);

b8 ltAddMesh(LTscene* scene, LTentity e);
b8 ltSetMesh(LTscene* scene, LTentity e);
b8 ltRemMesh(LTscene* scene, LTentity e);
LTcomponent ltGetMesh(LTscene* scene, LTentity entity);

LOTUS_API_ENTRY b8 ltMeshUpload(LTentity entity, LTvertexBuffer buffer);
