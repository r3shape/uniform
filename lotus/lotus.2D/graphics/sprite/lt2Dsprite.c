#include "lt2Dsprite.h"

#include "../primitives/lt2Dprims.h"
#include "../../../lotus.core/utility/ltmath.h"
#include "../../../lotus.core/graphics/gl/ltglapi.h"
#include "../../../lotus.core/graphics/renderer/ltrenderer.h"

LT2Dsprite lt2dMakeSprite(LTvec2 size, LTvec2 location, LTvec3 color, char* texturePath) {
    return (LT2Dsprite){
        .size = size,
        .matrix = ltIdentity(),
        .location = location,
        .primitive = lt2dMakeRectangle(size.x, size.y, color.x, color.y, color.z),
        .texture = (texturePath == NULL) ? (LTtexture2D){0} : ltglTexture2D(texturePath)
    };
}

b8 lt2dSetSpriteTexture(char* path, LT2Dsprite* sprite) {
    if (!sprite || !path) return LOTUS_FALSE;
    sprite->texture = ltglTexture2D(path);
    if (!sprite->texture.textureData) return LOTUS_FALSE;
    return LOTUS_TRUE;
}

b8 lt2dRemSpriteTexture(LT2Dsprite* sprite) {
    if (!sprite) return LOTUS_FALSE;
    if (!sprite->texture.textureData) return LOTUS_FALSE;
    sprite->texture = (LTtexture2D){0};
    return LOTUS_TRUE;
}

void lt2dDrawSprite(LT2Dsprite* sprite) {
    LTrenderState* renderState = ltGetRenderState();
    if (sprite->texture.textureData != NULL) {
        ltRendererSetTexture2D(renderState, &sprite->texture);
    }
    ltglSetUniform(*renderState->shaderProgram, LOTUS_UNIFORM_MAT4, "uProj", &renderState->mProj);
    ltglSetUniform(*renderState->shaderProgram, LOTUS_UNIFORM_MAT4, "uModel", &sprite->matrix);
    lt2dDrawPrimitive(&sprite->primitive);
}
