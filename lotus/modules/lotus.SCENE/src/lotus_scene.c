#include "../include/lotus_scene.h"

Lotus_Scene* lotus_init_scene(ubyte id, const char* name) {
    lotus_set_log_level(LOTUS_LOG_FATAL);
    
    Lotus_Scene* scene = (Lotus_Scene*)malloc(sizeof(Lotus_Scene));
    if (!scene) {
        lotus_log_fatal("Failed to allocate (scene): %s\n", scene->info.name);
        return NULL;
    }

    scene->info.id = id;
    scene->info.name = (!name) ? "Lotus Scene" : name;
    
    scene->allocator = lotus_make_allocator(LOTUS_DEFAULT_SCENE_HEAP_SIZE);

    return scene;
}

void lotus_destroy_scene(Lotus_Scene* scene) {
    if (!scene) return;
    lotus_destroy_allocator(&scene->allocator);
}

