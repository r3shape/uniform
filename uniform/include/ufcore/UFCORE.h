#ifndef _UFCORE_H_
#define _UFCORE_H_

#define _UF_R3DEF_
#include <ufcore/UFOS.h>
#include <ufcore/UFAPP.h>
#include <ufcore/UFMRG.h>
#include <ufcore/UFGPU.h>
#include <ufcore/UFEVIN.h>
#include <ufcore/UFPSOA.h>

UF_API u8 ufInitOS();
UF_API u8 ufInitMRG();
UF_API u8 ufInitGPU();
UF_API u8 ufInitEVIN();
UF_API u8 ufInitPSOA();

#endif //_UFCORE_H_
