#include "../../include/lotusECS.h"

int main () {
    b8 init = ltECSInit();

    Entity e = MakeEntity("lotus.entity");
    SetComponent(COMP2D_MESH, &e);

    if (HasComponent(COMP2D_MESH, &e)) { printf("entity has mesh component!\n"); }
    if (!HasComponent(COMP2D_TEXTURE, &e)) { printf("entity has no texture component!\n"); }
    if (!HasComponent(COMP2D_TRANSFORM, &e)) { printf("entity has no transform component!\n"); }

    ltECSExit();
    return 0;
}

