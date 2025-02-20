#include "simple_plugin.h"

#include <stdio.h>

// implement your plugin API!
void hello_plug_impl(void)          { printf("Hello R3 Plug!\n"); }
int add_numbers_impl(int a, int b)  { return a + b; }
void goodbye_plug_impl(void)        { printf("Goodbye R3 Plug!\n"); }

// write logic to initialize your plugin here!
R3_PLUG_INIT(
    printf("Simple Plugin Init!\n");
    return R3_TRUE;
)

// write logic to shutdown your plugin here!
R3_PLUG_SHUTDOWN(
    printf("Simple Plugin Shutdown!\n");
)

/**
 * this macro returns a pointer to a static instance of your plugin API.
 * the order of the functions listed after the API type MUST match the order defined in your plugin's header.
 * (^^^ look for a call to `R3_PLUG_API` ^^^)
 */
R3_PLUG_EXPORT(Simple_Plugin_API,
    hello_plug_impl,
    add_numbers_impl,
    goodbye_plug_impl
)
