#include <include/swarm/runtime/swarm_runtime.h>
#include <include/swarm/core/swarm_platform.h>
#include <include/swarm/swarm.h>

struct _RuntimeInternal {
    RuntimeConfig config;
    byte running;
} _RuntimeInternal = {0};

none _configureRuntime(RuntimeConfig config) {
    _RuntimeInternal.config.title = (!config.title) ? "Swarm Engine" : config.title;

    _RuntimeInternal.config.backend = (
        config.backend == GPU_BACKEND_INVALID) ? GPU_BACKEND_OPENGL : config.backend;

    _RuntimeInternal.config.windowPos = (
        config.windowPos.x < 0 || config.windowPos.y < 0
    ) ? (Vec2){100, 100} : config.windowPos;

    _RuntimeInternal.config.windowSize = (
        !config.windowSize.x || !config.windowSize.y
    ) ? (Vec2){800, 600} : config.windowSize;

    _RuntimeInternal.running = SSDK_TRUE;
}

RuntimeAPI _RuntimeExport = { .configure = _configureRuntime };
byte _initRuntime(cstr path) {
    SwarmLib lib = swarmPlatform->loadLib(path, "game");
    if (!lib.handle) {
        saneLog->log(SANE_LOG_ERROR, "[Runtime] Failed to load game.dll");
        return SSDK_FALSE;
    } else saneLog->log(SANE_LOG_SUCCESS, "[Runtime] Loaded game.dll");

    RuntimeExportFunc export = swarmPlatform->loadSymbol("export", &lib);
    if (!export) {
        saneLog->log(SANE_LOG_ERROR, "[Runtime] Missing 'export' in game.dll");
        return SSDK_FALSE;
    } else saneLog->log(SANE_LOG_SUCCESS, "[Runtime] RuntimeAPI Exported");

    export(&_RuntimeExport);

    if (!_RuntimeExport.init || !_RuntimeExport.update
        || !_RuntimeExport.render || !_RuntimeExport.shutdown) {
        saneLog->log(SANE_LOG_ERROR, "[Runtime] Incomplete RuntimeAPI");
        return SSDK_FALSE;
    } else saneLog->log(SANE_LOG_SUCCESS, "[Runtime] RuntimeAPI Complete");

    return SSDK_TRUE;
}

none _initCore(none) {
    ssdkInitMemory();
    ssdkInitMath();
    ssdkInitFile();
    ssdkInitLog();
    ssdkInitDS();

    swarmEvents->init();
    swarmInputs->init();
    swarmPlatform->init();
}

none _shutdownCore(none) {
    swarmPlatform->shutdown();
    swarmInputs->shutdown();
    swarmEvents->shutdown();
    
    ssdkExitDS();
    ssdkExitLog();
    ssdkExitFile();
    ssdkExitMath();
    ssdkExitMemory();
}

int main() {
    _initCore();
    
    if (!_initRuntime("external/runtime")) {
        saneLog->log(SANE_LOG_ERROR, "[Runtime] Failed to load Game Library");
        _shutdownCore();
        return 1;
    } else saneLog->log(SANE_LOG_SUCCESS, "[Runtime] Initialized");

    swarmPlatform->createWindow(
        _RuntimeInternal.config.title,
        (u32)_RuntimeInternal.config.windowSize.x,
        (u32)_RuntimeInternal.config.windowSize.y,
        _RuntimeInternal.config.backend
    );
    
    _RuntimeExport.init();
    do {
        swarmPlatform->pollEvents();
        swarmPlatform->pollInputs();
        
        if (swarmInputs->keyIsDown(SWARM_KEY_ESCAPE)) {
            _RuntimeInternal.running = SSDK_FALSE;
        }
        
        _RuntimeExport.update(1.0);
        _RuntimeExport.render();
        
        swarmRenderer->render();
        swarmPlatform->swapBuffers();
    } while(_RuntimeInternal.running);
    _RuntimeExport.shutdown();
    
    _shutdownCore();
    return 0;
}
