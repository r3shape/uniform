#include "r3kit/include/defines.h"
#include "r3kit/include/io/log.h"
#include "r3kit/include/mem/alloc.h"
#include <ufcore/UFPSOA.h>

// handle memory layout: [32-bit ID | 15-bit GEN | 16-bit SALT | 1-bit ALIVE]

static struct UFPSOAInternal {
    // Processes
    UFPSOAProcessLogic* processLogicv;
    u64* processMaskv;
    u8   processes;

    // Handles
    u64* handleMaskv;
    u64* freev;
    u64  free;
    u64  next;

    // Structures
    u64* structureHandleMap;    // global -> local (structures)
    u16** structureStridev;     // array of structure stride arrays structureStridev[structure][field] = stride
    u64* structureMaskv;        // array of structure masks
    ptr* structureRowv;         // array of row slabs (transform structure cols=3|row=0 stride=8 px0, px1, px2 |row=1 stride=4 vx0, vx1, vx2| || new slab)
    u8*  structureRows;         // number of rows
    u8*  structureCols;         // number of cols
    u64* structureMap;          // local -> global (structures)
    u64  structures;

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
} UFPSOAInternal = {NULL};

/* HANDLE API */


/* HANDLE API */
UFResource newHandle(none) {
    UFResource resource;
    if (UFPSOAInternal.free != 0) {
        resource = UFPSOAInternal.freev[UFPSOAInternal.free - 1];
        UFPSOAInternal.free--;
    } else { resource = ++UFPSOAInternal.next; } // increment then assign (preserve 0 as invalid handle)

    UFPSOAInternal.handleMaskv[resource - 1] = 0;

    return resource;
}

UFResource bindHandle(UFResource handle, UFResource structure) { return 0; }

UFResource unbindHandle(UFResource handle, UFResource structure) { return 0; }

none delHandle(UFResource handle) {
    if (!handle || handle > UFPSOAInternal.next) {
        r3_log_stdoutf(ERROR_LOG, "[UFPSOA] Invalid handle passed to `delHandle` -- %llu\n", handle);
        return;
    }

    if ((UFPSOAInternal.handleMaskv[handle - 1] & UF_HANDLE_INACTIVE) == UF_HANDLE_INACTIVE) {
        r3_log_stdoutf(WARN_LOG, "[UFPSOA] Handle already marked inactive -- %llu\n", handle);
        return;
    } else {
        // manage flags and push the handle back to the free pool
        UFPSOAInternal.handleMaskv[handle - 1] |= UF_HANDLE_INACTIVE;
        UFPSOAInternal.handleMaskv[handle - 1] &= ~UF_HANDLE_ACTIVE;
        UFPSOAInternal.freev[UFPSOAInternal.free++] = handle;

        r3_log_stdoutf(SUCCESS_LOG, "[UFPSOA] Handle marked inactive -- %llu\n", handle);
        return;
    }
}


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
    
    // Initialize context process fields
    UFPSOAInternal.processes = 0;
    UFPSOAInternal.processLogicv = r3_mem_alloc(sizeof(UFPSOAProcessLogic) * UF_PSOA_PROCESS_MAX, 8);
    if (!UFPSOAInternal.processLogicv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.processMaskv = r3_mem_alloc(sizeof(u64) * UF_PSOA_PROCESS_MAX, 8);
    if (!UFPSOAInternal.processMaskv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize context structure fields
    UFPSOAInternal.structures = 0;
    UFPSOAInternal.structureStridev = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.structureStridev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.structureMaskv = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.structureMaskv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.structureRowv = r3_mem_alloc(sizeof(ptr) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.structureRowv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.structureRows = r3_mem_alloc(sizeof(u8) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.structureRows) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.structureMap = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.structureMap) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize context handle fields
    UFPSOAInternal.next = 0;
    UFPSOAInternal.free = 0;
    UFPSOAInternal.handleMaskv = r3_mem_alloc(sizeof(u64) * UF_PSOA_HANDLE_MAX, 8);
    if (!UFPSOAInternal.handleMaskv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.freev = r3_mem_alloc(sizeof(u64) * UF_PSOA_HANDLE_MAX, 8);
    if (!UFPSOAInternal.freev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize iterable fields
    UFPSOAInternal.iterables = 0;
    UFPSOAInternal.iterStridev = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.iterStridev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.iterRowv = r3_mem_alloc(sizeof(ptr) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.iterRowv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.iterCols = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.iterCols) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.iterRows = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.iterRows) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.iterRow = r3_mem_alloc(sizeof(u16) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.iterRow) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.iterCol = r3_mem_alloc(sizeof(u64) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.iterCol) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    // Initialize configuration fields
    UFPSOAInternal.configurations = 0;
    UFPSOAInternal.configStructureMap = r3_mem_alloc(sizeof(u64*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configStructureMap) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.configStructures = r3_mem_alloc(sizeof(u16*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configStructures) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.configHandleMap = r3_mem_alloc(sizeof(u64*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configHandleMap) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.configStridev = r3_mem_alloc(sizeof(u16*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configStridev) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.configRowv = r3_mem_alloc(sizeof(ptr*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configRowv) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.configRows = r3_mem_alloc(sizeof(u16*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configRows) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

    UFPSOAInternal.configMask = r3_mem_alloc(sizeof(u64*) * UF_PSOA_STRUCTURE_MAX, 8);
    if (!UFPSOAInternal.configMask) {
        r3_log_stdout(ERROR_LOG, "[UFPSOA] Failed to create `UFPSOAContext` -- Out Of Memory\n");
        return 0;
    }

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

    r3_mem_dealloc(UFPSOAInternal.processMaskv);
    r3_mem_dealloc(UFPSOAInternal.processLogicv);

    r3_mem_dealloc(UFPSOAInternal.structureStridev);
    r3_mem_dealloc(UFPSOAInternal.structureMaskv);
    r3_mem_dealloc(UFPSOAInternal.structureRows);
    r3_mem_dealloc(UFPSOAInternal.structureRowv);
    r3_mem_dealloc(UFPSOAInternal.structureMap);
    
    r3_mem_dealloc(UFPSOAInternal.handleMaskv);
    r3_mem_dealloc(UFPSOAInternal.freev);

    r3_mem_dealloc(UFPSOAInternal.iterStridev);
    r3_mem_dealloc(UFPSOAInternal.iterRowv);
    r3_mem_dealloc(UFPSOAInternal.iterCols);
    r3_mem_dealloc(UFPSOAInternal.iterRows);
    r3_mem_dealloc(UFPSOAInternal.iterRow);
    r3_mem_dealloc(UFPSOAInternal.iterCol);
    
    r3_mem_dealloc(UFPSOAInternal.configStructureMap);
    r3_mem_dealloc(UFPSOAInternal.configStructures);
    r3_mem_dealloc(UFPSOAInternal.configHandleMap);
    r3_mem_dealloc(UFPSOAInternal.configStridev);
    r3_mem_dealloc(UFPSOAInternal.configRowv);
    r3_mem_dealloc(UFPSOAInternal.configRows);
    r3_mem_dealloc(UFPSOAInternal.configMask);
    
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
