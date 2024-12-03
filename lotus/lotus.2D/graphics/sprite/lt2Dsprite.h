#ifndef LT2DSPRITE_H
#define LT2DSPRITE_H

#include "../../lt2Dtypes.h"

LOTUS_API_ENTRY b8 lt2dRemSpriteTexture(LT2Dsprite* sprite);
LOTUS_API_ENTRY b8 lt2dSetSpriteTexture(char* path, LT2Dsprite* sprite);
LOTUS_API_ENTRY LT2Dsprite lt2dMakeSprite(LTvec2 size, LTvec2 location, LTvec3 color, char* texturePath);

#endif