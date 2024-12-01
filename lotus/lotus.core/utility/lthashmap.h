#pragma once

#include "../lotustypes.h"
#include "../defines.h"

int ltStringHash(const char* buffer);
bool ltProbeHashmapF(LThashmap* m, int* kHash, const char* key);
bool ltProbeHashmapR(LThashmap* m, int* kHash, const char* key);

LOTUS_API_ENTRY LThashmap* ltMakeHashmap(int max);
LOTUS_API_ENTRY void ltDestroyHashmap(LThashmap* m);
LOTUS_API_ENTRY void* ltGetHashmap(LThashmap* m, const char* key);
LOTUS_API_ENTRY LTerrorType ltRemHashmap(LThashmap* m, const char* key);
LOTUS_API_ENTRY LTerrorType ltSetHashmap(LThashmap* m, const char* key, void* value);

