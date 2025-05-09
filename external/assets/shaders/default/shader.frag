#version 460 core

in vec3 vertex_color;
out vec4 frag_color;

uniform vec3 u_color;

void main() {
    frag_color = vec4(u_color, 1.0);
}