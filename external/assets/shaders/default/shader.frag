#version 460 core

in vec3 vertex_color;
in vec2 tex_coord;

out vec4 frag_color;

uniform vec3 u_color;
uniform sampler2D u_texture;

void main() {
    // frag_color = vec4(vertex_color, 1.0);
    // frag_color = vec4(vec3(gl_FragCoord.z), 1.0);
    frag_color = texture(u_texture, tex_coord) * vec4(u_color, 1.0);
    // frag_color = texture(u_texture, tex_coord) * vec4(vertex_color, 1.0);
    // frag_color = texture(u_texture, tex_coord) * vec4(vec3(gl_FragCoord.z), 1.0);
}