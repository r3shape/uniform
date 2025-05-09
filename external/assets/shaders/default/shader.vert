#version 460 core

layout(location=0) in vec3 a_location;
layout(location=1) in vec3 a_color;

out vec3 vertex_color;


void main() {
   gl_Position = vec4(a_location, 1);
   vertex_color = a_color;
}