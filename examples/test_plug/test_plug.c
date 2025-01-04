// tst_plug.c
#include "test_plug.h"

// plugin API implementation
void hello_plug_impl(void) {
    printf("Hello Lotus Plug!\n");
}

void goodbye_plug_impl(void) {
    printf("Goodbye Lotus Plug!\n");
}

int add_numbers_impl(int a, int b) { return a + b; }

// plugin API configuration
LOTUS_DEFINE_PLUG_INIT(
    printf("My Lotus Plug Init!\n");
)

LOTUS_DEFINE_PLUG_SHUTDOWN(
    printf("My Lotus Plug Shutdown!\n");
)

LOTUS_EXPORT_PLUG_API(Test_Plug_API,
    hello_plug_impl,
    goodbye_plug_impl,
    add_numbers_impl
);

