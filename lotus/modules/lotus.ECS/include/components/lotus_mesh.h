#pragma once

#include "../lotus_ecs_types.h"

typedef struct Lotus_Mesh {
    ubyte attrs;
    ubyte4 vbo;
    ubyte4 ebo;
    ubyte4 vao;
    ubyte4 vertex_count;
    ubyte4 index_count;
    f32* vertices;
    ubyte4* indices;
} Lotus_Mesh;

typedef struct Lotus_Mesh_Data {
    ubyte* attrs;
    ubyte4* vbo;
    ubyte4* ebo;
    ubyte4* vao;
    ubyte4* vertex_count;
    ubyte4* index_count;
    f32** vertices;   // raw floating-point vertex data
    ubyte4** indices;
} Lotus_Mesh_Data;

ubyte _lotus_init_mesh_data(Lotus_Mesh_Data* data, void* graphics_api);
void _lotus_destroy_mesh_data(Lotus_Mesh_Data* data);

void _lotus_add_mesh(void* data, Lotus_Entity entity);
void _lotus_rem_mesh(void* data, Lotus_Entity entity);
void _lotus_set_mesh(void* data, Lotus_Component component, Lotus_Entity entity);
Lotus_Component _lotus_get_mesh(void* data, Lotus_Entity entity);
