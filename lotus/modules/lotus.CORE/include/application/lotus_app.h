#pragma once

#include "../platform/lotus_input.h"
#include "../platform/lotus_event.h"
#include "../platform/lotus_platform.h"

#include "../graphics/lotus_gl.h"

#include "../utility/lotus_hashmap.h"

#include "../../../lotus.SCENE/include/lotus_scene.h"
#include "../../../lotus.PLUG/include/lotus_plug.h"

#define LOTUS_ENGINE_SCENE_MAX  (1U << 4)
#define LOTUS_DEFAULT_APPLICATION_HEAP_SIZE 1024 * 1024 // 1kb application heap for runtime allocations

typedef struct Lotus_Application Lotus_Application;
typedef ubyte4(*_Application_Preframe_Callback_Ptr)(void);
typedef ubyte4(*_Application_Fixedframe_Callback_Ptr)(void);
typedef ubyte4(*_Application_Midframe_Callback_Ptr)(void);
typedef ubyte4(*_Application_Postframe_Callback_Ptr)(void);

typedef enum Lotus_Application_Event {
    LOTUS_APPLICATION_PREFRAME_EVENT = 0,
    LOTUS_APPLICATION_FIXEDFRAME_EVENT,
    LOTUS_APPLICATION_MIDFRAME_EVENT,
    LOTUS_APPLICATION_POSTFRAME_EVENT,
    LOTUS_APPLICATION_EVENTS,
    LOTUS_APPLICATION_EVENT_MAX = LOTUS_EVENT_MAX
} Lotus_Application_Event;

struct Lotus_Application {
    struct info {
        const char* name;
        ubyte4 window_size[2];
    } info;

    struct state {
        ubyte running;
        ubyte callback_count;

        ubyte scene_count;
        Lotus_Scene* current_scene;
    } state;

    struct resource {
        Lotus_Window window;
        Lotus_Allocator allocator;
        
        Lotus_Scene* scenes[LOTUS_ENGINE_SCENE_MAX];
        
        Lotus_Platform_API* platform_api;
        Lotus_Graphics_API* graphics_api;

        Lotus_Plug_API* plug_api;
        Lotus_Plug_Manager* plug_manager;
    } resource;

    Lotus_Event_Callback callbacks[LOTUS_APPLICATION_EVENTS];
};

typedef struct Lotus_Application_API {
    Lotus_Application* (*initialize)(const char* app_name, ubyte4 window_size[2]);
    void (*shutdown)(void);

    ubyte (*create_window)(const char* title, ubyte4 size[2]);
    void (*destroy_window)(void);
    
    sbyte (*create_scene)(const char* scene_name);
    Lotus_Scene* (*get_scene)(ubyte scene_id);
    ubyte (*set_scene)(ubyte scene_id);
    void (*destroy_scene)(ubyte scene_id);

    ubyte (*set_callback)(Lotus_Application_Event event, Lotus_Event_Callback callback);
    ubyte (*run)(void);

    // application-level ECS wrapper
    ubyte (*init_ecs)(ubyte scene_id);
    void (*shutdown_ecs)(ubyte scene_id);

    Lotus_Entity (*make_entity)(ubyte scene_id);
    ubyte (*kill_entity)(ubyte scene_id, Lotus_Entity entity);

    ubyte (*register_component)(
        ubyte scene_id,
        Lotus_Component_Type type,
        void* data,
        _add_component_ptr add_component,
        _rem_component_ptr rem_component,
        _set_component_ptr set_component,
        _get_component_ptr get_component
    );
    ubyte (*unregister_component)(ubyte scene_id, Lotus_Component_Type type);
    
    void (*add_component)(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity);
    ubyte (*has_component)(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity);
    void (*rem_component)(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity);
    void (*set_component)(ubyte scene_id, Lotus_Component component, Lotus_Entity entity);
    Lotus_Component (*get_component)(ubyte scene_id, Lotus_Component_Type type, Lotus_Entity entity);
} Lotus_Application_API;

LOTUS_API_ENTRY Lotus_Application_API* lotus_init_application(void);
