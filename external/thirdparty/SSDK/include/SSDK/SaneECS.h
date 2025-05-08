#ifndef __SANEECS_H__
#define __SANEECS_H__

#include <include/SSDK/globals.h>

typedef struct SaneECS {} SaneECS;
extern SaneECS* saneECS;

SSDK_FUNC byte ssdkInitECS(none);
SSDK_FUNC none ssdkExitECS(none);

#endif  // __SANEECS_H__