#pragma once

#include "../lotustypes.h"
#include "../defines.h"

// dynamic value
int ltGetInt(LTvalue* v);
float ltGetFloat(LTvalue* v);
char* ltGetString(LTvalue* v);

void ltDestroyValue(LTvalue* v);
LTvalue* ltMakeInt(int value);
LTvalue* ltMakeFloat(float value);
LTvalue* ltMakeString(const char* value);
LTvalue* ltMakeValue(LTvalueType type, void* value);

// dynamic array
void ltDestroyArray(LTarray* inArr);
LTarray* ltMakeArray(int max, int resize);
LTerrorType ltResizeArray(LTarray* inArr);
LTvalue* ltPopArray(LTarray* inArr, int index);
int ltQueryArrayInt(LTarray* inArr, int index);
LTvalue* ltQueryArray(LTarray* inArr, int index);
float ltQueryArrayFloat(LTarray* inArr, int index);
char* ltQueryArrayString(LTarray* inArr, int index);
LTerrorType ltInsertArray(LTarray* inArr, int index, LTvalue* value);
LTerrorType ltInsertArrayV(LTarray* inArr, int count, LTvalue** values);