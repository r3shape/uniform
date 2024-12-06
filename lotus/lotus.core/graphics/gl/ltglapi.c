#include "ltgl.h"
#include "ltglapi.h"
#include "../../utility/lthashmap.h"
#include "../../platform/ltlogger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../vendor/stb/stb_image.h"

LTvertexData ltglVertexDataBuffer(LTvertexBuffer buffer) {
    return ltglVertexData(buffer.nAttributes, buffer.nVertices, buffer.nIndices, buffer.vertices, buffer.indices);
}

LTvertexData ltglVertexData(u32 nAttributes, u32 nVertices, u32 nIndices, f32* vertices, u32* indices) {
    LTvertexData data = {.nVertices=nVertices, .nIndices=nIndices};
    ltglGenVertexArrays(1, &data.vertexArray);
    ltglGenBuffers(1, &data.vertexBuffer);
    
    // Lotus vertex attributes
    // uLocation(3) + uColor(3) + uTCoord(2) + uNormal(2)
    u32 stride = (nAttributes == 1) ? 3  :   \
                 (nAttributes == 2) ? 6  :   \
                 (nAttributes == 3) ? 8  :   \
                 (nAttributes == 4) ? 10 :   3;
    
    u32 vComponents = stride*nVertices;
    
    ltglBindVertexArray(data.vertexArray);
    ltglBindBuffer(LTGL_ARRAY_BUFFER, data.vertexBuffer);
    ltglBufferData(LTGL_ARRAY_BUFFER, vComponents*sizeof(f32), vertices, LTGL_STATIC_DRAW);

    if (indices != NULL && nIndices > 0) {
        ltglGenBuffers(1, &data.elementBuffer);
        ltglBindBuffer(LTGL_ELEMENT_ARRAY_BUFFER, data.elementBuffer);
        ltglBufferData(LTGL_ELEMENT_ARRAY_BUFFER, nIndices*sizeof(u32), indices, LTGL_STATIC_DRAW);
    } else { data.elementBuffer = -1; data.nIndices = 0; }

    // "uLocation" attribute
    ltglVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(f32), (void*)0);
    ltglEnableVertexAttribArray(0);
    
    // "uColor" attribute
    if (nAttributes >= 2) {
        ltglVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(f32), (void*)(3*sizeof(f32)));
        ltglEnableVertexAttribArray(1);
    }
    
    // "uTCoord" attribute
    if (nAttributes >= 3) {
        ltglVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(f32), (void*)(6*sizeof(f32)));
        ltglEnableVertexAttribArray(2);
    }
    
    // "uNormal" attribute
    if (nAttributes >= 4) {
        ltglVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride * sizeof(f32), (void*)(8*sizeof(f32)));
        ltglEnableVertexAttribArray(3);
    }
    
    ltglBindBuffer(LTGL_ARRAY_BUFFER, 0);
    ltglBindVertexArray(0);
    return data;
}

LTshaderProgram ltglShaderProgram(const char* vertexSrc, const char* fragmentSrc) {
    i32 link = 0;
    i32 compile = 0;

    LTshaderProgram program = {.uniforms=ltMakeHashmap(16)};
    if (!program.uniforms) {
        printf("failed to allocate uniform hashmap!\n");
        return (LTshaderProgram){0};
    }

    program.program = ltglCreateProgram();
    i32 vShader = ltglCreateShader(LTGL_VERTEX_SHADER);
    i32 fShader = ltglCreateShader(LTGL_FRAGMENT_SHADER);

    ltglShaderSource(vShader, 1, &vertexSrc, NULL);
    ltglCompileShader(vShader);
    ltglGetShaderiv(vShader, LTGL_COMPILE_STATUS, &compile);
    if (!compile) {
        printf("failed to compile vertex-shader!\n");
        ltDestroyHashmap(program.uniforms);
        return (LTshaderProgram){0};
    }
    
    ltglShaderSource(fShader, 1, &fragmentSrc, NULL);
    ltglCompileShader(fShader);
    ltglGetShaderiv(fShader, LTGL_COMPILE_STATUS, &compile);
    if (!compile) {
        printf("failed to compile fragment-shader!\n");
        ltDestroyHashmap(program.uniforms);
        return (LTshaderProgram){0};
    }

    ltglAttachShader(program.program, vShader);
    ltglAttachShader(program.program, fShader);
    ltglLinkProgram(program.program);
    ltglGetProgramiv(program.program, LTGL_LINK_STATUS, &link);
    if (!link) {
        printf("failed to link shader program!\n");
        ltDestroyHashmap(program.uniforms);
        return (LTshaderProgram){0};
    }

    ltglDeleteShader(vShader);
    ltglDeleteShader(fShader);
    return program;

}

void ltglSetUniform(LTshaderProgram program, LTGLuniformType type, const char* name, void* value) {
    i32 location = ltglGetUniformLocation(program.program, name);
    ltglUseProgram(program.program);
    if (location < 0) return;
    switch (type) {
        case LOTUS_UNIFORM_NONE: break;
        case LOTUS_UNIFORM_TYPES: break;
        case LOTUS_UNIFORM_VEC2: ltglUniform2fv(location, 1, value); break;
        case LOTUS_UNIFORM_VEC3: ltglUniform3fv(location, 1, value); break;
        case LOTUS_UNIFORM_VEC4: ltglUniform4fv(location, 1, value); break;
        case LOTUS_UNIFORM_MAT4: ltglUniformMatrix4fv(location, 1, 0, value); break;
        default: break;
    }; ltSetHashmap(program.uniforms, name, value);
}

LTshaderUniform ltglGetUniform(LTshaderProgram program, const char* name) {
    i32 location = ltglGetUniformLocation(program.program, name);
    if (location < 0) return (LTshaderUniform){0};
    
    void* value = ltGetHashmap(program.uniforms, name);
    if (!value) return (LTshaderUniform){0};
    
    return (LTshaderUniform){.location=location, .name=name, .value=value};
}

LTtexture2D ltglTexture2D(const char* texturePath) {
    LTtexture2D texture = {.width=0, .height=0, .nChannels=0};
    stbi_set_flip_vertically_on_load(LOTUS_TRUE);
    texture.textureData = stbi_load(texturePath, &texture.width, &texture.height, &texture.nChannels, 4);
    if (!texture.textureData) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to load texture from source: %s\n", texturePath);
        return (LTtexture2D){0};
    }
    
    ltglGenTextures(1, &texture.handle);
    ltglBindTexture(GL_TEXTURE_2D, texture.handle);

    // set the texture wrapping parameters
    ltglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    ltglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // set texture filtering parameters
    ltglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    ltglTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ltglTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.textureData);
    ltglGenerateMipmap(GL_TEXTURE_2D);
    
    ltglBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

