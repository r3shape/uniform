#define LOTUS_2D
#include "../../engine/include/lotus.h"
#include <stdio.h>

int list_test() {
    f32 data[3] = {
        420.0,
        123.0,
        456.0
    };

    Lotus_List_Node* start_node = lotus_create_list(sizeof(f32));
    if (start_node) {
        printf("node1 created\n");
        start_node->data = &data[0];\
    }
    
    if (lotus_create_list_node(start_node)) printf("node2 created\n");
    if (lotus_create_list_node(start_node)) printf("node3 created\n");

    Lotus_List_Node* node2 = lotus_query_list_node(start_node, 1);
    if (node2) node2->data = &data[1];

    Lotus_List_Node* node3 = lotus_query_list_node(start_node, 2);
    if (node3) node3->data = &data[2];

    Lotus_List_Header list_header = lotus_get_list_header(start_node);

    f32 node_data[3] = {
        *(f32*)start_node->data,
        *(f32*)node3->prev->data,
        *(f32*)node2->next->data,
    };

    printf("Node Data: %0.1f, %0.1f, %0.1f\n", node_data[0], node_data[1], node_data[2]);

    lotus_destroy_list(start_node);

    return 0;
}

int array_test() {
    f32 *arr = (f32 *)lotus_create_array(sizeof(f32), 3);
    lotus_insert_array(arr, 0, &(f32){123.0});
    lotus_insert_array(arr, 1, &(f32){456.0});
    lotus_insert_array(arr, 2, &(f32){420.0});
    
    lotus_print_array("Some Array", arr);
    lotus_destroy_array(arr);

    return 0;
}

int memory_test() {
    float vec3[] = {
        123.0,
        456.0,
        420.0
    };

    Lotus_Memory_Region regionV3F;
    lotus_memory_api->init_region(&regionV3F, sizeof(float), 3, 16);

    lotus_memory_api->set_region_data(&regionV3F, &vec3[0]);
    lotus_memory_api->set_region_data(&regionV3F, &vec3[1]);
    lotus_memory_api->set_region_data(&regionV3F, &vec3[2]);

    printf("Region Vec3: (cap): %d | (count) %d | (data): <%0.1f, %0.1f, %0.1f>\n",
    regionV3F.capacity,
    regionV3F.count,
    *(float *)lotus_memory_api->get_region_data(&regionV3F, 0),
    *(float *)lotus_memory_api->get_region_data(&regionV3F, 1),
    *(float *)lotus_memory_api->get_region_data(&regionV3F, 2)
    );

    lotus_memory_api->spawn_region(&regionV3F, sizeof(int), 1, 16);
    Lotus_Memory_Region* intRegion = lotus_memory_api->get_region(&regionV3F, 1);

    int age = 22;
    lotus_memory_api->set_region_data(intRegion, &age);
    printf("Age: %d\n", *(int *)lotus_memory_api->get_region_data(regionV3F.next, 0));

    lotus_memory_api->free_all_regions(&regionV3F);

    return 0;
}

int hashmap_test() {
    Lotus_Hashmap *hmap = lotus_create_hashmap(10);

    lotus_set_hashmap(hmap, "Name", "D34D0S");
    
    char *name = (char *)lotus_get_hashmap(hmap, "Name");
    if (name) printf("Name Value: %s\n", name);
    else printf("Failed to get name key!\n");

    lotus_destroy_hashmap(hmap);

    return 0;
}

int graphics_test() {
    Lotus_Window window = lotus_platform_api->create_window("My Lotus Window", 1280, 720);
    lotus_platform_api->create_gl_context(&window);

    lotus_init_graphics();

    Lotus_Shader shader = lotus_graphics_api->make_shader(
        lotus_read_file("engine/assets/shaders/default001/vertex.glsl"),
        lotus_read_file("engine/assets/shaders/default001/fragment.glsl")
    );
    lotus_graphics_api->set_shader(&shader);
    
    Lotus_Mat4 view = lotus_identity();
    view = lotus_look_at(
        lotus_new_vec3(0.0f, 0.0f, 1.0f),
        lotus_new_vec3(0.0f, 0.0f, 0.0f),
        lotus_new_vec3(0.0f, 1.0f, 0.0f)
    );
    lotus_graphics_api->set_uniform(&shader, "u_view", &view);

    Lotus_Mat4 tModel = lotus_identity();
    tModel = lotus_mul_mat4(lotus_identity(), lotus_trans_mat4(1280/2, 720/2, 0));
    Lotus2D_Primitive triangle = lotus2d_create_triangle(lotus_new_vec2(32, 32), LOTUS_COLOR3(25, 80, 150));

    Lotus_Mat4 rModel = lotus_identity();
    rModel = lotus_mul_mat4(lotus_identity(), lotus_trans_mat4(1280/4, 720/4, 0));
    Lotus2D_Primitive rectangle = lotus2d_create_rectangle(lotus_new_vec2(64, 64), LOTUS_COLOR3(43, 58, 12));

    lotus_graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(133, 161, 172, 255),
        lotus_ortho(0, window.size[0], 0, window.size[1], 0.1, 10.0)
    );
    
    ubyte4 frame = 0;
    while (frame < 255) {
        lotus_platform_api->poll_events();
        lotus_graphics_api->draw_clear();

        if (lotus_key_is_down(LOTUS_KEY_A)) printf("Pressing A!\n");

        lotus_graphics_api->send_uniform(&shader, LOTUS_UNIFORM_MAT4, "u_view");
        
        lotus_graphics_api->set_uniform(&shader, "u_model", &tModel);
        lotus_graphics_api->draw_data(triangle.vertexData);
        
        lotus_graphics_api->set_uniform(&shader, "u_model", &rModel);
        lotus_graphics_api->draw_data(rectangle.vertexData);

        lotus_platform_api->poll_inputs();
        lotus_platform_api->swap_buffers(&window);
        frame++;
    }

    lotus_graphics_api->destroy_vertex_data(&triangle.vertexData);
    lotus_graphics_api->destroy_vertex_data(&rectangle.vertexData);
    lotus_platform_api->destroy_gl_context(&window);
    lotus_platform_api->destroy_window(&window);

    lotus_shutdown_graphics();

    return 0;
}

int main() {
    lotus_init_core();

    list_test();
    array_test();
    memory_test();
    hashmap_test();
    graphics_test();

    lotus_shutdown_core();
    return 0;
}
