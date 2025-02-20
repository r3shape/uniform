#define R3_2D
#include "../../r3engine/include/r3engine.h"
#include <stdio.h>

int list_test() {
    f32 data[3] = {
        420.0,
        123.0,
        456.0
    };

    R3_List_Node* start_node = r3_create_list(sizeof(f32));
    if (start_node) {
        printf("node1 created\n");
        start_node->data = &data[0];\
    }
    
    if (r3_create_list_node(start_node)) printf("node2 created\n");
    if (r3_create_list_node(start_node)) printf("node3 created\n");

    R3_List_Node* node2 = r3_query_list_node(start_node, 1);
    if (node2) node2->data = &data[1];

    R3_List_Node* node3 = r3_query_list_node(start_node, 2);
    if (node3) node3->data = &data[2];

    R3_List_Header list_header = r3_get_list_header(start_node);

    f32 node_data[3] = {
        *(f32*)start_node->data,
        *(f32*)node3->prev->data,
        *(f32*)node2->next->data,
    };

    printf("Node Data: %0.1f, %0.1f, %0.1f\n", node_data[0], node_data[1], node_data[2]);

    r3_destroy_list(start_node);

    return 0;
}

int array_test() {
    f32 *arr = (f32 *)r3_create_array(sizeof(f32), 3);
    r3_insert_array(arr, 0, &(f32){123.0});
    r3_insert_array(arr, 1, &(f32){456.0});
    r3_insert_array(arr, 2, &(f32){420.0});
    
    r3_print_array("Some Array", arr);
    r3_destroy_array(arr);

    return 0;
}

int memory_test() {
    float vec3[] = {
        123.0,
        456.0,
        420.0
    };

    R3_Memory_Region regionV3F;
    r3_memory_api->init_region(&regionV3F, sizeof(float), 3, 16);

    r3_memory_api->set_region_data(&regionV3F, &vec3[0]);
    r3_memory_api->set_region_data(&regionV3F, &vec3[1]);
    r3_memory_api->set_region_data(&regionV3F, &vec3[2]);

    printf("Region Vec3: (cap): %d | (count) %d | (data): <%0.1f, %0.1f, %0.1f>\n",
    regionV3F.capacity,
    regionV3F.count,
    *(float *)r3_memory_api->get_region_data(&regionV3F, 0),
    *(float *)r3_memory_api->get_region_data(&regionV3F, 1),
    *(float *)r3_memory_api->get_region_data(&regionV3F, 2)
    );

    r3_memory_api->spawn_region(&regionV3F, sizeof(int), 1, 16);
    R3_Memory_Region* intRegion = r3_memory_api->get_region(&regionV3F, 1);

    int age = 22;
    r3_memory_api->set_region_data(intRegion, &age);
    printf("Age: %d\n", *(int *)r3_memory_api->get_region_data(regionV3F.next, 0));

    r3_memory_api->free_all_regions(&regionV3F);

    return 0;
}

int hashmap_test() {
    R3_Hashmap *hmap = r3_create_hashmap(10);

    r3_set_hashmap(hmap, "Name", "D34D0S");
    
    char *name = (char *)r3_get_hashmap(hmap, "Name");
    if (name) printf("Name Value: %s\n", name);
    else printf("Failed to get name key!\n");

    r3_destroy_hashmap(hmap);

    return 0;
}

int graphics_test() {
    R3_Window* window = r3_platform_api->create_window("My R3 Window", 1280, 720);
    r3_platform_api->create_gl_context(window);

    r3_init_graphics();

    R3_Shader shader = r3_graphics_api->create_shader(
        r3_read_file("r3/assets/shaders/default001/vertex.glsl"),
        r3_read_file("r3/assets/shaders/default001/fragment.glsl")
    );
    r3_graphics_api->set_shader(&shader);
    
    R3_Mat4 view = r3_identity();
    view = r3_look_at(
        r3_new_vec3(0.0f, 0.0f, 1.0f),
        r3_new_vec3(0.0f, 0.0f, 0.0f),
        r3_new_vec3(0.0f, 1.0f, 0.0f)
    );
    r3_graphics_api->set_uniform(&shader, "u_view", &view);

    r3_graphics_api->draw_begin(
        R3_TRIANGLE_MODE,
        R3_COLOR4(133, 161, 172, 255),
        r3_ortho(0, window->size[0], 0, window->size[1], 0.1, 10.0)
    );
    
    ubyte4 frame = 0;
    while (frame < 255) {
        r3_platform_api->poll_events();
        r3_graphics_api->draw_clear();

        if (r3_key_is_down(R3_KEY_A)) printf("Pressing A!\n");

        r3_graphics_api->send_uniform(&shader, R3_UNIFORM_MAT4, "u_view");
        
        r3_platform_api->poll_inputs();
        r3_platform_api->swap_buffers(window);
        frame++;
    }

    r3_platform_api->destroy_gl_context(window);
    r3_platform_api->destroy_window(window);

    r3_shutdown_graphics();

    return 0;
}

int main() {
    r3_init_core();

    list_test();
    array_test();
    memory_test();
    hashmap_test();
    graphics_test();

    r3_shutdown_core();
    return 0;
}
