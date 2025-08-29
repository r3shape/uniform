#version 460 core

layout(location=0) in vec3 a_location;
layout(location=1) in vec3 a_color;
layout(location=2) in vec2 a_tex_coord;

out vec3 vertex_color;
out vec2 tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
   gl_Position = u_proj * u_view * u_model * vec4(a_location, 1);
   vertex_color = a_color;
   tex_coord = a_tex_coord;
}