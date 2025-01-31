#define LOTUS_NEXT
#include "../engine/include/lotus.h"

#include <stdio.h>

// include our plugin header
#include "../examples/simple_plugin/simple_plugin.h"

void main() {
    lotus_init_core();
    if (lotus_init_plug()) printf("`Lotus-Next` Layer Initialized: Lotus Plug\n");

    Simple_Plugin_API* simple_api = lotus_plug_api->load_plug("simple_plugin", "../build/plugins");
    if (!simple_api) printf("Failed To Load Plugin!\n");

    simple_api->hello_plugin();
    printf("Simple Addition Result (33*2) + 3: %d\n", simple_api->add_numbers((33*2), 3));
    simple_api->goodbye_plugin();

    lotus_shutdown_plug();
    printf("Shutown Plugin Layer!\n");
    lotus_shutdown_core();
    printf("Shutown Core Layer!\n");
}
