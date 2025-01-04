#include "../include/components/lotus_mesh.h"

#include <stdlib.h>
#include "../include/components/lotus_component.h"

#include "../../lotus.CORE/include/utility/lotus_array.h"

#include "../../lotus.CORE/include/graphics/lotus_gl.h"

static Lotus_Graphics_API* internal_graphics_api = NULL;

ubyte _lotus_init_mesh_data(Lotus_Mesh_Data* data, void* graphics_api) {
    if (!data || !graphics_api) return LOTUS_FALSE;
    internal_graphics_api = (Lotus_Graphics_API*)graphics_api;

    data->attrs = (ubyte*)lotus_make_array(sizeof(ubyte), LOTUS_ENTITY_MAX);
    data->vbo = (ubyte4*)lotus_make_array(sizeof(ubyte4), LOTUS_ENTITY_MAX);
    data->ebo = (ubyte4*)lotus_make_array(sizeof(ubyte4), LOTUS_ENTITY_MAX);
    data->vao = (ubyte4*)lotus_make_array(sizeof(ubyte4), LOTUS_ENTITY_MAX);
    data->index_count = (ubyte4*)lotus_make_array(sizeof(ubyte4), LOTUS_ENTITY_MAX);
    data->vertex_count = (ubyte4*)lotus_make_array(sizeof(ubyte4), LOTUS_ENTITY_MAX);
    data->vertices = (f32**)lotus_make_array(sizeof(f32*), LOTUS_ENTITY_MAX);
    data->indices = (ubyte4**)lotus_make_array(sizeof(ubyte4*), LOTUS_ENTITY_MAX);

    return LOTUS_TRUE;
}

void _lotus_destroy_mesh_data(Lotus_Mesh_Data* data) {
    lotus_destroy_array(data->attrs);
    lotus_destroy_array(data->vbo);
    lotus_destroy_array(data->ebo);
    lotus_destroy_array(data->vao);
    lotus_destroy_array(data->vertex_count);
    lotus_destroy_array(data->index_count);
    lotus_destroy_array(data->vertices);
    lotus_destroy_array(data->indices);
    data->attrs = NULL;
    data->vbo = NULL;
    data->ebo = NULL;
    data->vao = NULL;
    data->vertex_count = NULL;
    data->index_count = NULL;
    data->vertices = NULL;
    data->indices = NULL;
}

ubyte _lotus_validate_mesh_data(Lotus_Mesh_Data* data) {
    return (
        data->attrs == NULL         ||
        data->vbo == NULL           ||
        data->ebo == NULL           ||
        data->vao == NULL           ||
        data->vertex_count == NULL    ||
        data->index_count == NULL     ||
        data->vertices == NULL      ||
        data->indices  == NULL
    ) ? LOTUS_FALSE : LOTUS_TRUE;
}

void _increment_mesh_data_arrays(Lotus_Mesh_Data* data, ubyte4 count) {
    Lotus_Mesh_Data* mesh_data = (Lotus_Mesh_Data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return;

    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->attrs, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->vbo, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->ebo, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->vao, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->vertex_count, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->index_count, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->vertices, LOTUS_ARRAY_LENGTH_FIELD, count);
    LOTUS_ARRAY_SET_HEADER_FIELD(mesh_data->indices, LOTUS_ARRAY_LENGTH_FIELD, count);
}

void _lotus_add_mesh(void* data, Lotus_Entity entity) {
    Lotus_Mesh_Data* mesh_data = (Lotus_Mesh_Data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return;
    ubyte4 count = LOTUS_ARRAY_GET_HEADER_FIELD(mesh_data->attrs, LOTUS_ARRAY_LENGTH_FIELD);
    _increment_mesh_data_arrays(mesh_data, count+1);

    mesh_data->attrs[entity] = 0;
    
    mesh_data->vbo[entity] = 0;
    mesh_data->ebo[entity] = 0;
    mesh_data->vao[entity] = 0;
    
    mesh_data->index_count[entity] = 0;
    mesh_data->vertex_count[entity] = 0;
    mesh_data->vertices[entity] = NULL;
    mesh_data->indices[entity] = NULL;
}

void _lotus_rem_mesh(void* data, Lotus_Entity entity) {
    Lotus_Mesh_Data* mesh_data = (Lotus_Mesh_Data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return;
    ubyte4 count = LOTUS_ARRAY_GET_HEADER_FIELD(mesh_data->attrs, LOTUS_ARRAY_LENGTH_FIELD);
    _increment_mesh_data_arrays(mesh_data, count-1);
    
    Lotus_Vertex_Data vertex_data = {
        .attrs = mesh_data->attrs[entity],
        .vbo = mesh_data->vbo[entity],
        .ebo = mesh_data->ebo[entity],
        .vao = mesh_data->vao[entity],
        .vertices = mesh_data->vertices[entity],
        .vertex_count = mesh_data->vertex_count[entity],
        .indices = mesh_data->indices[entity],
        .index_count = mesh_data->index_count[entity]
    };
    internal_graphics_api->destroy_vertex_data(&vertex_data);
    
    mesh_data->attrs[entity] = 0;
    mesh_data->vbo[entity] = 0;
    mesh_data->ebo[entity] = 0;
    mesh_data->vao[entity] = 0;
    mesh_data->vertices[entity] = NULL;
    mesh_data->vertex_count[entity] = 0;
    mesh_data->indices[entity] = NULL;
    mesh_data->index_count[entity] = 0;
}

void _lotus_set_mesh(void* data, Lotus_Component component, Lotus_Entity entity) {
    Lotus_Mesh_Data* mesh_data = (Lotus_Mesh_Data*)data;
    if (!_lotus_validate_mesh_data(mesh_data) || component.type != LOTUS_MESH_COMPONENT) return;

    if (component.type == LOTUS_MESH_COMPONENT) {
        Lotus_Vertex_Data data = internal_graphics_api->make_vertex_data(
            component.data.mesh.vertices,
            component.data.mesh.vertex_count,
            component.data.mesh.indices,
            component.data.mesh.index_count,
            component.data.mesh.attrs
        );

        mesh_data->attrs[entity] = data.attrs;
        mesh_data->vbo[entity] = data.vbo;
        mesh_data->ebo[entity] = data.ebo;
        mesh_data->vao[entity] = data.vao;
        mesh_data->vertex_count[entity] = data.vertex_count;
        mesh_data->vertices[entity] = data.vertices;
        mesh_data->index_count[entity] = data.index_count;
        mesh_data->indices[entity] = data.indices;

    } else { return; }
}

Lotus_Component _lotus_get_mesh(void* data, Lotus_Entity entity) {
    Lotus_Mesh_Data* mesh_data = (Lotus_Mesh_Data*)data;
    if (!_lotus_validate_mesh_data(mesh_data)) return (Lotus_Component){ .type = LOTUS_COMPONENT_MAX+1 };
    
    return (Lotus_Component){
        .type = LOTUS_MESH_COMPONENT,
        .data.mesh.attrs = mesh_data->attrs[entity],
        .data.mesh.vbo = mesh_data->vbo[entity],
        .data.mesh.ebo = mesh_data->ebo[entity],
        .data.mesh.vao = mesh_data->vao[entity],
        .data.mesh.index_count = mesh_data->index_count[entity],
        .data.mesh.vertex_count = mesh_data->vertex_count[entity],
        .data.mesh.vertices = mesh_data->vertices[entity],
        .data.mesh.indices = mesh_data->indices[entity]
    };
}
