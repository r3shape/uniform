#include <include/swarm/swarm.h>

GPUHandle ib;
GPUHandle vb;
GPUHandle vs;
GPUHandle pipe;
u32 elements[] = {1, 2, 3, 4, 5, 6};
f32 vertices[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

void init() {
    ib = swarmRenderer->createBuffer(GPU_BUFFER_ELEMENT, sizeof(u32)*6, elements);
    vb = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, sizeof(f32)*9, vertices);
    vs = swarmRenderer->createProgram("vs.glsl", "fs.glsl");
    pipe = swarmRenderer->createPipeline(vs, 0x1);
}

void update(float dt) {
    if (swarmInputs->keyIsDown(SWARM_KEY_A)) saneLog->log(SANE_LOG_SUCCESS, "[Game] Pressed A");
    if (swarmInputs->keyIsDown(SWARM_KEY_D)) saneLog->log(SANE_LOG_SUCCESS, "[Game] Pressed D");
}

void render() {
    GPUHandle frame = swarmRenderer->createFrame();
    GPUHandle depthPhase = swarmRenderer->createPhase(GPU_PHASE_DEPTH, (Vec3){1, 0, 0}, (Vec3){1, 1, 1}, 0);
    // GPUHandle opaquePhase = swarmRenderer->createPhase(GPU_PHASE_OPAQUE, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);
    // GPUHandle shadowPhase = swarmRenderer->createPhase(GPU_PHASE_SHADOW, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);

    swarmRenderer->createCall((GPUNode){
        .phase = depthPhase,
        .vertexBuffer = vb,
        .elementBuffer = ib,
        .pipeline = pipe
    });
        
    swarmRenderer->commitFrame(frame);
}

void shutdown() {
    saneLog->log(SANE_LOG_SUCCESS, "[Game] Shutdown");
}

void export(RuntimeAPI* api) {
    api->configure((RuntimeConfig){
        .title = NULL,
        .windowPos = {100, 100},
        .windowSize = {800, 600},
        .backend = GPU_BACKEND_OPENGL
    });
    api->init = init;
    api->update = update;
    api->render = render;
    api->shutdown = shutdown;
}