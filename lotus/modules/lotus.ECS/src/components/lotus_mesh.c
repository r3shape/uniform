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
    
    mesh_data->attrs[entity] = 0;
    
    internal_graphics_api->GL_API.delete_buffers(1, &mesh_data->vbo[entity]);
    mesh_data->vbo[entity] = 0;
    
    internal_graphics_api->GL_API.delete_buffers(1, &mesh_data->ebo[entity]);
    mesh_data->ebo[entity] = 0;
    
    internal_graphics_api->GL_API.delete_vertex_arrays(1, &mesh_data->vao[entity]);
    mesh_data->vao[entity] = 0;
    
    mesh_data->index_count[entity] = 0;
    mesh_data->vertex_count[entity] = 0;
    mesh_data->vertices[entity] = NULL;
    mesh_data->indices[entity] = NULL;
}

void _lotus_set_mesh(void* data, Lotus_Component component, Lotus_Entity entity) {
    Lotus_Mesh_Data* mesh_data = (Lotus_Mesh_Data*)data;
    if (!_lotus_validate_mesh_data(mesh_data) || component.type != LOTUS_MESH_COMPONENT) return;

    if (component.type == LOTUS_MESH_COMPONENT) {
        if ((component.data.mesh.attrs & ~((1 << LOTUS_VERTEX_ATTRIBS) - 1)) != 0 || !component.data.mesh.vertices) {
            return;
        }

        // calculate stride and offsets dynamically
        ubyte4 stride = 0;
        ubyte4 offsets[LOTUS_VERTEX_ATTRIBS] = {0};
        const ubyte4 attr_sizes[LOTUS_VERTEX_ATTRIBS] = {
            3, // LOTUS_LOCATION_ATTR: vec3
            3, // LOTUS_COLOR_ATTR: vec3
            2, // LOTUS_TCOORD_ATTR: vec2
            2  // LOTUS_NORMAL_ATTR: vec2
        };

        for (int i = 0; i < LOTUS_VERTEX_ATTRIBS; i++) {
            if ((component.data.mesh.attrs & (1 << i)) != 0) {
                // accumulate stride for enabled vertex attributes
                offsets[i] = stride;
                stride += attr_sizes[i];
            }
        }

        internal_graphics_api->GL_API.gen_vertex_arrays(1, &mesh_data->vao[entity]);
        internal_graphics_api->GL_API.gen_buffers(1, &mesh_data->vbo[entity]);

        internal_graphics_api->GL_API.bind_vertex_array(mesh_data->vao[entity]);
        internal_graphics_api->GL_API.bind_buffer(LOTUS_ARRAY_BUFFER, mesh_data->vbo[entity]);
        
        size_t vertex_data_size = component.data.mesh.vertex_count * (stride * sizeof(f32));
        internal_graphics_api->GL_API.buffer_data(LOTUS_ARRAY_BUFFER, vertex_data_size, component.data.mesh.vertices, LOTUS_STATIC_DRAW);

        // generate EBO if indices are provided
        if (component.data.mesh.index_count > 0 && component.data.mesh.indices) {
            internal_graphics_api->GL_API.gen_buffers(1, &mesh_data->ebo[entity]);

            internal_graphics_api->GL_API.bind_buffer(LOTUS_ELEMENT_ARRAY_BUFFER, mesh_data->ebo[entity]);
            
            size_t index_data_size = component.data.mesh.index_count * sizeof(ubyte4);
            internal_graphics_api->GL_API.buffer_data(LOTUS_ELEMENT_ARRAY_BUFFER, index_data_size, component.data.mesh.indices, LOTUS_STATIC_DRAW);
            
            mesh_data->indices[entity] = component.data.mesh.indices;
            mesh_data->index_count[entity] = component.data.mesh.index_count;
        } else {
            mesh_data->ebo[entity] = 0;
            mesh_data->indices[entity] = NULL;
            mesh_data->index_count[entity] = 0;
        }

        // configure vertex attributes dynamically
        for (int i = 0; i < LOTUS_VERTEX_ATTRIBS; i++) {
            if ((component.data.mesh.attrs & (1 << i)) != 0) {
                internal_graphics_api->GL_API.vertex_attrib_pointer(
                    i, 
                    attr_sizes[i], 
                    GL_FLOAT, 
                    GL_FALSE, 
                    stride * sizeof(f32), 
                    (void*)(offsets[i] * sizeof(f32))
                );
                internal_graphics_api->GL_API.enable_vertex_attrib_array(i);
            }
        }

        internal_graphics_api->GL_API.bind_buffer(LOTUS_ARRAY_BUFFER, 0);
        internal_graphics_api->GL_API.bind_vertex_array(0);

        mesh_data->attrs[entity] = component.data.mesh.attrs;
        mesh_data->vertex_count[entity] = component.data.mesh.vertex_count;
        mesh_data->vertices[entity] = component.data.mesh.vertices;

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
