#define LOTUS_2D
#define LOTUS_APPLICATION
#include "../../lotus/include/lotus.h"

#include "plugs/test_plug/include/test_plug.h"

char vShader[] = {
    "#version 460 core\n"
    "layout(location = 0) in vec3 a_Location;\n"
    "layout(location = 1) in vec3 a_Color;\n"
    "uniform mat4 u_model;\n"
    "uniform mat4 u_view;\n"
    "uniform mat4 u_projection;\n"
    "out vec3 vertex_color;\n"
    "void main() {\n"
    "   gl_Position = u_projection * u_view * u_model * vec4(a_Location, 1.0f);\n"
    "   vertex_color = a_Color;\n"
    "}\0"
};

char fShader[] = {
    "#version 460 core\n"
    "in vec3 vertex_color;\n"
    "out vec4 frag_color;\n"
    "void main() {\n"
    "   frag_color = vec4(vertex_color, 1.0f);\n"
    "}\0"
};
Lotus_Shader my_shader;

// camera-data handled manually until implentation :)
Lotus_Mat4 m_view;
Lotus_Vec3 up = {0.0f, 1.0f, 0.0f};
Lotus_Vec3 eye = {0.0f, 0.0f, 1.0f};
Lotus_Vec3 center = {0.0f, 0.0f, 0.0f};

Lotus_Application_API* app_api = NULL;

Lotus_Application* app = NULL;
ubyte scene_id;

Lotus_Entity e0;
Lotus_Component e0_mesh;
Lotus_Component e0_transform;

// lotus plug example
void load_plug() {
    Lotus_Plug_API* plug_api = app->resource.plug_api;
    // load a plugin
    Lotus_Plug* test_plug = plug_api->load_plug(app->resource.platform_api, "plugs/test_plug/bin", "test_plug");
    if (test_plug) {
        // export and use the plugin's API
        Test_Plug_API* test_plug_api = test_plug->export_api();
        if (!test_plug_api) {
            printf("Failed to export plugin API\n");
        } else printf("Exported plugin API!\n");

        test_plug_api->hello_plug();
        printf("Addition Result: %d\n", test_plug_api->add_numbers(33*2, 3));
        test_plug_api->goodbye_plug();
    }; plug_api->unload_plug(app->resource.platform_api, "test_plug");
}

ubyte midframe_callback(Lotus_Event data, ubyte2 event_code) {
    if (event_code == LOTUS_APPLICATION_MIDFRAME_EVENT) {
        app->resource.graphics_api->draw_clear();

        f32 speed = 0.01;
        
        if (lotus_key_is_down(LOTUS_KEY_ESCAPE)) app->state.running = LOTUS_FALSE;

        if (lotus_key_is_down(LOTUS_KEY_A)) e0_transform.data.transform.location->x -= speed;
        if (lotus_key_is_down(LOTUS_KEY_D)) e0_transform.data.transform.location->x += speed;
        
        if (lotus_key_is_down(LOTUS_KEY_W)) e0_transform.data.transform.location->y += speed;
        if (lotus_key_is_down(LOTUS_KEY_S)) e0_transform.data.transform.location->y -= speed;
        
        if (lotus_key_is_down(LOTUS_KEY_Q)) e0_transform.data.transform.rotation->z -= 1.5;
        if (lotus_key_is_down(LOTUS_KEY_E)) e0_transform.data.transform.rotation->z += 1.5;
        
        if (lotus_key_is_down(LOTUS_KEY_F1)) app->resource.graphics_api->wireframe_mode(LOTUS_TRUE);
        else app->resource.graphics_api->wireframe_mode(LOTUS_FALSE);

        if (lotus_key_is_down(LOTUS_KEY_F5)) load_plug();

        app->resource.graphics_api->send_uniform(&my_shader, LOTUS_UNIFORM_MAT4, "u_view");

        app->resource.graphics_api->set_uniform(&my_shader, "u_model", e0_transform.data.transform.model);
        app->resource.graphics_api->draw_data((Lotus_Vertex_Data){
            .vbo = e0_mesh.data.mesh.vbo,
            .ebo = e0_mesh.data.mesh.ebo,
            .vao = e0_mesh.data.mesh.vao,
            .vertices = e0_mesh.data.mesh.vertices,
            .vertex_count = e0_mesh.data.mesh.vertex_count
        });
        
        return LOTUS_TRUE;
    }; return LOTUS_FALSE;
}

int main() {
    app_api = lotus_init_application();
    app = app_api->initialize("My Application", LOTUS_VEC2(ubyte4, 1280, 720));
    
    sbyte scene_id = app_api->create_scene("My Scene");

    e0 = app_api->make_entity(scene_id);

    app_api->add_component(scene_id, LOTUS_MESH_COMPONENT, e0);
    app_api->add_component(scene_id, LOTUS_TRANSFORM_COMPONENT, e0);
    
    app_api->set_component(scene_id, 
        (Lotus_Component){ 
            .type = LOTUS_MESH_COMPONENT, 
            .data.mesh.attrs = LOTUS_LOCATION_ATTR | LOTUS_COLOR_ATTR,
            .data.mesh.vertex_count = 3,
            .data.mesh.vertices = (f32[]){
                -0.5, -0.5, 0.5, 1.0, 0.0, 0.0,
                 0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
                 0.0,  0.5, 0.5, 0.0, 0.0, 1.0
            },
        }, e0
    );
    
    e0_mesh = app_api->get_component(scene_id, LOTUS_MESH_COMPONENT, e0);
    e0_transform = app_api->get_component(scene_id, LOTUS_TRANSFORM_COMPONENT, e0);
    
    my_shader = app->resource.graphics_api->make_shader(vShader, fShader);
    app->resource.graphics_api->set_shader(&my_shader);
    
    // set model matrix
    app->resource.graphics_api->set_uniform(&my_shader, "u_model", e0_transform.data.transform.model);

    // set view matrix
    m_view = lotus_look_at(eye, center, up);
    app->resource.graphics_api->set_uniform(&my_shader, "u_view", &m_view);
    
    // 2D camera
    // app->resource.graphics_api->draw_begin(
    //     LOTUS_TRIANGLE_MODE,
    //     LOTUS_COLOR4(133, 161, 172, 255),
    //     lotus_ortho(0, app->resource.window.size[0], 0, app->resource.window.size[1], 0.1, 10)
    // );
    
    // 3D camera
    app->resource.graphics_api->draw_begin(
        LOTUS_TRIANGLE_MODE,
        LOTUS_COLOR4(133, 161, 172, 255),
        lotus_perspective(lotus_to_radians(45.0), app->resource.window.size[0]/app->resource.window.size[1], 0.1, 1000.0)
    );
    
    app_api->set_callback(LOTUS_APPLICATION_MIDFRAME_EVENT, midframe_callback);
    
    app_api->run();
    
    return 0;
}
