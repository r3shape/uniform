#include "../include/lotus_shapes3D.h"

Lotus_Vertex_Data _create_cube3D_impl(Lotus_Vec3 size, Lotus_Vec3 color) {
    float half_width  = (size.x / 100.0) / 2.0f;
    float half_height = (size.y / 100.0) / 2.0f;
    float half_depth  = (size.z / 100.0) / 2.0f;

    float vertices[] = {
        -half_width, -half_height, -half_depth,     color.x, color.y, color.z,     0.0f, 0.0f,
         half_width, -half_height, -half_depth,     color.x, color.y, color.z,     1.0f, 0.0f,
         half_width,  half_height, -half_depth,     color.x, color.y, color.z,     1.0f, 1.0f,
        -half_width,  half_height, -half_depth,     color.x, color.y, color.z,     0.0f, 1.0f,
        -half_width, -half_height,  half_depth,     color.x, color.y, color.z,     0.0f, 0.0f,
         half_width, -half_height,  half_depth,     color.x, color.y, color.z,     1.0f, 0.0f,
         half_width,  half_height,  half_depth,     color.x, color.y, color.z,     1.0f, 1.0f,
        -half_width,  half_height,  half_depth,     color.x, color.y, color.z,     0.0f, 1.0f
    };

    unsigned int indices[] = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 3, 7, 7, 4, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Bottom face
        0, 1, 5, 5, 4, 0,
        // Top face
        3, 2, 6, 6, 7, 3
    };

    return lotus_graphics_api->create_vertex_data(
        vertices, 8,
        indices,  36,
        LOTUS_LOCATION_ATTR | LOTUS_COLOR_ATTR | LOTUS_TCOORD_ATTR
    );
}
