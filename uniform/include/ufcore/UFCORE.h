#ifndef _UFCORE_H_
#define _UFCORE_H_

#define _UF_R3DEF_
#include <ufcore/UFOS.h>
#include <ufcore/UFMRG.h>
#include <ufcore/UFEVIN.h>

UF_API u8 ufInitOS(UFEVINInterface* evinPtr, UFOSInterface* osInterface);
UF_API u8 ufExitOS(UFOSInterface* osInterface);

UF_API u8 ufInitMRG(UFGPUInterface* gpuPtr, UFMRGInterface* mrgInterface);
UF_API u8 ufExitMRG(UFMRGInterface* mrgInterface);

UF_API u8 ufInitGPU(UFResource window, UFOSInterface* osPtr, UFGPUInterface* gpuInterface);
UF_API u8 ufExitGPU(UFGPUInterface* gpuInterface);

UF_API u8 ufInitEVIN(UFEVINInterface* evinInterface);
UF_API u8 ufExitEVIN(UFEVINInterface* evinInterface);

#endif //_UFCORE_H_
