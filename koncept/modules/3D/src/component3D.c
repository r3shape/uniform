#include <koncept/modules/3D/include/koncept_3D.h>

static Mat4 identity;

static struct KC_Mesh_Storage {
    Vec3 size[ENTITY_MAX];
    Vec3 color[ENTITY_MAX];                 // u_mesh_color;
    KC_Texture texture[ENTITY_MAX];
    KC_Vertex_Data vertex[ENTITY_MAX];
} mesh_store = {0};

static struct KC_Transform_Storage {
    f32 speed[ENTITY_MAX];
    Mat4 model[ENTITY_MAX];
    Vec3 scale[ENTITY_MAX];
    Vec3 rotation[ENTITY_MAX];
    Vec3 velocity[ENTITY_MAX];
    Vec3 location[ENTITY_MAX];
} transform_store = {0};

static struct KC_Material_Storage {
    f32 shine[ENTITY_MAX];
    Vec3 ambient[ENTITY_MAX];
    Vec3 diffuse[ENTITY_MAX];
    Vec3 specular[ENTITY_MAX];
    KC_Uniform uniforms[ENTITY_MAX][4];
} material_store = {0};

static struct KC_ShaderSrc_Storage {
    KC_Shader shader[ENTITY_MAX];
} shader_store = {0};


u8 _add_mesh3D_impl(u32 entity) {
    mesh_store.size[entity] = corex->mathx.vec.vec3(64, 64, 64);
    mesh_store.texture[entity] = (KC_Texture){.id = 0};
    mesh_store.color[entity] = corex->mathx.vec.vec3(84/255.0, 52/255.0, 150/255.0);
    mesh_store.vertex[entity] = koncept_3D->shape3D.cube3D(mesh_store.size[entity], mesh_store.color[entity]);
    return COREX_TRUE;
}

u8 _rem_mesh3D_impl(u32 entity) {
    mesh_store.size[entity] = (Vec3){0};
    mesh_store.color[entity] = (Vec3){0};
    mesh_store.texture[entity] = (KC_Texture){0};
    mesh_store.vertex[entity] = (KC_Vertex_Data){0};
    return COREX_TRUE;
}

u8 _get_mesh3D_impl(u32 entity, KC_Mesh3D* component) {
    component->size = &mesh_store.size[entity];
    component->color = &mesh_store.color[entity];
    component->texture = &mesh_store.texture[entity];
    component->vertex = &mesh_store.vertex[entity];
    return COREX_TRUE;
}

u8 _mesh3D_system_impl(u32 entity) {
    koncept_core->graphics.push_pipeline(&(KC_Render_Call){
        .mode = KC_TRIANGLE_MODE,
        .vertex = &mesh_store.vertex[entity],
        .type = (mesh_store.vertex[entity].indexCount) ? KC_RENDER_ELEMENTS : KC_RENDER_ARRAYS,
        .model = (corex->ecsx.has_component(KC_TRANSFORM3D, entity)) ? &transform_store.model[entity] : &identity,
        .shader = (corex->ecsx.has_component(KC_SHADER3D, entity) && shader_store.shader[entity].program) ? &shader_store.shader[entity] : NULL,
        .texture = (mesh_store.texture[entity].id) ? &mesh_store.texture[entity] : NULL,
        .uniform_count = 1, .uniforms = {
            (KC_Uniform){
                .name = "u_mesh_color",
                .type = KC_UNIFORM_VEC3,
                .value = &mesh_store.color[entity]
            }
        }
    });

    return COREX_TRUE;
}


u8 _add_transform3D_impl(u32 entity) {
    transform_store.speed[entity] = 10.0;
    transform_store.model[entity] = corex->mathx.mat.identity4();
    transform_store.scale[entity] = corex->mathx.vec.vec3(1, 1, 1);
    transform_store.rotation[entity] = corex->mathx.vec.vec3(0, 0, 0);
    transform_store.velocity[entity] = corex->mathx.vec.vec3(0, 0, 0);
    transform_store.location[entity] = corex->mathx.vec.vec3(0, 0, 0);
    return COREX_TRUE;
}

u8 _rem_transform3D_impl(u32 entity) {
    transform_store.speed[entity] = 0.0;
    transform_store.model[entity] = (Mat4){0};
    transform_store.scale[entity] = (Vec3){0, 0, 0};
    transform_store.rotation[entity] = (Vec3){0, 0, 0};
    transform_store.velocity[entity] = (Vec3){0, 0, 0};
    transform_store.location[entity] = (Vec3){0, 0, 0};
    return COREX_TRUE;
}

u8 _get_transform3D_impl(u32 entity, KC_Transform3D* component) {
    component->speed = &transform_store.speed[entity];
    component->model = &transform_store.model[entity];
    component->scale = &transform_store.scale[entity];
    component->rotation = &transform_store.rotation[entity];
    component->velocity = &transform_store.velocity[entity];
    component->location = &transform_store.location[entity];
    return COREX_TRUE;
}

u8 _transform3D_system_impl(u32 entity) {
    transform_store.velocity[entity] = corex->mathx.vec.scale3(
        transform_store.velocity[entity],
        transform_store.speed[entity]
    );

    transform_store.location[entity] = corex->mathx.vec.add3(
        transform_store.location[entity],
        transform_store.velocity[entity]
    );

    // model scaling
    transform_store.model[entity] = corex->mathx.mat.identity4();
    transform_store.model[entity] = corex->mathx.mat.mult4(
        transform_store.model[entity],
        corex->mathx.mat.scale4(
            transform_store.scale[entity].x,
            transform_store.scale[entity].y,
            transform_store.scale[entity].z
    ));

    // model rotation
    transform_store.model[entity] = corex->mathx.mat.mult4(
        transform_store.model[entity],
        corex->mathx.mat.rotx4(transform_store.rotation[entity].x)
    );
    transform_store.model[entity] = corex->mathx.mat.mult4(
        transform_store.model[entity],
        corex->mathx.mat.roty4(transform_store.rotation[entity].y)
    );
    transform_store.model[entity] = corex->mathx.mat.mult4(
        transform_store.model[entity],
        corex->mathx.mat.rotz4(transform_store.rotation[entity].z)
    );
    
    // model translation
    transform_store.model[entity] = corex->mathx.mat.mult4(
        transform_store.model[entity],
        corex->mathx.mat.trans4(
            transform_store.location[entity].x,
            transform_store.location[entity].y,
            transform_store.location[entity].z
    ));

    return COREX_TRUE;
}


u8 _add_material3D_impl(u32 entity) {
    material_store.shine[entity] = 32.0;
    material_store.ambient[entity] = corex->mathx.vec.vec3(1.0, 0.5, 0.31);
    material_store.diffuse[entity] = corex->mathx.vec.vec3(1.0, 0.5, 0.31);
    material_store.specular[entity] = corex->mathx.vec.vec3(0.5, 0.5, 0.5);
    material_store.uniforms[entity][0] = (KC_Uniform){.type = KC_UNIFORM_FLOAT,.name =  "u_material.shine",   .value = &material_store.shine[entity]};
    material_store.uniforms[entity][1] = (KC_Uniform){.type = KC_UNIFORM_VEC3, .name =  "u_material.ambient", .value = &material_store.ambient[entity]};
    material_store.uniforms[entity][2] = (KC_Uniform){.type = KC_UNIFORM_VEC3, .name =  "u_material.diffuse", .value = &material_store.diffuse[entity]};
    material_store.uniforms[entity][3] = (KC_Uniform){.type = KC_UNIFORM_VEC3, .name =  "u_material.specular",.value = &material_store.specular[entity]};
    return COREX_TRUE;
}

u8 _rem_material3D_impl(u32 entity) {
    material_store.shine[entity] = 0.0;
    material_store.ambient[entity] = (Vec3){0, 0, 0};
    material_store.diffuse[entity] = (Vec3){0, 0, 0};
    material_store.specular[entity] = (Vec3){0, 0, 0};
    return COREX_TRUE;
}

u8 _get_material3D_impl(u32 entity, KC_Material3D* component) {
    component->shine = &material_store.shine[entity];
    component->ambient = &material_store.ambient[entity];
    component->diffuse = &material_store.diffuse[entity];
    component->specular = &material_store.specular[entity];
    return COREX_TRUE;
}

u8 _material3D_system_impl(u32 entity) {
    KC_Shader* shader = (corex->ecsx.has_component(KC_SHADER3D, entity)) ? &shader_store.shader[entity] : koncept_core->graphics.pipeline.shader;
    koncept_core->graphics.set_uniform(shader, &material_store.uniforms[entity][0]); // u_material.shine
    koncept_core->graphics.set_uniform(shader, &material_store.uniforms[entity][1]); // u_material.ambient
    koncept_core->graphics.set_uniform(shader, &material_store.uniforms[entity][2]); // u_material.diffuse
    koncept_core->graphics.set_uniform(shader, &material_store.uniforms[entity][3]); // u_material.specular
    return COREX_TRUE;
}


u8 _add_shadekonceptD_impl(u32 entity) {
    shader_store.shader[entity] = (KC_Shader){0};
}

u8 _set_shadekonceptD_impl(u32 entity, KC_Shader shader) {
    if (!corex->ecsx.has_component(KC_SHADER3D, entity)) return COREX_FALSE;   // error: entity does not have component!
    if (!shader.program) return COREX_FALSE; // error: invalid shader passed!
    shader_store.shader[entity] = shader;
    return COREX_TRUE;
}

u8 _rem_shadekonceptD_impl(u32 entity) {
    koncept_core->graphics.destroy_shader(&shader_store.shader[entity]);
    shader_store.shader[entity] = (KC_Shader){0};
}

u8 _get_shadekonceptD_impl(u32 entity, KC_Shader* component) {
    component = &shader_store.shader[entity];
    return COREX_TRUE;
}


u8 _koncept_register_component3D(_koncept_3D_api* koncept_3D) {
    if (!koncept_3D) return COREX_FALSE;    // error: null ptr!

    identity =  corex->mathx.mat.identity4();

    if (!corex->ecsx.register_component(KC_MESH3D, &mesh_store, _add_mesh3D_impl, _rem_mesh3D_impl, (COMPONENT_GET_FPTR)_get_mesh3D_impl)) {
        printf("failed to register KC_Mesh3D component!\n");
        return COREX_FALSE;
    }
    
    if (!corex->ecsx.register_component(KC_TRANSFORM3D, &transform_store, _add_transform3D_impl, _rem_transform3D_impl, (COMPONENT_GET_FPTR)_get_transform3D_impl)) {
        printf("failed to register KC_Transform3D component!\n");
        return COREX_FALSE;
    }
    
    if (!corex->ecsx.register_component(KC_MATERIAL3D, &material_store, _add_material3D_impl, _rem_material3D_impl, (COMPONENT_GET_FPTR)_get_material3D_impl)) {
        printf("failed to register KC_Material3D component!\n");
        return COREX_FALSE;
    }
    
    if (!corex->ecsx.register_component(KC_SHADER3D, &shader_store, _add_shadekonceptD_impl, _rem_shadekonceptD_impl, (COMPONENT_GET_FPTR)_get_shadekonceptD_impl)) {
        printf("failed to register KC_Shader component!\n");
        return COREX_FALSE;
    }
    
    if (!corex->ecsx.register_system(KC_MESH3D, "rendekonceptD", _mesh3D_system_impl)) {
        printf("failed to register KC_Mesh3D component systems!\n");
        return COREX_FALSE;
    }
    
    if (!corex->ecsx.register_system(KC_TRANSFORM3D, "transform3D", _transform3D_system_impl)) {
        printf("failed to register KC_Transform3D component systems!\n");
        return COREX_FALSE;
    }
    
    if (!corex->ecsx.register_system(KC_MATERIAL3D, "material3D", _material3D_system_impl)) {
        printf("failed to register KC_Material3D component systems!\n");
        return COREX_FALSE;
    }

    koncept_3D->set_shadekonceptD = _set_shadekonceptD_impl;

    return COREX_TRUE;
}

u8 _koncept_unregister_component3D(void) {
    COREX_FORI(0, KC_COMPONENT3D_COUNT, 1) {
        if (!corex->ecsx.unregister_systems(i)) return COREX_FALSE;
        if (!corex->ecsx.unregister_component(i)) return COREX_FALSE;
    }

    return COREX_TRUE;
}
