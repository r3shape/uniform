#include <r3/r3.2D/include/r3.2D.h>

#define R3_SPRITE_MAX (1U << 16)
static struct R3_Sprite_Storage {
    Vec2 size[R3_SPRITE_MAX];
    Vec3 color[R3_SPRITE_MAX];                 // u_sprite_color;
    R3_Texture texture[R3_SPRITE_MAX];
    R3_Vertex_Data vertex[R3_SPRITE_MAX];
} sprite_store = {0};

u8 _add_sprite_impl(u32 entity) {
    sprite_store.size[entity] = (Vec2){32.0, 32.0};
    sprite_store.color[entity] = (Vec3){84/255.0, 52/255.0, 150/255.0};
    sprite_store.texture[entity] = (R3_Texture){0};
    sprite_store.vertex[entity] = r3_2D->shape2D.quad2D(sprite_store.size[entity], sprite_store.color[entity]);
    return LIBX_TRUE;
}

u8 _rem_sprite_impl(u32 entity) {
    sprite_store.size[entity] = (Vec2){0};
    sprite_store.color[entity] = (Vec3){0};
    sprite_store.texture[entity] = (R3_Texture){0};
    sprite_store.vertex[entity] = (R3_Vertex_Data){0};
    return LIBX_TRUE;
}

u8 _get_sprite_impl(u32 entity, R3_Sprite* component) {
    if (!component) return LIBX_FALSE;
    component->size = &sprite_store.size[entity];
    component->color = &sprite_store.color[entity];
    component->texture = &sprite_store.texture[entity];
    component->vertex = &sprite_store.vertex[entity];
    return LIBX_TRUE;
}

u8 _r3_register_component2D(void) {
    if (!ecsx->register_component(R3_SPRITE, &sprite_store, _add_sprite_impl, _rem_sprite_impl, _get_sprite_impl)) {
        printf("failed to register R3_Sprite component!\n");
        return LIBX_FALSE;
    }

    return LIBX_TRUE;
}

u8 _r3_unregister_component2D(void) {
    if (!ecsx->unregister_component(R3_SPRITE)) {
        printf("failed to unregister R3_Sprite component!\n");
        return LIBX_FALSE;
    }

    return LIBX_TRUE;
}
