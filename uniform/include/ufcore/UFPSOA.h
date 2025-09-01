#ifndef _UFPSOA_H_
#define _UFPSOA_H_

#define _UF_R3MEM_
#define _UF_R3LOG_
#include <ufcore/UFDEF.h>

#define UF_PSOA_HANDLE_MAX      0xFFFFFF
#define UF_PSOA_CONTEXT_MAX     0x010
#define UF_PSOA_PROCESS_MAX     0x040
#define UF_PSOA_STRUCTURE_MAX   0x040

typedef struct UFPSOAStructureDesc {
    u16* stridev;       // width of each column per-row
    u64 cols;           // number of columns per-row
    u64 mask;           // custom set mask used in structure queries
    u8  rows;           // number of rows
} UFPSOAStructureDesc;

typedef struct UFPSOAQueryDesc {
    u64 maskAll;    // structures that MUST be bound
    u64 maskAny;    // structures that MUST be undbound
    u64 maskNone;   // structures that MAY be bound
} UFPSOAQueryDesc;

typedef none (*UFPSOAProcessLogic)(UFResource iter);

typedef struct UFPSOAProcessDesc {
    u64 mask;
    UFPSOAProcessLogic logic;
} UFPSOAProcessDesc;

typedef struct UFPSOAInterface {
    UFVersion ver;

    // Context
    UF_API_METHOD(UFResource, newContext, none);
    UF_API_METHOD(none, delContext, UFResource ctx);

    // Handle
    UF_API_METHOD(UFResource, newHandle, none);
    UF_API_METHOD(UFResource, bindHandle, UFResource handle, UFResource structure);
    UF_API_METHOD(UFResource, unbindHandle, UFResource handle, UFResource structure);
    UF_API_METHOD(none, delHandle, UFResource handle);

    // Query
    UF_API_METHOD(UFResource, newQuery, UFPSOAQueryDesc query);
    UF_API_METHOD(none, delQuery, UFResource query);

    // Iterable
    UF_API_METHOD(u8, iterNextCol, UFResource query);
    UF_API_METHOD(u8, iterNextRow, UFResource query);

    // Process
    UF_API_METHOD(UFResource, newProcess, UFPSOAProcessDesc process);
    UF_API_METHOD(none, delProcess, UFResource process);
    
    UF_API_METHOD(none, runProcessAll, none);
    UF_API_METHOD(none, runProcess, UFResource process);
    UF_API_METHOD(none, runProcessQuery, UFResource query, UFResource process);
    
    UF_API_METHOD(none, scheduleProcessMs, f32 ms, UFResource process);
    UF_API_METHOD(none, scheduleProcessEv, UFEventCode ev, UFResource process);
 
    // PSOA
    UF_API_METHOD(UFResource, newStructure, UFPSOAStructureDesc desc, UFResource ctx);
    UF_API_METHOD(none, delStructure, UFResource structure, UFResource ctx);
    
    UF_API_METHOD(none, setRow, ptr in, UFResource row, UFResource structure, UFResource context);
    UF_API_METHOD(none, getRow, ptr out, UFResource row, UFResource structure, UFResource context);
    UF_API_METHOD(none, setRowCol, ptr in, UFResource row, UFResource col, UFResource structure, UFResource context);
    UF_API_METHOD(none, getRowCol, ptr out, UFResource row, UFResource col, UFResource structure, UFResource context);
} UFPSOAInterface;

#endif // _UFPSOA_H_
