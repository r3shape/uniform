#include "../engine/include/r3.h"

#include <stdio.h>

u8 my_callback(u16 event_code, R3_Event data) {
    u16 key = data.u16[0];
    if (key == R3_KEY_ESCAPE) printf("Quit Pressed!\n");
    return LIBX_TRUE;
}

int main() {
    if (r3_init_core()) printf("r3 core initialized!\n");
    else printf("r3 core failed to initialize!\n");
    
    R3_Window window = r3_core->platform.create_window("My Window", 800, 600);
    if (r3_core->platform.create_gl_context()) printf("GL Context Created!\n");
    if (r3_core->graphics.init_gl(&r3_core->graphics)) printf("GL Functions Loaded!\n");
    
    if (r3_core->events.register_callback(R3_EVENT_KEY_PRESSED, my_callback)) printf("Event Callback Registered!\n");
    
    r3_core->graphics.render_begin(0, math_api->create_vector(LIBX_VEC4, (f32[4]){123/255.0, 161/255.0, 172/255.0, 255/255.0}), NULL);
    
    u8 running = 1;
    while (running) {
        r3_core->platform.poll_events();
        r3_core->platform.poll_inputs();
        r3_core->graphics.render_clear();
        
        if (r3_core->input.key_was_down(R3_KEY_ESCAPE)) running = 0;
        
        r3_core->graphics.render_call(R3_TRIANGLE_MODE, 10, 420, 0, 25);
        r3_core->graphics.render_call(R3_TRIANGLE_MODE, 17, 666, 0, 11);
        r3_core->graphics.render_end();

        r3_core->platform.swap_buffers();
    }
    
    if (r3_core->events.unregister_callback(123, my_callback)) printf("Event Callback Unregistered!\n");
    
    r3_core->platform.destroy_gl_context();
    r3_core->platform.destroy_window(&window);

    if (r3_cleanup_core()) printf("r3 core cleaned up!\n");
    else printf("r3 core failed to clean up!\n");
    return 0;
}
