#include "../../include/lotusECS.h"

int main () {
    b8 ecsInit = ltECSInit();

    LTscene* scene = ltMakeScene("PlanetPeepo");
    printf("My Scene Name: %s\n", scene->info.name);

    LTentity e0 = scene->entityManager.makeEntity();
    LTentity e1 = scene->entityManager.makeEntity();

    printf("My First Entity: %d\n", e0);
    printf("My Second Entity: %d\n", e1);
    
    b8 added = scene->componentManager.addComponent(e1, LOTUS_TRANSFORM_COMPONENT);
    printf("E1 Transfom Added: %d\n", added);

    printf("E0 Transfom Present: %d\n", scene->componentManager.hasComponent(e0, LOTUS_TRANSFORM_COMPONENT));
    printf("E1 Transfom Present: %d\n", scene->componentManager.hasComponent(e1, LOTUS_TRANSFORM_COMPONENT));

    LTcomponent transform = scene->componentManager.getComponent(e1, LOTUS_TRANSFORM_COMPONENT);
    if (transform.type == LOTUS_TRANSFORM_COMPONENT) {
        LTvec3* location = ((LTvec3*)transform.data.transform.location);
        location->x += 100;
        printf("E1 Location: %0.1f, %0.1f, %0.1f\n", location->x, location->y, location->z);
    }

    scene->componentManager.remComponent(e1, LOTUS_TRANSFORM_COMPONENT);
    printf("E1 Transfom Present: %d\n", scene->componentManager.hasComponent(e1, LOTUS_TRANSFORM_COMPONENT));

    ltDestroyScene(scene);

    return 0;
}

