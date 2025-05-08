#define SWARM_CORE
#include <include/swarm/swarm.h>

int main() {
    swarmInit(GPU_BACKEND_OPENGL);
    
    swarmPlatform->createWindow("Some Window", 800, 600);
    
    GPUHandle ib = swarmRenderer->createBuffer(GPU_BUFFER_ELEMENT, sizeof(u32)*6, (u32[]){1, 2, 3, 4, 5, 6});
    GPUHandle vb = swarmRenderer->createBuffer(GPU_BUFFER_VERTEX, sizeof(f32)*9, (f32[]){1, 2, 3, 4, 5, 6, 7, 8, 9});
    GPUHandle vs = swarmRenderer->createProgram("vs.glsl", "fs.glsl");
    GPUHandle pipe = swarmRenderer->createPipeline(vs, 0x1);
    
    u32 frames = 1;
    do {
        swarmPlatform->pollEvents();
        GPUHandle frame = swarmRenderer->createFrame();
        GPUHandle depthPhase = swarmRenderer->createPhase(GPU_PHASE_DEPTH, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);
        // GPUHandle opaquePhase = swarmRenderer->createPhase(GPU_PHASE_OPAQUE, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);
        // GPUHandle shadowPhase = swarmRenderer->createPhase(GPU_PHASE_SHADOW, (Vec3){0, 0, 0}, (Vec3){0, 0, 0}, 0);

        swarmRenderer->createCall((GPUNode){
            .phase = depthPhase,
            .vertexBuffer = vb,
            .elementBuffer = ib,
            .pipeline = pipe
        });
        
        swarmRenderer->commitFrame(frame);
        swarmRenderer->render();
        frames--;
    } while (frames);
    
    swarmShutdown();
    return 0;
}
