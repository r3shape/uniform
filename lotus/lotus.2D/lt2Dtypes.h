#ifndef LT2DTYPES_H
#define LT2DTYPES_H

#include "../lotus.core/defines.h"
#include "../lotus.core/lotustypes.h"

// signatures of internal engine "getter" functions
void* ltGetWindow(void);
void* ltGetRenderState(void);
void* ltGetPlatformState(void);

#endif