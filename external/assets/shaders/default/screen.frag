#version 460 core

in vec2 v_tex;
out vec4 f_color;

uniform sampler2D u_tex;

void main() {
    f_color = texture(u_tex, v_tex);
}
