#pragma once

#include "../lotustypes.h"
#include "../defines.h"

// dynamic value
int ecGetInt(LTvalue* v);
float ecGetFloat(LTvalue* v);
char* ecGetString(LTvalue* v);

void ecDestroyValue(LTvalue* v);
LTvalue* ecMakeInt(int value);
LTvalue* ecMakeFloat(float value);
LTvalue* ecMakeString(const char* value);
LTvalue* ecMakeValue(LTvalueType type, void* value);

// dynamic array
void ecDestroyArray(LTarray* inArr);
LTarray* ecMakeArray(int max, int resize);
LTerrorType ecResizeArray(LTarray* inArr);
LTvalue* ecPopArray(LTarray* inArr, int index);
int ecQueryArrayInt(LTarray* inArr, int index);
LTvalue* ecQueryArray(LTarray* inArr, int index);
float ecQueryArrayFloat(LTarray* inArr, int index);
char* ecQueryArrayString(LTarray* inArr, int index);
LTerrorType ecInsertArray(LTarray* inArr, int index, LTvalue* value);
LTerrorType ecInsertArrayV(LTarray* inArr, int count, LTvalue** values);