#define R3_PACK
#include "../../../include/r3.h"

#include <stdio.h>

R3_PACK_API(test_pack,
    R3_PACK_METHOD(void, hello_test, str name);
    R3_PACK_METHOD(u32, add_test, u32 num1, u32 num2);
);
