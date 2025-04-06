#pragma once

#include <r3/r3.core/include/r3.core.h>

#define R3_SPRITE (1 << 0)
typedef struct R3_Sprite {
    Vec2* size;
    Vec3* color;                 // u_sprite_color;
    R3_Texture* texture;
    R3_Vertex_Data* vertex;
} R3_Sprite;

