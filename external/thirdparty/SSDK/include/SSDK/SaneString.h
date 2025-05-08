#ifndef __SANESTRING_H__
#define __SANESTRING_H__

#include <include/SSDK/globals.h>

typedef struct SaneString {} SaneString;
extern SaneString* saneString;

SSDK_FUNC byte ssdkInitString(none);
SSDK_FUNC none ssdkExitString(none);

#endif  // __SANESTRING_H__