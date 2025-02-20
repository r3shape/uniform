#define R3_NEXT
#include "../../r3engine/include/r3engine.h"

#include <stdio.h>

// include our plugin header
#include "../../examples/simple_plugin/simple_plugin.h"

void main() {
    r3_init_core();
    if (r3_init_plug()) printf("`R3-Next` Layer Initialized: R3engine Plug\n");

    Simple_Plugin_API* simple_api = r3_plug_api->load_plug("Simple-Plugin", "../bin/plugins");
    if (!simple_api) printf("Failed To Load Plugin!\n");

    simple_api->hello_plugin();
    printf("Simple Addition Result (33*2) + 3: %d\n", simple_api->add_numbers((33*2), 3));
    simple_api->goodbye_plugin();

    r3_shutdown_plug();
    printf("Shutown Plugin Layer!\n");
    r3_shutdown_core();
    printf("Shutown Core Layer!\n");
}
