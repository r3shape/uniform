#include <koncept/modules/3D/include/koncept_3D.h>

KC_Vertex_Data _cube3D_impl(Vec3 size, Vec3 color) {
    f32 half_width  = (size.x / 100.0) / 2.0f;
    f32 half_height = (size.y / 100.0) / 2.0f;
    f32 half_depth  = (size.z / 100.0) / 2.0f;

    f32 vertices[] = {
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

    u32 indices[] = {
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

    return koncept_core->graphics.create_vertex_data(
        vertices, 24,
        indices, 36,
        KC_LOCATION_ATTR | KC_COLOR_ATTR | KC_TCOORD_ATTR | KC_NORMAL_ATTR
    );
}


u8 _koncept_init_shape3D(_koncept_3D_api* koncept_3D) {
    if (!koncept_3D) return COREX_FALSE;  // error: null ptr!
    
    if (koncept_3D->shape3D.cube3D != NULL
    ) return COREX_TRUE; // error: reduntant call: shape3D api initialized!

    koncept_3D->shape3D.cube3D = _cube3D_impl;

    return COREX_TRUE;
}
