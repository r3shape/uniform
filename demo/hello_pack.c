#include "../engine/include/r3.h"
#include "../engine/assets/packs/test_pack/test_pack.h"

void main() {
    r3_init_core();
    r3_init_pack(r3_core);

    test_pack_api* tpack = r3_pack->load_pack("test_pack", "../bin/packs");
    if (!tpack) printf("failed to load test pack!\n");
    else printf("loaded test pack!\n");

    tpack->hello_test("d34d0s");
    printf("2 + 2 = %d\n", tpack->add_test(2, 2));

    if(r3_pack->unload_pack("test_pack")) printf("test pack unloaded!\n");
    else printf("failed to unload test_pack!\n");

    r3_cleanup_pack();
    r3_cleanup_core();
}
