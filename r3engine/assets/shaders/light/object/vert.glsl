#version 460 core

layout(location = 0) in vec3 a_Location;
layout(location = 3) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 normal;
out vec3 frag_location;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(a_Location, 1.0f);
    
    // transform vertex location from local to world space
    frag_location = vec3(u_model * vec4(a_Location, 1.0));

    // TODO: move the calculation of the normal-matrix to the CPU and pass via uniform
    normal = vec3(transpose(inverse(u_model)) * vec4(a_normal, 0.0));
}
