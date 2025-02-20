#version 460 core

layout(location = 0) in vec3 a_Location;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 vertex_color;
out vec2 tex_coord;

void main() {
   gl_Position = u_projection * u_view * u_model * vec4(a_Location, 1.0f);
   vertex_color = a_Color;
   tex_coord = a_TCoord;
}
