
#version 460 core

in vec2 tex_coord;
in vec3 vertex_color;

out vec4 frag_color;

uniform sampler2D u_texture;

void main() {
    frag_color = texture(u_texture, tex_coord) * vec4(vertex_color, 1.0);
}