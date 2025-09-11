#ifndef _UFGPU_H_
#define _UFGPU_H_

#define _UF_R3DEF_
#include <ufcore/UFDEF.h>

#define UF_GPU_CALL_MAX               KiB
#define UF_GPU_PHASE_MAX              4
#define UF_GPU_RESOURCE_MAX           KiB
#define UF_GPU_PROGRAM_BUFFER_SIZE   (2 * KiB)

#define UF_GPU_CALL_UNIFORM_MAX           8

#define UF_GPU_PHASE_BUFFER_MAX           8
#define UF_GPU_PHASE_UNIFORM_MAX          4

#define UF_GPU_PIPELINE_BUFFER_MAX        8
#define UF_GPU_PIPELINE_UNIFORM_MAX       4

typedef enum UFGPUBufferType {
    UF_GPU_BUFFER_INVALID,
    UF_GPU_BUFFER_TEXTURE,
    UF_GPU_BUFFER_ELEMENT,
    UF_GPU_BUFFER_VERTEX,
    UF_GPU_BUFFER_FRAME,
    UF_GPU_BUFFER_TYPES
} UFGPUBufferType;

typedef enum UFGPUTextureType {
    UF_GPU_TEXTURE_INVALID,
    UF_GPU_TEXTURE_2D,
    UF_GPU_TEXTURE_3D
} UFGPUTextureType;

typedef enum UFGPUTextureFormat {
    UF_GPU_TEXTURE_FORMAT_INVALID,
    UF_GPU_TEXTURE_FORMAT_RGBA        = 0x1908,
    UF_GPU_TEXTURE_FORMAT_RGB         = 0x1907
} UFGPUTextureFormat;

typedef enum UFGPUMode {
    UF_GPU_MODE_INVLAID,
    UF_GPU_MODE_TRIANGLE      = 0x0004,
    UF_GPU_MODE_POINT         = 0x0000,
    UF_GPU_MODE_LINE          = 0x0001,
    UF_GPU_MODE_QUAD          = 0x0007,
} UFGPUMode;

typedef enum UFGPUVertexAttribute {
    UF_GPU_VERTEX_LOCATION_ATTRIBUTE  = 1 << 0,
    UF_GPU_VERTEX_TEXTURE_ATTRIBUTE   = 1 << 1,
    UF_GPU_VERTEX_NORMAL_ATTRIBUTE    = 1 << 2,
    UF_GPU_VERTEX_COLOR_ATTRIBUTE     = 1 << 3,
    UF_GPU_VERTEX_ATTRIBUTES          = 4
} UFGPUVertexAttribute;

typedef enum UFGPUUniformType {
    UF_GPU_UNIFORM_INVALID,
    UF_GPU_UNIFORM_INT,
    UF_GPU_UNIFORM_FLOAT,
    UF_GPU_UNIFORM_VEC2,
    UF_GPU_UNIFORM_VEC3,
    UF_GPU_UNIFORM_VEC4,
    UF_GPU_UNIFORM_MAT4,
    UF_GPU_UNIFORM_TYPES
} UFGPUUniformType;

typedef enum UFGPUProgramType {
    UF_GPU_VERTEX_PROGRAM,
    UF_GPU_COMPUTE_PROGRAM,
    UF_GPU_GEOMETRY_PROGRAM,
    UF_GPU_PROGRAM_TYPES
} UFGPUProgramType;

typedef enum UFGPUPhaseType {
    UF_GPU_PHASE_INVALID,
    UF_GPU_PHASE_OPAQUE,
    UF_GPU_PHASE_SHADOW,
    UF_GPU_PHASE_LIGHT,
    UF_GPU_PHASE_DEPTH,
    UF_GPU_PHASE_POST,
    UF_GPU_PHASE_TYPES
} UFGPUPhaseType;

typedef struct UFGPUElementBufferDesc { const u32* indexData; u32 indexCount; u32 size; } UFGPUElementBufferDesc;
typedef struct UFGPUFrameBufferDesc { u32 width,height; u32 colorAttachments; u8 hasDepth; } UFGPUFrameBufferDesc;
typedef struct UFGPUVertexBufferDesc { const float* vertexData; u32 vertexCount; u32 size; u8 attribMask; } UFGPUVertexBufferDesc;
typedef struct UFGPUTextureBufferDesc { const void* data; const char* path; u32 width,height,channels,slot,format; } UFGPUTextureBufferDesc;

typedef struct UFGPUBufferDesc {
    union {
        UFGPUFrameBufferDesc frame;
        UFGPUVertexBufferDesc vertex;
        UFGPUElementBufferDesc element;
        UFGPUTextureBufferDesc texture;
    };
    UFGPUBufferType type;
} UFGPUBufferDesc;

typedef struct UFGPUUniformDesc {
    union data {
        f32 f32;
        Vec2 vec2;
        Vec3 vec3;
        Vec4 vec4;
        Mat4 mat4;
    } data;
    CString name;
    UFGPUUniformType type;
} UFGPUUniformDesc;

typedef struct UFGPUVertexProgramDesc {
    CString vertexPath;
    CString fragmentPath;
} UFGPUVertexProgramDesc;

typedef struct UFGPUComputeProgramDesc {
    CString vertexPath;
    CString fragmentPath;
} UFGPUComputeProgramDesc;

typedef struct UFGPUGeometryProgramDesc {
    CString vertexPath;
    CString fragmentPath;
} UFGPUGeometryProgramDesc;

typedef struct UFGPUProgramDesc {
    union {
        UFGPUVertexProgramDesc vertex;
        UFGPUComputeProgramDesc compute;
        UFGPUGeometryProgramDesc geometry;
    };
    UFGPUProgramType type;
} UFGPUProgramDesc;

typedef struct UFGPUPhaseDesc {
    UFGPUUniformDesc uniformv[UF_GPU_PHASE_UNIFORM_MAX];
    UFGPUPhaseType type;
    u8 uniforms;
} UFGPUPhaseDesc;

typedef struct UFGPUPipelineDesc {
    UFGPUUniformDesc uniformv[UF_GPU_PIPELINE_UNIFORM_MAX];
    UFResource bufferv[UF_GPU_PIPELINE_BUFFER_MAX];
    UFResource program;
    u8 uniforms;
    u8 buffers;
} UFGPUPipelineDesc;

// UFGPUCalls dispatch:
// during its configured UFGPUPhase
// sending its configured element/vertex buffers
// through its configured UFGPUPipeline
typedef struct UFGPUCall {
    UFGPUUniformDesc uniformv[UF_GPU_CALL_UNIFORM_MAX];
    UFResource elementBuffer;
    UFResource vertexBuffer;
    UFResource pipeline;
    UFResource phase;
    u8 uniforms;
} UFGPUCall;

typedef struct UFGPUFrame {
    u8 phases;                              // number of phases executed during a frame
    u32 calls;                              // number of calls submitted during a frame
    UFResource buffer;                      // a resource handle to a buffer used for color sampling during a frame
    Arena_Allocator arena;                  // arena allocator for small allocs during a frame
    UFGPUCall callv[UF_GPU_CALL_MAX];       // array of calls submitted during a frame
    UFResource phasev[UF_GPU_PHASE_MAX];    // array of phases executed during a frame
} UFGPUFrame;

typedef struct UFGPUInterface {
    UFGpuApi api;
    UFGpuCtx ctx;
    
    // Programs
    UF_API_METHOD(UFResource, newProgram, UFGPUProgramDesc program);
    UF_API_METHOD(u8, bindProgram, UFResource program);
    UF_API_METHOD(u8, delProgram, UFResource program);
    
    // Uniforms
    // backend may support setting uniforms by index/slot
    UF_API_METHOD(none, setUniformInt, UFResource program, s32 slot, void* data);
    UF_API_METHOD(none, sendUniformInt, UFResource program, s32 slot);

    // backend may support setting uniforms by string alias
    UF_API_METHOD(none, setUniformStr, UFResource program, char* alias, void* data);
    UF_API_METHOD(none, sendUniformStr, UFResource program, char* alias);
    
    // Buffers
    UF_API_METHOD(none, clearDepthBuffer, f32 depth);
    UF_API_METHOD(none, clearColorBuffer, Vec4 color);

    UF_API_METHOD(UFResource, newBuffer, UFGPUBufferDesc buffer);
    UF_API_METHOD(none, delBuffer, UFResource buffer);
    
    UF_API_METHOD(none, bindBuffer, UFResource buffer);
    // 'draw' equivalent for vertex/element buffers
    UF_API_METHOD(none, readBuffer, UFResource buffer);
    // 'update' equivalent
    UF_API_METHOD(none, writeBuffer, u64 size, ptr data, UFResource buffer);
} UFGPUInterface;

#endif // _UFGPU_H_
