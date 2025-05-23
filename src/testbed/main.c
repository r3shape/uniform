#include <include/swarm/swarm.h>

GPUHandle depth;
GPUHandle opaque;
GPUHandle program;
GPUHandle program2;
GPUHandle tBuffer;
GPUHandle tBuffer2;
GPUHandle vBuffer;
GPUHandle eBuffer;
GPUHandle vBuffer2;
GPUHandle eBuffer2;
GPUHandle pipeline;
GPUHandle pipeline2;

u32 triElements[] = {0, 1, 2};
Vec3 u_color = {1.0, 0.0, 1.0};
f32 triangle[] = {
    -0.5, -0.5, 0.0,  0.23, 0.72, 0.54,   0.0, 0.0,
     0.5, -0.5, 0.0,  0.72, 0.54, 0.23,   1.0, 0.0,
     0.0,  0.5, 0.0,  0.54, 0.23, 0.72,   0.5, 1.0
};

u32 quadElements[] = {0, 1, 2, 2, 3, 0};
f32 quad[] = {
    -0.5, -0.5, 0.0,  0.23, 0.72, 0.54,  0.0, 0.0,
     0.5, -0.5, 0.0,  0.72, 0.54, 0.23,  1.0, 0.0,
     0.5,  0.5, 0.0,  0.54, 0.23, 0.72,  1.0, 1.0,
    -0.5,  0.5, 0.0,  0.12, 0.36, 0.42 , 0.0, 1.0
};

void init() {
    // create a vertex buffer
    vBuffer = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, GPU_VERTEX_LOCATION|GPU_VERTEX_COLOR|GPU_VERTEX_TEX, sizeof(triangle), triangle);
    vBuffer2 = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, GPU_VERTEX_LOCATION|GPU_VERTEX_COLOR|GPU_VERTEX_TEX, sizeof(quad), quad);
    
    // create an element buffer
    eBuffer = swarmRenderer->createBuffer(GPU_BUFFER_ELEMENT, 0, sizeof(triElements), triElements);
    eBuffer2 = swarmRenderer->createBuffer(GPU_BUFFER_ELEMENT, 0, sizeof(quadElements), quadElements);
    
    // create a program
    program = swarmRenderer->createProgram(
        "external/assets/shaders/default/shader.vert",
        "external/assets/shaders/default/shader.frag"
    );
    
    program2 = swarmRenderer->createProgram(
        "external/assets/shaders/default/screen.vert",
        "external/assets/shaders/default/screen.frag"
    );
    
    // create a texture buffer
    tBuffer = swarmRenderer->createBuffer(GPU_BUFFER_TEXTURE, GPU_TEXTURE_FORMAT_RGBA, 0, &(GPUTexture){
        .path = "external/assets/textures/r3bow.png",
        .type = GPU_TEXTURE_PLANE, .slot = 0
    });
    tBuffer2 = swarmRenderer->createBuffer(GPU_BUFFER_TEXTURE, GPU_TEXTURE_FORMAT_RGBA, 0, &(GPUTexture){
        .path = "external/assets/textures/r3bow2.png",
        .type = GPU_TEXTURE_PLANE, .slot = 0
    });
    
    // create a pipeline using the program
    pipeline = swarmRenderer->createPipeline(program, 0x1);
    pipeline2 = swarmRenderer->createPipeline(program2, 0x2);
    // configure the pipeline
    swarmRenderer->bindPipelineBuffer(tBuffer, pipeline);
    swarmRenderer->bindPipelineBuffer(tBuffer2, pipeline2);
}

void update(float dt) {
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
    // create a GPU Frame
    swarmRenderer->createFrame();
    
    // create GPU phases
    depth = swarmRenderer->createPhase(GPU_PHASE_DEPTH, 1.0, (Vec3){0});
    opaque = swarmRenderer->createPhase(GPU_PHASE_OPAQUE, 0.0, (Vec3){45.0, 60.0, 85.0});
    
    // create GPU Calls to specific GPU Phases
    swarmRenderer->createCall((GPUNode){
        .phase = depth,
        .pipeline = pipeline,
        .uniforms = 0,
        .vertexBuffer = vBuffer,
        .elementBuffer = eBuffer
    });
    
    swarmRenderer->createCall((GPUNode){
        .phase = opaque,
        .pipeline = pipeline2,
        .vertexBuffer = vBuffer2,
        .elementBuffer = eBuffer2,
        .uniforms = 0
    });
    
    swarmRenderer->createCall((GPUNode){
        .phase = opaque,
        .pipeline = pipeline,
        .vertexBuffer = vBuffer,
        .elementBuffer = eBuffer,
        .uniforms = 1, .uniformv = { {.name = "u_color", .type = GPU_UNIFORM_VEC3, .value.v3 = u_color } }
    });
}    

void shutdown() {
    saneLog->log(SANE_LOG_SUCCESS, "[Game] Shutdown");
}

void export(RuntimeAPI* api) {
    api->configure((RuntimeConfig){
        "Swarm Testbed",
        {100, 100},
        {1200, 800},
        GPU_BACKEND_OPENGL
    });
    api->init = init;
    api->update = update;
    api->render = render;
    api->shutdown = shutdown;
}