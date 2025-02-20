#version 460 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D u_texture;

void main() {
    vec3 light_color = vec3(0.3, 0.5, 1.0);
    
    float ambient_magnitude = 0.1;
    vec3 ambient = ambient_magnitude * light_color;

   frag_color = vec4(ambient, 1.0) * texture(u_texture, tex_coord);
}
