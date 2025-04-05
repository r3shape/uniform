#include <r3/r3.pack/installed/include/test_pack.h>

void _hello_test_impl(str name) {
    printf("Hello, %s!\n", name);
}

u32 _add_test_impl(u32 num1, u32 num2) {
    return num1 + num2;
}

R3_PACK_INIT(
    printf("test pack initialized!\n");
    return LIBX_TRUE;
)

R3_PACK_CLEANUP(
    printf("test pack cleaned-up!\n");
    return LIBX_TRUE;
)

R3_PACK_EXPORT(test_pack_api,
    _hello_test_impl,
    _add_test_impl
)
