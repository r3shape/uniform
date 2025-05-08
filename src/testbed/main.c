#include <include/swarm/swarm.h>

GPUHandle eb;
GPUHandle vb;
GPUHandle vs;
GPUHandle pipe;
Vec3 u_color = {1.0, 0.0, 1.0};
u32 elements[] = {0, 1, 2};
f32 vertices[] = {
    -0.5, -0.5, 0.0,//  0.23, 0.72, 0.54,//   0.0, 0.0,
     0.5, -0.5, 0.0,//  0.72, 0.54, 0.23,//   1.0, 0.0,
     0.0,  0.5, 0.0,//  0.54, 0.23, 0.72,//   0.5, 1.0
};

void init() {
    vb = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, GPU_VERTEX_LOCATION, sizeof(vertices), vertices);
    eb = swarmRenderer->createBuffer(GPU_BUFFER_ELEMENT, 0, sizeof(elements), elements);
    vs = swarmRenderer->createProgram(
        saneFile->read("external/assets/shaders/default/shader.vert", 0),
        saneFile->read("external/assets/shaders/default/shader.frag", 0));
    pipe = swarmRenderer->createPipeline(vs, 0x1);
}

void update(float dt) {
    if (swarmInputs->keyIsDown(SWARM_KEY_A)) saneLog->log(SANE_LOG_SUCCESS, "[Game] Pressed A");
    if (swarmInputs->keyIsDown(SWARM_KEY_D)) saneLog->log(SANE_LOG_SUCCESS, "[Game] Pressed D");
    if (swarmInputs->keyIsDown(SWARM_KEY_R)) u_color.x += 0.1;
    if (swarmInputs->keyIsDown(SWARM_KEY_G)) u_color.y += 0.1;
    if (swarmInputs->keyIsDown(SWARM_KEY_B)) u_color.z += 0.1;

    u_color.x -= 0.01;
    u_color.y -= 0.01;
    u_color.z -= 0.01;

    u_color.x = SSDK_CLAMP(u_color.x, 0.0, 1.0);
    u_color.y = SSDK_CLAMP(u_color.y, 0.0, 1.0);
    u_color.z = SSDK_CLAMP(u_color.z, 0.0, 1.0);
}

void render() {
    GPUHandle frame = swarmRenderer->createFrame();
    // GPUHandle depthPhase = swarmRenderer->createPhase(GPU_PHASE_DEPTH, (Vec3){45.0, 60.0, 85.0}, (Vec3){1, 1, 1}, 0);
    GPUHandle opaquePhase = swarmRenderer->createPhase(GPU_PHASE_OPAQUE, (Vec3){45.0, 60.0, 85.0}, (Vec3){1.0, 1.0, 1.0}, 0);
    // GPUHandle shadowPhase = swarmRenderer->createPhase(GPU_PHASE_SHADOW, (Vec3){45.0, 60.0, 85.0}, (Vec3){0, 0, 0}, 0);

    swarmRenderer->createCall((GPUNode){
        .phase = opaquePhase,
        .vertexBuffer = vb,
        .elementBuffer = eb,
        .pipeline = pipe,
        .uniforms = 1, .uniformv = {
            (GPUUniform){
                .name = "u_color",
                .type = GPU_UNIFORM_VEC3,
                .value.v3 = u_color
            }
        }
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