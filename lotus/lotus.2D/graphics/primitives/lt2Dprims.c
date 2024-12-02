#include "lt2Dprims.h"

#include "../../../lotus.core/utility/ltmath.h"
#include "../../../lotus.core/graphics/gl/ltglapi.h"

LT2Dprimitive lt2dMakeTriangle(f32 width, f32 height, f32 r, f32 g, f32 b) {
    f32 halfWidth = width / 2.0f;
    f32 halfHeight = height / 2.0f;

    f32 vertices[] = {
        //  uLocation         uColor
        -halfWidth, -halfHeight, 0.0,  r, g, b,
         halfWidth, -halfHeight, 0.0,  r, g, b,
         0.0,        halfHeight, 0.0,  r, g, b,
    };

    return (LT2Dprimitive){
        .color = (LTvec3){r, g, b},
        .vertexData = ltglVertexData(2, 3, 0, vertices, (void*)0)
    };
}

LT2Dprimitive lt2dMakeRectangle(f32 width, f32 height, f32 r, f32 g, f32 b) {
    f32 halfWidth = width / 2.0f;
    f32 halfHeight = height / 2.0f;

    // Define 4 unique vertices for the rectangle
    f32 vertices[] = {
        //   uLocation         uColor
        -halfWidth, -halfHeight, 0.0,  r, g, b,  // Bottom-left (0)
         halfWidth, -halfHeight, 0.0,  r, g, b,  // Bottom-right (1)
         halfWidth,  halfHeight, 0.0,  r, g, b,  // Top-right (2)
        -halfWidth,  halfHeight, 0.0,  r, g, b   // Top-left (3)
    };

    // Define indices for two triangles
    u32 indices[] = {
        0, 1, 2,  // Bottom-left triangle
        2, 3, 0   // Top-right triangle
    };

    return (LT2Dprimitive){
        .color = (LTvec3){r, g, b},
        .vertexData = ltglVertexData(2, 4, 6, vertices, indices)
    };
}

LT2Dprimitive lt2dMakeCircle(f32 radius, u32 segments, f32 r, f32 g, f32 b) {
    // Allocate memory for vertices and indices
    f32* vertices = malloc((segments + 1) * 6 * sizeof(f32));  // 3 position + 3 color
    u32* indices = malloc(segments * 3 * sizeof(u32));         // Triangle fan indices

    // Center vertex (first vertex in the array)
    vertices[0] = 0.0f;  // x
    vertices[1] = 0.0f;  // y
    vertices[2] = 0.0f;  // z
    vertices[3] = r;     // color r
    vertices[4] = g;     // color g
    vertices[5] = b;     // color b

    // vertices around the circle's perimeter
    f32 angleStep = 2.0f * (f32)LOTUS_PI / (f32)segments;
    for (u32 i = 0; i < segments; i++) {
        f32 angle = i * angleStep;
        f32 x = radius * cosf(angle);
        f32 y = radius * sinf(angle);

        // uLocation
        vertices[(i + 1) * 6 + 0] = x;
        vertices[(i + 1) * 6 + 1] = y;
        vertices[(i + 1) * 6 + 2] = 0.0f;
        
        // uColor
        vertices[(i + 1) * 6 + 3] = r;
        vertices[(i + 1) * 6 + 4] = g;
        vertices[(i + 1) * 6 + 5] = b;

        // indices for the triangle fan
        indices[i * 3 + 0] = 0;       // center vertex
        indices[i * 3 + 1] = i + 1;   // current perimeter vertex
        indices[i * 3 + 2] = (i + 1) % segments + 1;  // next perimeter vertex
    }

    LT2Dprimitive circle = {
        .color = (LTvec3){r, g, b},
        .vertexData = ltglVertexData(2, segments + 1, segments * 3, vertices, indices)
    };

    free(vertices);
    free(indices);
    return circle;
}


