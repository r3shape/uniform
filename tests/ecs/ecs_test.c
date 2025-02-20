#include "../../r3engine/include/r3engine.h"
#include "../../r3engine/modules/ecs/include/r3_ecs.h"

#include <stdio.h>

typedef enum Component_2D {
    MESH2D = 0,
    TRANSFORM2D,
    COMPONENTS_2D
} Component_2D;

typedef struct Mesh2D {
    R3_Vertex_Data vertex_data;
} Mesh2D;
typedef struct Transform2D {
    R3_Vec2 velocity;
    R3_Vec2 location;
} Transform2D;

void render_system(ubyte2 entity_id) {
    if (!r3_ecs_api->has_component(entity_id, MESH2D)) return;
    Mesh2D* mesh = (Mesh2D*)r3_ecs_api->get_component(entity_id, MESH2D);
    r3_graphics_api->draw_data(mesh->vertex_data);
}

void transform_system(ubyte2 entity_id) {
    if (!r3_ecs_api->has_component(entity_id, TRANSFORM2D)) return;
    Transform2D* transform = (Transform2D*)r3_ecs_api->get_component(entity_id, TRANSFORM2D);
    transform->location.x += transform->velocity.x;
    transform->location.y += transform->velocity.y;
    printf("Entity %d moved to (%.2f, %.2f)\n", entity_id, transform->location.x, transform->location.y);
}

int main() {
    r3_init_core();
    r3_init_ecs();

    r3_ecs_api->register_component(sizeof(Mesh2D), MESH2D);
    r3_ecs_api->register_system(MESH2D, render_system);
    
    r3_ecs_api->register_component(sizeof(Transform2D), TRANSFORM2D);
    r3_ecs_api->register_system(TRANSFORM2D, transform_system);

    ubyte base_components[] = {MESH2D, TRANSFORM2D};
    ubyte2 base_prefab = r3_ecs_api->create_prefab(base_components, 2);

    // Instantiate the prefab
    ubyte2 entity = r3_ecs_api->instance_prefab(base_prefab);

    // Entity now has Mesh2D and Transform2D!
    Mesh2D* mesh = (Mesh2D*)r3_ecs_api->get_component(entity, MESH2D);
    Transform2D* transform = (Transform2D*)r3_ecs_api->get_component(entity, TRANSFORM2D);

    transform->location.x = 0;
    transform->location.y = 0;
    transform->velocity.x = 20.0f;
    transform->velocity.y = 3.0f;

    r3_ecs_api->run_system(TRANSFORM2D);
    // r3_ecs_api->run_system(MESH2D);   // a window needs to be created and the graphics layer initialized

    r3_shutdown_ecs();
    r3_shutdown_core();
    return 0;
}
