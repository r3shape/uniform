#include <include/swarm/swarm.h>

GPUHandle eb;
GPUHandle vb;
GPUHandle vs;
GPUHandle pipe;
u32 elements[] = {0, 1, 2};
f32 vertices[] = {
    -0.5, -0.5, 0.0,//  color.x, color.y, color.z,   0.0, 0.0,
     0.5, -0.5, 0.0,//  color.x, color.y, color.z,   1.0, 0.0,
     0.0,  0.5, 0.0,//  color.x, color.y, color.z,   0.5, 1.0
};

void init() {
    vb = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, sizeof(vertices), vertices);
    eb = swarmRenderer->createBuffer(GPU_BUFFER_ELEMENT, sizeof(elements), elements);
    vs = swarmRenderer->createProgram(
        saneFile->read("external/assets/shaders/default/shader.vert", 0),
        saneFile->read("external/assets/shaders/default/shader.frag", 0));
    pipe = swarmRenderer->createPipeline(vs, 0x1);
}

void update(float dt) {
    if (swarmInputs->keyIsDown(SWARM_KEY_A)) saneLog->log(SANE_LOG_SUCCESS, "[Game] Pressed A");
    if (swarmInputs->keyIsDown(SWARM_KEY_D)) saneLog->log(SANE_LOG_SUCCESS, "[Game] Pressed D");
}

void render() {
    GPUHandle frame = swarmRenderer->createFrame();
    // GPUHandle depthPhase = swarmRenderer->createPhase(GPU_PHASE_DEPTH, (Vec3){45.0, 60.0, 85.0}, (Vec3){1, 1, 1}, 0);
    GPUHandle opaquePhase = swarmRenderer->createPhase(GPU_PHASE_OPAQUE, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);
    // GPUHandle shadowPhase = swarmRenderer->createPhase(GPU_PHASE_SHADOW, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);

    swarmRenderer->createCall((GPUNode){
        .phase = opaquePhase,
        .vertexBuffer = vb,
        .elementBuffer = 0,
        .pipeline = pipe
    });
        
    swarmRenderer->commitFrame(frame);
}

void shutdown() {
    saneLog->log(SANE_LOG_SUCCESS, "[Game] Shutdown");
}

void export(RuntimeAPI* api) {
    api->configure((RuntimeConfig){
        "Swarm Testbed",
        {100, 100},
        {800, 600},
        GPU_BACKEND_OPENGL
    });
    api->init = init;
    api->update = update;
    api->render = render;
    api->shutdown = shutdown;
}