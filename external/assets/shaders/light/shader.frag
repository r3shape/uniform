#version 460 core

in vec3 normal;
in vec3 fragment;
in vec2 tex_coord;

out vec4 frag_color;

struct Material {
    float shine;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material u_material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 location;
};
uniform Light u_light;

uniform vec3 u_mesh_color;
uniform vec3 u_view_location;

uniform sampler2D u_texture;

void main() {
    vec3 ambient = u_light.ambient * u_material.ambient;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(u_light.location - fragment);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);

    vec3 view_direction = normalize(u_view_location - fragment);
    vec3 reflection_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shine);
    vec3 specular = u_light.specular * (spec * u_material.specular);

    vec3 result = ambient + diffuse + specular;
    // frag_color = texture(u_texture, tex_coord) * vec4(u_mesh_color * result, 1.0);
    frag_color = vec4(u_mesh_color * result, 1.0);
}