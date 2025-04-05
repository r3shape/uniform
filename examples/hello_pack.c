#define R3_MODULES R3_CORE|R3_PACK
#include <r3/r3.core/include/r3.core.h>
#include <r3/r3.pack/installed/include/test_pack.h>

void main() {
    r3_init_core(R3_MODULES);

    test_pack_api* tpack = (test_pack_api*)r3_pack->load_pack("test_pack", "r3/r3.pack/installed/bin");
    if (!tpack) printf("failed to load test pack!\n");
    else printf("loaded test pack!\n");

    tpack->hello_test("d34d0s");
    printf("2 + 2 = %d\n", tpack->add_test(2, 2));

    if(r3_pack->unload_pack("test_pack")) printf("test pack unloaded!\n");
    else printf("failed to unload test_pack!\n");

    r3_cleanup_core();
}