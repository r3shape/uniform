#version 460 core

out vec4 frag_color;

in vec3 normal;
in vec3 frag_location;

struct Material {
   float shine;
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
}; uniform Material u_material;

struct Light {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   vec3 location;
}; uniform Light u_light;

uniform vec3 u_cam_location;

void main() {
   vec3 norm = normalize(normal);
   vec3 view_direction = normalize(u_cam_location - frag_location);

   // ambient lighting
   vec3 ambient = u_light.ambient * u_material.ambient;

   // diffuse lighting
   vec3 light_direction = normalize(u_light.location - frag_location);  // compute light direction vector
   float diff = max(dot(norm, light_direction), 0.0);                   // compute diffuse impact on the fragment
   vec3 diffuse = u_light.diffuse * (diff * u_material.diffuse);

   // specular lighting
   vec3 reflection_direction = reflect(-light_direction, norm);         // negate the light_direction vector as it points from the fragment to the light
   float spec = pow(max(dot(view_direction, reflection_direction), 0.0), u_material.shine);
   vec3 specular = u_light.specular * (spec * u_material.specular);

   vec3 result = ambient + diffuse + specular;
   frag_color = vec4(result, 1.0);
}
