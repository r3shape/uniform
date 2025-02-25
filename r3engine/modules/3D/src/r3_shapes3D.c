#include "../include/r3_shapes3D.h"

R3_Vertex_Data _create_quad3D_impl(R3_Vec3 size, R3_Vec3 color) {
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

    return r3_graphics_api->create_vertex_data(
        vertices, 4,
        indices, 6,
        R3_LOCATION_ATTR | R3_COLOR_ATTR | R3_TCOORD_ATTR
    );
}

R3_Vertex_Data _create_cube3D_impl(R3_Vec3 size, R3_Vec3 color) {
    float half_width  = (size.x / 100.0) / 2.0f;
    float half_height = (size.y / 100.0) / 2.0f;
    float half_depth  = (size.z / 100.0) / 2.0f;

    float vertices[] = {
        // Back face (-Z normal)
        -half_width, -half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         half_width, -half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
         half_width,  half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
        -half_width,  half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    
        // Front face (+Z normal)
        -half_width, -half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         half_width, -half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
         half_width,  half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
        -half_width,  half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    
        // Left face (-X normal)
        -half_width, -half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -half_width, -half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        -half_width,  half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
        -half_width,  half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    
        // Right face (+X normal)
         half_width, -half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         half_width, -half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
         half_width,  half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
         half_width,  half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
    
        // Bottom face (-Y normal)
        -half_width, -half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         half_width, -half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
         half_width, -half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
        -half_width, -half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    
        // Top face (+Y normal)
        -half_width,  half_height, -half_depth,  color.x, color.y, color.z,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         half_width,  half_height, -half_depth,  color.x, color.y, color.z,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
         half_width,  half_height,  half_depth,  color.x, color.y, color.z,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        -half_width,  half_height,  half_depth,  color.x, color.y, color.z,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
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

    return r3_graphics_api->create_vertex_data(
        vertices, 24,
        indices, 36,
        R3_LOCATION_ATTR | R3_COLOR_ATTR | R3_TCOORD_ATTR | R3_NORMAL_ATTR
    );
}
