#version 460 core

layout(location=0) in vec3 a_location;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_location, 1);
}