#define _UF_R3DEF_
#include <ufcore/UFCORE.h>
#include <unicorn/unicorn.h>

struct Unicorn {
    s32 version;

    // UFOSInterface os;
    // UFGPUInterface gpu;
    // UFMRGInterface mrg;
    // UFEVINInterface evin;

    // UFLibrary gpuLibrary;
    // UFLibrary appLibrary;
    UFApplication application;
} Unicorn = {.version=123};

none ufInitRuntime(char* path) { return; }

s32 main() {
    return 0;
}
