#ifndef _UFCORE_H_
#define _UFCORE_H_

#define _UF_R3DEF_
#include <ufcore/UFOS.h>
#include <ufcore/UFMRG.h>
#include <ufcore/UFEVIN.h>
#include <ufcore/UFPSOA.h>

UF_API u8 ufInitOS(UFOSInterface* osInterface);
UF_API u8 ufExitOS(UFOSInterface* osInterface);

UF_API u8 ufInitMRG(UFGpuApi api, UFGPUInterface* gpuPtr, UFMRGInterface* mrgInterface);
UF_API u8 ufExitMRG(UFMRGInterface* mrgInterface);

UF_API u8 ufInitGPU(UFGPUInterface* gpuInterface);
UF_API u8 ufExitGPU(UFGPUInterface* gpuInterface);

UF_API u8 ufInitEVIN(UFEVINInterface* evinInterface);
UF_API u8 ufExitEVIN(UFEVINInterface* evinInterface);

UF_API u8 ufInitPSOA(UFPSOAInterface* psoaInterface);
UF_API u8 ufExitPSOA(UFPSOAInterface* psoaInterface);

#endif //_UFCORE_H_
