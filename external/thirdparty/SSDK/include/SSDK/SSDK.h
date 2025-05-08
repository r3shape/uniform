/*
----------------------------------------------------------
Sane Software Development Kit
----------------------------------------------------------
Modules:
    - SaneLog           : Log module
    - SaneMath          : Math module
    - SaneMemory        : Memory module
    - SaneData          : Data structures module
    - SaneFIO           : File I/O module
    - SaneECS           : Entity-Component-System module
----------------------------------------------------------
*/

#ifndef __SSDK_H__
#define __SSDK_H__

#include <include/SSDK/SaneDS.h>
#include <include/SSDK/SaneLog.h>
#include <include/SSDK/SaneMath.h>
#include <include/SSDK/SaneMemory.h>

typedef struct SSDK {
    SaneLog log;
    SaneMath math;
} SSDK;
extern SSDK* ssdk;

SSDK_FUNC byte ssdkInit();
SSDK_FUNC none ssdkExit();

#endif  // __SSDK_H__