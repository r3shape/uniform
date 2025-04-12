#include <r3/modules/2D/include/r3.2D.h>

static Mat4 identity;

static struct R3_Sprite_Storage {
    Vec2 size[ENTITY_MAX];
    Vec3 color[ENTITY_MAX];                 // u_sprite_color;
    R3_Texture texture[ENTITY_MAX];
    R3_Vertex_Data vertex[ENTITY_MAX];
} sprite_store = {0};

static struct R3_Transform_Storage {
    f32 speed[ENTITY_MAX];
    Mat4 model[ENTITY_MAX];
    Vec2 scale[ENTITY_MAX];
    f32 rotation[ENTITY_MAX];
    Vec2 velocity[ENTITY_MAX];
    Vec2 location[ENTITY_MAX];
} transform_store = {0};


u8 _add_sprite_impl(u32 entity) {
    sprite_store.size[entity] = (Vec2){32.0, 32.0};
    sprite_store.color[entity] = (Vec3){84/255.0, 52/255.0, 150/255.0};
    sprite_store.texture[entity] = (R3_Texture){.id = 0};
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

u8 _get_sprite_impl(u32 entity, R3_Sprite2D* component) {
    component->size = &sprite_store.size[entity];
    component->color = &sprite_store.color[entity];
    component->texture = &sprite_store.texture[entity];
    component->vertex = &sprite_store.vertex[entity];
    return LIBX_TRUE;
}

u8 _sprite_system_impl(u32 entity) {
    r3_core->graphics.push_pipeline(&(R3_Render_Call){
        .mode = R3_TRIANGLE_MODE,
        .vertex = &sprite_store.vertex[entity],
        .type = (sprite_store.vertex[entity].indexCount) ? R3_RENDER_ELEMENTS : R3_RENDER_ARRAYS,
        .model = (libx->ecsx.has_component(R3_TRANSFORM2D, entity)) ? &transform_store.model[entity] : &identity,
        .shader = NULL, .texture = (sprite_store.texture[entity].id) ? &sprite_store.texture[entity] : NULL,
        .uniform_count = 1, .uniforms = {
            (R3_Uniform){
                .name = "u_sprite_color",
                .type = R3_UNIFORM_VEC3,
                .value = &sprite_store.color[entity]
            }
        }
    });

    return LIBX_TRUE;
}


u8 _add_transform_impl(u32 entity) {
    transform_store.speed[entity] = 10.0;
    transform_store.model[entity] = libx->mathx.mat.identity4();
    transform_store.scale[entity] = (Vec2){1, 1};
    transform_store.rotation[entity] = 0.0;
    transform_store.velocity[entity] = (Vec2){0, 0};
    transform_store.location[entity] = (Vec2){0, 0};
    return LIBX_TRUE;
}

u8 _rem_transform_impl(u32 entity) {
    transform_store.speed[entity] = 0.0;
    transform_store.model[entity] = (Mat4){0};
    transform_store.scale[entity] = (Vec2){0};
    transform_store.rotation[entity] = 0.0;
    transform_store.velocity[entity] = (Vec2){0};
    transform_store.location[entity] = (Vec2){0};
    return LIBX_TRUE;
}

u8 _get_transform_impl(u32 entity, R3_Transform2D* component) {
    component->speed = &transform_store.speed[entity];
    component->model = &transform_store.model[entity];
    component->scale = &transform_store.scale[entity];
    component->rotation = &transform_store.rotation[entity];
    component->velocity = &transform_store.velocity[entity];
    component->location = &transform_store.location[entity];
    return LIBX_TRUE;
}

u8 _transform_system_impl(u32 entity) {
    transform_store.velocity[entity] = libx->mathx.vec.scale2(
        transform_store.velocity[entity],
        transform_store.speed[entity]
    );

    transform_store.location[entity] = libx->mathx.vec.add2(
        transform_store.location[entity],
        transform_store.velocity[entity]
    );

    transform_store.model[entity] = libx->mathx.mat.identity4();
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.scale4(
            transform_store.scale[entity].x,
            transform_store.scale[entity].y,
            1.0
    ));

    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.rotz4(transform_store.rotation[entity])
    );
    
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.trans4(
            transform_store.location[entity].x,
            transform_store.location[entity].y,
            0.0
    ));

    return LIBX_TRUE;
}


u8 _r3_register_component2D(void) {
    identity =  libx->mathx.mat.identity4();

    if (!libx->ecsx.register_component(R3_SPRITE2D, &sprite_store, _add_sprite_impl, _rem_sprite_impl, (COMPONENT_GET_FPTR)_get_sprite_impl)) {
        printf("failed to register R3_Sprite2D component!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_component(R3_TRANSFORM2D, &transform_store, _add_transform_impl, _rem_transform_impl, (COMPONENT_GET_FPTR)_get_transform_impl)) {
        printf("failed to register R3_Transform2D component!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_system(R3_SPRITE2D, "render2D", _sprite_system_impl)) {
        printf("failed to register R3_Sprite2D component systems!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_system(R3_TRANSFORM2D, "transform2D", _transform_system_impl)) {
        printf("failed to register R3_Transform2D component systems!\n");
        return LIBX_FALSE;
    }

    return LIBX_TRUE;
}

u8 _r3_unregister_component2D(void) {
    LIBX_FORI(0, R3_COMPONENT2D_COUNT, 1) {
        if (!libx->ecsx.unregister_systems(i)) return LIBX_FALSE;
        if (!libx->ecsx.unregister_component(i)) return LIBX_FALSE;
    }

    return LIBX_TRUE;
}
