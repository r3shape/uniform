#version 460 core

layout(location=0) in vec3 a_location;
layout(location=2) in vec2 a_tex_coord;
layout(location=3) in vec3 a_normal;

out vec3 normal;
out vec3 fragment;
out vec2 tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_location, 1);
    fragment = mat3(u_model) * a_location;
    normal = a_normal;
    tex_coord = a_tex_coord;
}