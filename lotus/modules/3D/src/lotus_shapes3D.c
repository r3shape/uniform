#include "../include/lotus_shapes3D.h"

Lotus_Vertex_Data _create_quad3D_impl(Lotus_Vec3 size, Lotus_Vec3 color) {
    float half_width  = (size.x / 100.0) / 2.0f;
    float half_height = (size.y / 100.0) / 2.0f;

    float vertices[] = {
        -half_width, -half_height, 0.0f, color.x, color.y, color.z, 0.0f, 0.0f,
         half_width, -half_height, 0.0f, color.x, color.y, color.z, 1.0f, 0.0f,
         half_width,  half_height, 0.0f, color.x, color.y, color.z, 1.0f, 1.0f,
        -half_width,  half_height, 0.0f, color.x, color.y, color.z, 0.0f, 1.0f,
    };

    ubyte4 indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    return lotus_graphics_api->create_vertex_data(
        vertices, 4,
        indices, 6,
        LOTUS_LOCATION_ATTR | LOTUS_COLOR_ATTR | LOTUS_TCOORD_ATTR
    );
}

Lotus_Vertex_Data _create_cube3D_impl(Lotus_Vec3 size, Lotus_Vec3 color) {
    float half_width  = (size.x / 100.0) / 2.0f;
    float half_height = (size.y / 100.0) / 2.0f;
    float half_depth  = (size.z / 100.0) / 2.0f;

    float vertices[] = {
        // Back face
        -half_width, -half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,
         half_width, -half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,
         half_width,  half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,
        -half_width,  half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,
    
        // Front face
        -half_width, -half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,
         half_width, -half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,
         half_width,  half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,
        -half_width,  half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,
    
        // Left face
        -half_width, -half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,
        -half_width, -half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,
        -half_width,  half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,
        -half_width,  half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,
    
        // Right face
         half_width, -half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,
         half_width, -half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,
         half_width,  half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,
         half_width,  half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,
    
        // Bottom face
        -half_width, -half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,
         half_width, -half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,
         half_width, -half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,
        -half_width, -half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,
    
        // Top face
        -half_width,  half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,
         half_width,  half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,
         half_width,  half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,
        -half_width,  half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,
    };

    unsigned int indices[] = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Bottom face
        16, 17, 18, 18, 19, 16,
        // Top face
        20, 21, 22, 22, 23, 20
    };

    return lotus_graphics_api->create_vertex_data(
        vertices, 24,
        indices, 36,
        LOTUS_LOCATION_ATTR | LOTUS_COLOR_ATTR | LOTUS_TCOORD_ATTR
    );
}
