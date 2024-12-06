#include "lt2Dprims.h"

#include "../../../include/lotus2D.h"
#include "../../../lotus.core/utility/ltmath.h"
#include "../../../lotus.core/graphics/gl/ltglapi.h"
#include "../../../lotus.core/graphics/renderer/ltrenderer.h"

#include "../../../lotus.ECS/component/ltcomponent.h"

void ltDrawMesh2D(LTentity entity) {
    LTscene* scene2D = ltGetScene2D();
    if (scene2D == (void*)0 || !scene2D->componentManager.hasComponent(entity, LOTUS_MESH_COMPONENT)) return;

    LTcomponent component = scene2D->componentManager.getComponent(entity, LOTUS_MESH_COMPONENT);
    if (component.type == LOTUS_MESH_COMPONENT) {        

        LTmeshComponent mesh = component.data.mesh;
        LTrenderState* renderState = ltGetRenderState();
        ltRendererSetVertexData(renderState, mesh.vdata);

        if (!scene2D->componentManager.hasComponent(entity, LOTUS_TRANSFORM_COMPONENT)) {
            ltglSetUniform(*renderState->shaderProgram, LOTUS_UNIFORM_MAT4, "uModel", &LOTUS_IDENTITY_MATRIX);
        } else {
            component = scene2D->componentManager.getComponent(entity, LOTUS_TRANSFORM_COMPONENT);
            if (component.type == LOTUS_TRANSFORM_COMPONENT) {
                LTtransformComponent transform = component.data.transform;
                ltglSetUniform(*renderState->shaderProgram, LOTUS_UNIFORM_MAT4, "uModel", transform.model);
            }
        }
        ltglSetUniform(*renderState->shaderProgram, LOTUS_UNIFORM_MAT4, "uProj", &renderState->mProj);
        ltRendererDraw(renderState);
    }
} 

b8 ltTriangleMesh2D(LTentity entity, f32 width, f32 height, f32 r, f32 g, f32 b) {
    f32 halfWidth = width / 2.0f;
    f32 halfHeight = height / 2.0f;

    f32 vertices[] = {
        //  uLocation         uColor            uTexCoord
        -halfWidth, -halfHeight, 0.0,  r, g, b,  0.0, 0.0,
         halfWidth, -halfHeight, 0.0,  r, g, b,  1.0, 0.0,
         0.0,        halfHeight, 0.0,  r, g, b,  0.5, 1.0
    };

    LTvertexBuffer buffer = {
        .nAttributes=3,
        .nVertices=3,
        .nIndices=0,
        .vertices=vertices,
        .indices=(void*)0
    };
    
    return ltMeshUpload(entity, buffer);
}

b8 ltRectangleMesh2D(LTentity entity, f32 width, f32 height, f32 r, f32 g, f32 b) {
    f32 halfWidth = width / 2.0f;
    f32 halfHeight = height / 2.0f;

    f32 vertices[] = {
        //   uLocation         uColor            uTexCoord
        -halfWidth, -halfHeight, 0.0,  r, g, b,  0.0, 0.0,
         halfWidth, -halfHeight, 0.0,  r, g, b,  1.0, 0.0,
         halfWidth,  halfHeight, 0.0,  r, g, b,  1.0, 1.0,
        -halfWidth,  halfHeight, 0.0,  r, g, b,  0.0, 1.0
    };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0 
    };

    LTvertexBuffer buffer = {
        .nAttributes=3,
        .nIndices=6,
        .indices=indices,
        .nVertices=4,
        .vertices=vertices
    };
    
    return ltMeshUpload(entity, buffer);
}


b8 ltCircleMesh2D(LTentity entity, f32 radius, u32 segments, f32 r, f32 g, f32 b) {
    f32* vertices = malloc((segments + 1) * 8 * sizeof(f32));  // 3 position + 3 color + 2 texcoord
    u32* indices = malloc(segments * 3 * sizeof(u32));

    // center vertex (first vertex in the array)
    vertices[0] = 0.0f;  // x
    vertices[1] = 0.0f;  // y
    vertices[2] = 0.0f;  // z
    vertices[3] = r;     // color r
    vertices[4] = g;     // color g
    vertices[5] = b;     // color b
    vertices[6] = 0.5f;  // texcoord x (center)
    vertices[7] = 0.5f;  // texcoord y (center)

    // vertices around the circle's perimeter
    f32 angleStep = 2.0f * (f32)LOTUS_PI / (f32)segments;
    for (u32 i = 0; i < segments; i++) {
        f32 angle = i * angleStep;
        f32 x = radius * cosf(angle);
        f32 y = radius * sinf(angle);

        // uLocation
        vertices[(i + 1) * 8 + 0] = x;
        vertices[(i + 1) * 8 + 1] = y;
        vertices[(i + 1) * 8 + 2] = 0.0f;
        
        // uColor
        vertices[(i + 1) * 8 + 3] = r;
        vertices[(i + 1) * 8 + 4] = g;
        vertices[(i + 1) * 8 + 5] = b;

        // uTexCoord (circle perimeter)
        vertices[(i + 1) * 8 + 6] = (x / radius + 1.0f) * 0.5f; // normalized X texcoord
        vertices[(i + 1) * 8 + 7] = (y / radius + 1.0f) * 0.5f; // normalized Y texcoord

        // indices for the triangle fan
        indices[i * 3 + 0] = 0;       // center vertex
        indices[i * 3 + 1] = i + 1;   // current perimeter vertex
        indices[i * 3 + 2] = (i + 1) % segments + 1;  // next perimeter vertex
    }

    LTvertexBuffer buffer = {
        .nAttributes=3,
        .nIndices=segments * 3,
        .indices=indices,
        .nVertices=segments + 1,
        .vertices=vertices
    };
    
    free(vertices);
    free(indices);
    return ltMeshUpload(entity, buffer);
}


