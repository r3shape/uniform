#ifndef __SANEFIO_H__
#define __SANEFIO_H__

#include <include/SSDK/globals.h>

typedef struct SaneFile {} SaneFile;
extern SaneFile* saneFile;

SSDK_FUNC byte ssdkInitFile(none);
SSDK_FUNC none ssdkExitFile(none);

#endif  // __SANEFIO_H__