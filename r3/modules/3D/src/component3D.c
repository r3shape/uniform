#include <r3/modules/3D/include/r3.3D.h>

static Mat4 identity;

static struct R3_Mesh_Storage {
    Vec3 size[ENTITY_MAX];
    Vec3 color[ENTITY_MAX];                 // u_mesh_color;
    R3_Texture texture[ENTITY_MAX];
    R3_Vertex_Data vertex[ENTITY_MAX];
} mesh_store = {0};

static struct R3_Transform_Storage {
    f32 speed[ENTITY_MAX];
    Mat4 model[ENTITY_MAX];
    Vec3 scale[ENTITY_MAX];
    Vec3 rotation[ENTITY_MAX];
    Vec3 velocity[ENTITY_MAX];
    Vec3 location[ENTITY_MAX];
} transform_store = {0};

static struct R3_Material_Storage {
    f32 shine[ENTITY_MAX];
    Vec3 ambient[ENTITY_MAX];
    Vec3 diffuse[ENTITY_MAX];
    Vec3 specular[ENTITY_MAX];
    R3_Uniform uniforms[ENTITY_MAX][4];
} material_store = {0};

static struct R3_ShaderSrc_Storage {
    R3_Shader shader[ENTITY_MAX];
} shader_store = {0};


u8 _add_mesh3D_impl(u32 entity) {
    mesh_store.size[entity] = libx->mathx.vec.vec3(64, 64, 64);
    mesh_store.texture[entity] = (R3_Texture){.id = 0};
    mesh_store.color[entity] = libx->mathx.vec.vec3(84/255.0, 52/255.0, 150/255.0);
    mesh_store.vertex[entity] = r3_3D->shape3D.cube3D(mesh_store.size[entity], mesh_store.color[entity]);
    return LIBX_TRUE;
}

u8 _rem_mesh3D_impl(u32 entity) {
    mesh_store.size[entity] = (Vec3){0};
    mesh_store.color[entity] = (Vec3){0};
    mesh_store.texture[entity] = (R3_Texture){0};
    mesh_store.vertex[entity] = (R3_Vertex_Data){0};
    return LIBX_TRUE;
}

u8 _get_mesh3D_impl(u32 entity, R3_Mesh3D* component) {
    component->size = &mesh_store.size[entity];
    component->color = &mesh_store.color[entity];
    component->texture = &mesh_store.texture[entity];
    component->vertex = &mesh_store.vertex[entity];
    return LIBX_TRUE;
}

u8 _mesh3D_system_impl(u32 entity) {
    r3_core->graphics.push_pipeline(&(R3_Render_Call){
        .mode = R3_TRIANGLE_MODE,
        .vertex = &mesh_store.vertex[entity],
        .type = (mesh_store.vertex[entity].indexCount) ? R3_RENDER_ELEMENTS : R3_RENDER_ARRAYS,
        .model = (libx->ecsx.has_component(R3_TRANSFORM3D, entity)) ? &transform_store.model[entity] : &identity,
        .shader = (libx->ecsx.has_component(R3_SHADER3D, entity) && shader_store.shader[entity].program) ? &shader_store.shader[entity] : NULL,
        .texture = (mesh_store.texture[entity].id) ? &mesh_store.texture[entity] : NULL,
        .uniform_count = 1, .uniforms = {
            (R3_Uniform){
                .name = "u_mesh_color",
                .type = R3_UNIFORM_VEC3,
                .value = &mesh_store.color[entity]
            }
        }
    });

    return LIBX_TRUE;
}


u8 _add_transform3D_impl(u32 entity) {
    transform_store.speed[entity] = 10.0;
    transform_store.model[entity] = libx->mathx.mat.identity4();
    transform_store.scale[entity] = libx->mathx.vec.vec3(1, 1, 1);
    transform_store.rotation[entity] = libx->mathx.vec.vec3(0, 0, 0);
    transform_store.velocity[entity] = libx->mathx.vec.vec3(0, 0, 0);
    transform_store.location[entity] = libx->mathx.vec.vec3(0, 0, 0);
    return LIBX_TRUE;
}

u8 _rem_transform3D_impl(u32 entity) {
    transform_store.speed[entity] = 0.0;
    transform_store.model[entity] = (Mat4){0};
    transform_store.scale[entity] = (Vec3){0, 0, 0};
    transform_store.rotation[entity] = (Vec3){0, 0, 0};
    transform_store.velocity[entity] = (Vec3){0, 0, 0};
    transform_store.location[entity] = (Vec3){0, 0, 0};
    return LIBX_TRUE;
}

u8 _get_transform3D_impl(u32 entity, R3_Transform3D* component) {
    component->speed = &transform_store.speed[entity];
    component->model = &transform_store.model[entity];
    component->scale = &transform_store.scale[entity];
    component->rotation = &transform_store.rotation[entity];
    component->velocity = &transform_store.velocity[entity];
    component->location = &transform_store.location[entity];
    return LIBX_TRUE;
}

u8 _transform3D_system_impl(u32 entity) {
    transform_store.velocity[entity] = libx->mathx.vec.scale3(
        transform_store.velocity[entity],
        transform_store.speed[entity]
    );

    transform_store.location[entity] = libx->mathx.vec.add3(
        transform_store.location[entity],
        transform_store.velocity[entity]
    );

    // model scaling
    transform_store.model[entity] = libx->mathx.mat.identity4();
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.scale4(
            transform_store.scale[entity].x,
            transform_store.scale[entity].y,
            transform_store.scale[entity].z
    ));

    // model rotation
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.rotx4(transform_store.rotation[entity].x)
    );
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.roty4(transform_store.rotation[entity].y)
    );
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.rotz4(transform_store.rotation[entity].z)
    );
    
    // model translation
    transform_store.model[entity] = libx->mathx.mat.mult4(
        transform_store.model[entity],
        libx->mathx.mat.trans4(
            transform_store.location[entity].x,
            transform_store.location[entity].y,
            transform_store.location[entity].z
    ));

    return LIBX_TRUE;
}


u8 _add_material3D_impl(u32 entity) {
    material_store.shine[entity] = 32.0;
    material_store.ambient[entity] = libx->mathx.vec.vec3(1.0, 0.5, 0.31);
    material_store.diffuse[entity] = libx->mathx.vec.vec3(1.0, 0.5, 0.31);
    material_store.specular[entity] = libx->mathx.vec.vec3(0.5, 0.5, 0.5);
    material_store.uniforms[entity][0] = (R3_Uniform){.type = R3_UNIFORM_FLOAT,.name =  "u_material.shine",   .value = &material_store.shine[entity]};
    material_store.uniforms[entity][1] = (R3_Uniform){.type = R3_UNIFORM_VEC3, .name =  "u_material.ambient", .value = &material_store.ambient[entity]};
    material_store.uniforms[entity][2] = (R3_Uniform){.type = R3_UNIFORM_VEC3, .name =  "u_material.diffuse", .value = &material_store.diffuse[entity]};
    material_store.uniforms[entity][3] = (R3_Uniform){.type = R3_UNIFORM_VEC3, .name =  "u_material.specular",.value = &material_store.specular[entity]};
    return LIBX_TRUE;
}

u8 _rem_material3D_impl(u32 entity) {
    material_store.shine[entity] = 0.0;
    material_store.ambient[entity] = (Vec3){0, 0, 0};
    material_store.diffuse[entity] = (Vec3){0, 0, 0};
    material_store.specular[entity] = (Vec3){0, 0, 0};
    return LIBX_TRUE;
}

u8 _get_material3D_impl(u32 entity, R3_Material3D* component) {
    component->shine = &material_store.shine[entity];
    component->ambient = &material_store.ambient[entity];
    component->diffuse = &material_store.diffuse[entity];
    component->specular = &material_store.specular[entity];
    return LIBX_TRUE;
}

u8 _material3D_system_impl(u32 entity) {
    R3_Shader* shader = (libx->ecsx.has_component(R3_SHADER3D, entity)) ? &shader_store.shader[entity] : r3_core->graphics.pipeline.shader;
    r3_core->graphics.set_uniform(shader, &material_store.uniforms[entity][0]); // u_material.shine
    r3_core->graphics.set_uniform(shader, &material_store.uniforms[entity][1]); // u_material.ambient
    r3_core->graphics.set_uniform(shader, &material_store.uniforms[entity][2]); // u_material.diffuse
    r3_core->graphics.set_uniform(shader, &material_store.uniforms[entity][3]); // u_material.specular
    return LIBX_TRUE;
}


u8 _add_shader3D_impl(u32 entity) {
    shader_store.shader[entity] = (R3_Shader){0};
}

u8 _set_shader3D_impl(u32 entity, R3_Shader shader) {
    if (!libx->ecsx.has_component(R3_SHADER3D, entity)) return LIBX_FALSE;   // error: entity does not have component!
    if (!shader.program) return LIBX_FALSE; // error: invalid shader passed!
    shader_store.shader[entity] = shader;
    return LIBX_TRUE;
}

u8 _rem_shader3D_impl(u32 entity) {
    r3_core->graphics.destroy_shader(&shader_store.shader[entity]);
    shader_store.shader[entity] = (R3_Shader){0};
}

u8 _get_shader3D_impl(u32 entity, R3_Shader* component) {
    component = &shader_store.shader[entity];
    return LIBX_TRUE;
}


u8 _r3_register_component3D(_r3_3D_api* r3_3D) {
    if (!r3_3D) return LIBX_FALSE;    // error: null ptr!

    identity =  libx->mathx.mat.identity4();

    if (!libx->ecsx.register_component(R3_MESH3D, &mesh_store, _add_mesh3D_impl, _rem_mesh3D_impl, (COMPONENT_GET_FPTR)_get_mesh3D_impl)) {
        printf("failed to register R3_Mesh3D component!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_component(R3_TRANSFORM3D, &transform_store, _add_transform3D_impl, _rem_transform3D_impl, (COMPONENT_GET_FPTR)_get_transform3D_impl)) {
        printf("failed to register R3_Transform3D component!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_component(R3_MATERIAL3D, &material_store, _add_material3D_impl, _rem_material3D_impl, (COMPONENT_GET_FPTR)_get_material3D_impl)) {
        printf("failed to register R3_Material3D component!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_component(R3_SHADER3D, &shader_store, _add_shader3D_impl, _rem_shader3D_impl, (COMPONENT_GET_FPTR)_get_shader3D_impl)) {
        printf("failed to register R3_Shader component!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_system(R3_MESH3D, "render3D", _mesh3D_system_impl)) {
        printf("failed to register R3_Mesh3D component systems!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_system(R3_TRANSFORM3D, "transform3D", _transform3D_system_impl)) {
        printf("failed to register R3_Transform3D component systems!\n");
        return LIBX_FALSE;
    }
    
    if (!libx->ecsx.register_system(R3_MATERIAL3D, "material3D", _material3D_system_impl)) {
        printf("failed to register R3_Material3D component systems!\n");
        return LIBX_FALSE;
    }

    r3_3D->set_shader3D = _set_shader3D_impl;

    return LIBX_TRUE;
}

u8 _r3_unregister_component3D(void) {
    LIBX_FORI(0, R3_COMPONENT3D_COUNT, 1) {
        if (!libx->ecsx.unregister_systems(i)) return LIBX_FALSE;
        if (!libx->ecsx.unregister_component(i)) return LIBX_FALSE;
    }

    return LIBX_TRUE;
}
