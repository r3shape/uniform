#pragma once

#include "../lotustypes.h"
#include "../defines.h"

LThashmap* ltMakeHashmap(int max);
void ltDestroyHashmap(LThashmap* m);
int ltStringHash(const char* buffer);

bool ltProbeHashmapF(LThashmap* m, int* kHash, const char* key);
bool ltProbeHashmapR(LThashmap* m, int* kHash, const char* key);

void* ltGetHashmap(LThashmap* m, const char* key);
LTerrorType ltRemHashmap(LThashmap* m, const char* key);
LTerrorType ltSetHashmap(LThashmap* m, const char* key, void* value);

