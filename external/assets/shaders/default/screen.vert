#version 460 core

layout(location=0) in vec3 a_loc;
layout(location=2) in vec2 a_tex;

out vec2 v_tex;

void main() {
    gl_Position = vec4(a_loc, 1.0);
    v_tex = a_tex;
}
