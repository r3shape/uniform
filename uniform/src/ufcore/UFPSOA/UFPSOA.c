#include "r3kit/include/defines.h"
#include "r3kit/include/io/log.h"
#include "r3kit/include/mem/alloc.h"
#include <ufcore/UFPSOA.h>

// handle memory layout: [32-bit ID | 15-bit GEN | 16-bit SALT | 1-bit ALIVE]

typedef struct UFPSOAContext {
    // Processes
    UFPSOAProcessLogic* processLogicv;
    u64* processMaskv;
    u8   processes;

    // Structures
    u16* structureStridev;
    u64* structureMaskv;
    
    // array of row slabs (transform structure cols=3|row=0 stride=8 px0, px1, px2 |row=1 stride=4 vx0, vx1, vx2| || new slab)
    ptr* structureRowv;
    
    u8*  structureRows; // number of rows
    u64* structureMap;  // local -> global (handles)
    u64  structures;

    // Handles
    u64* handleMaskv;
    u64* freev;
    u64  free;
    u64  next;

    // Queries
    u64* queryMaskNone;
    u64* queryMaskAny;
    u64* queryMaskAll;
    u16  queries;

    // Iterables
    u16* iterStridev;   // array of stride values per-column, per row
    ptr* iterRowv;      // array of matched row pointers
    u64* iterCols;      // number of columns per-matched row
    u16* iterRows;      // number of matched rows
    u16* iterRow;       // current row
    u64* iterCol;       // current column
    u16  iterables;

    // Configurations
    u64** configStructureMap;   // local -> global (structures)
    u16** configStructures;     // number of masked structures
    u64** configHandleMap;      // local -> global (handles)
    u16** configStridev;        // arrayy of stride values per-row per masked structure
    ptr** configRowv;           // arrau pf row pointers per masked structure
    u16** configRows;           // number of rows per masked structure
    u64** configMask;           // bitmask of structures
    u16 configurations;
} UFPSOAContext;
 
static struct UFPSOAInternal {
    UFPSOAContext* contextv;
    u8 contexts;
} UFPSOAInternal = { .contextv = NULL, .contexts = 0 };


/* CONTEXT API */
UFResource newContext(none) {
    if (!UFPSOAInternal.contextv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Internal state NULL\n");;
        return 0;
    }

    UFPSOAContext* contextPtr = &UFPSOAInternal.contextv[UFPSOAInternal.contexts];

    // Initialize context process fields
    contextPtr->processes = 0;
    contextPtr->processLogicv = r3_mem_alloc(sizeof(UFPSOAProcessLogic) * UF_PSOA_PROCESS_MAX, 8);
    if (!contextPtr->processLogicv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->processMaskv = r3_mem_alloc(sizeof(u64) * UF_PSOA_PROCESS_MAX, 8);
    if (!contextPtr->processMaskv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize context structure fields
    contextPtr->structures = 0;
    contextPtr->structureStridev = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->structureStridev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->structureMaskv = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->structureMaskv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->structureRowv = r3_mem_alloc(sizeof(ptr) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->structureRowv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->structureRows = r3_mem_alloc(sizeof(u8) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->structureRows) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->structureMap = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->structureMap) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize context handle fields
    contextPtr->next = 0;
    contextPtr->free = 0;
    contextPtr->handleMaskv = r3_mem_alloc(sizeof(u64) * UF_PSOA_HANDLE_MAX, 8);
    if (!contextPtr->handleMaskv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->freev = r3_mem_alloc(sizeof(u64) * UF_PSOA_HANDLE_MAX, 8);
    if (!contextPtr->freev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize query fields
    contextPtr->queries = 0;
    contextPtr->queryMaskNone = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->queryMaskNone) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->queryMaskAny = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->queryMaskAny) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->queryMaskAll = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->queryMaskAll) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize iterable fields
    contextPtr->iterables = 0;
    contextPtr->iterStridev = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->iterStridev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->iterRowv = r3_mem_alloc(sizeof(ptr) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->iterRowv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->iterCols = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->iterCols) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->iterRows = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->iterRows) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->iterRow = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->iterRow) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->iterCol = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->iterCol) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize configuration fields
    contextPtr->configurations = 0;
    contextPtr->configStructureMap = r3_mem_alloc(sizeof(u64*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configStructureMap) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->configStructures = r3_mem_alloc(sizeof(u16*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configStructures) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->configHandleMap = r3_mem_alloc(sizeof(u64*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configHandleMap) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->configStridev = r3_mem_alloc(sizeof(u16*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configStridev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->configRowv = r3_mem_alloc(sizeof(ptr*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configRowv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->configRows = r3_mem_alloc(sizeof(u16*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configRows) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    contextPtr->configMask = r3_mem_alloc(sizeof(u64*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!contextPtr->configMask) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // incrememnt then assign
    UFResource resource = ++UFPSOAInternal.contexts;
    return resource;
}

none delContext(UFResource context) {
    if (!context) {
        r3_log_stdout(WARN_LOG, "[UFPSOA] `UFPSOAContext` Avoided double free\n");
        return;
    }

    UFPSOAContext* contextPtr = &UFPSOAInternal.contextv[context];
    if (!contextPtr) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to delete `UFPSOAContext` -- Invalid Pointer\n");
        return;
    }

    r3_mem_dealloc(contextPtr->processMaskv);
    r3_mem_dealloc(contextPtr->processLogicv);

    r3_mem_dealloc(contextPtr->structureStridev);
    r3_mem_dealloc(contextPtr->structureMaskv);
    r3_mem_dealloc(contextPtr->structureRows);
    r3_mem_dealloc(contextPtr->structureRowv);
    r3_mem_dealloc(contextPtr->structureMap);
    
    r3_mem_dealloc(contextPtr->handleMaskv);
    r3_mem_dealloc(contextPtr->freev);

    r3_mem_dealloc(contextPtr->queryMaskNone);
    r3_mem_dealloc(contextPtr->queryMaskAny);
    r3_mem_dealloc(contextPtr->queryMaskAll);

    r3_mem_dealloc(contextPtr->iterStridev);
    r3_mem_dealloc(contextPtr->iterRowv);
    r3_mem_dealloc(contextPtr->iterCols);
    r3_mem_dealloc(contextPtr->iterRows);
    r3_mem_dealloc(contextPtr->iterRow);
    r3_mem_dealloc(contextPtr->iterCol);
    
    r3_mem_dealloc(contextPtr->configStructureMap);
    r3_mem_dealloc(contextPtr->configStructures);
    r3_mem_dealloc(contextPtr->configHandleMap);
    r3_mem_dealloc(contextPtr->configStridev);
    r3_mem_dealloc(contextPtr->configRowv);
    r3_mem_dealloc(contextPtr->configRows);
    r3_mem_dealloc(contextPtr->configMask);

    // swap-last and zero
    UFPSOAInternal.contextv[context - 1] = UFPSOAInternal.contextv[UFPSOAInternal.contexts - 1];
    UFPSOAInternal.contextv[UFPSOAInternal.contexts - 1] = (UFPSOAContext){NULL};
    UFPSOAInternal.contexts--;
}


/* HANDLE API */
UFResource newHandle(none) { return 0; }

UFResource bindHandle(UFResource handle, UFResource structure) { return 0; }

UFResource unbindHandle(UFResource handle, UFResource structure) { return 0; }

none delHandle(UFResource handle) {}


/* QUERY API */
UFResource newQuery(UFPSOAQueryDesc query) { return 0; }

none delQuery(UFResource query) {}

u8 iterNextCol(UFResource query) { return 0; }

u8 iterNextRow(UFResource query) { return 0; }


/* PROCESS API */
UFResource newProcess(UFPSOAProcessDesc process) { return 0; }

none delProcess(UFResource process) {}

none runProcessAll(none) {}

none runProcess(UFResource process) {}

none runProcessQuery(UFResource query, UFResource process) {}

none scheduleProcessMs(f32 ms, UFResource process) {}

none scheduleProcessEv(UFEventCode ev, UFResource process) {}


/* STRUCTURE API */
UFResource newStructure(UFPSOAStructureDesc desc, UFResource ctx) { return 0; }

none delStructure(UFResource structure, UFResource ctx) {}

none setRow(ptr in, UFResource row, UFResource structure, UFResource context) {}

none getRow(ptr out, UFResource row, UFResource structure, UFResource context) {}

none setRowCol(ptr in, UFResource row, UFResource col, UFResource structure, UFResource context) {}

none getRowCol(ptr out, UFResource row, UFResource col, UFResource structure, UFResource context) {}


/* INTERNAL API */
u8 ufInitPSOA(UFPSOAInterface* psoaInterface) {
    if (!psoaInterface) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Invalid `UFPSOAInterface` pointer\n");
        return 0;
    }
    
    UFPSOAInternal.contexts = 0;
    UFPSOAInternal.contextv = r3_mem_alloc(sizeof(UFPSOAContext) * UF_PSOA_CONTEXT_MAX, 8);
    if (!UFPSOAInternal.contextv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to initialize `UFPSOAInterface` -- Out of memory\n");
        return 0;
    }

    psoaInterface->newContext = newContext;
    psoaInterface->delContext = delContext;

    psoaInterface->newHandle = newHandle;
    psoaInterface->delHandle = delHandle;
    psoaInterface->bindHandle = bindHandle;
    psoaInterface->unbindHandle = unbindHandle;
    
    psoaInterface->newQuery = newQuery;
    psoaInterface->delQuery = delQuery;

    psoaInterface->iterNextCol = iterNextCol;
    psoaInterface->iterNextRow = iterNextRow;

    psoaInterface->newProcess = newProcess;
    psoaInterface->delProcess = delProcess;

    psoaInterface->runProcess = runProcess;
    psoaInterface->runProcessAll = runProcessAll;
    psoaInterface->runProcessQuery = runProcessQuery;

    psoaInterface->scheduleProcessMs = scheduleProcessMs;
    psoaInterface->scheduleProcessEv = scheduleProcessEv;

    psoaInterface->newStructure = newStructure;
    psoaInterface->delStructure = delStructure;

    psoaInterface->setRow = setRow;
    psoaInterface->getRow = getRow;
    psoaInterface->setRowCol = setRowCol;
    psoaInterface->getRowCol = getRowCol;

    r3_log_stdout(SUCCESS_LOG, "[UFPSOA] Initialized\n");
    return 1;
}

u8 ufExitPSOA(UFPSOAInterface* psoaInterface) {
    if (!psoaInterface) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Invalid `UFPSOAInterface` pointer\n");
        return 0;
    }

    UFPSOAInternal.contexts = 0;
    r3_mem_dealloc(UFPSOAInternal.contextv);
    
    psoaInterface->newContext = NULL;
    psoaInterface->delContext = NULL;

    psoaInterface->newHandle = NULL;
    psoaInterface->delHandle = NULL;
    psoaInterface->bindHandle = NULL;
    psoaInterface->unbindHandle = NULL;
    
    psoaInterface->newQuery = NULL;
    psoaInterface->delQuery = NULL;

    psoaInterface->iterNextCol = NULL;
    psoaInterface->iterNextRow = NULL;

    psoaInterface->newProcess = NULL;
    psoaInterface->delProcess = NULL;

    psoaInterface->runProcess = NULL;
    psoaInterface->runProcessAll = NULL;
    psoaInterface->runProcessQuery = NULL;

    psoaInterface->scheduleProcessMs = NULL;
    psoaInterface->scheduleProcessEv = NULL;

    psoaInterface->newStructure = NULL;
    psoaInterface->delStructure = NULL;

    psoaInterface->setRow = NULL;
    psoaInterface->getRow = NULL;
    psoaInterface->setRowCol = NULL;
    psoaInterface->getRowCol = NULL;

    r3_log_stdout(SUCCESS_LOG, "[UFPSOA] Deinitialized\n");
    return 1;
}
