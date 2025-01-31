#include "simple_plugin.h"

#include <stdio.h>

// implement your plugin API!
void hello_plug_impl(void)          { printf("Hello Lotus Plug!\n"); }
int add_numbers_impl(int a, int b)  { return a + b; }
void goodbye_plug_impl(void)        { printf("Goodbye Lotus Plug!\n"); }

// write logic to initialize your plugin here!
LOTUS_PLUG_INIT(
    printf("Simple Plugin Init!\n");
    return LOTUS_TRUE;
)

// write logic to shutdown your plugin here!
LOTUS_PLUG_SHUTDOWN(
    printf("Simple Plugin Shutdown!\n");
)

/**
 * this macro returns a pointer to a static instance of your plugin API.
 * the order of the functions listed after the API type MUST match the order defined in your plugin's header.
 * (^^^ look for a call to `LOTUS_PLUG_API` ^^^)
 */
LOTUS_PLUG_EXPORT(Simple_Plugin_API,
    hello_plug_impl,
    add_numbers_impl,
    goodbye_plug_impl
)
