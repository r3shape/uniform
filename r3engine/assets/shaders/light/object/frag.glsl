#version 460 core

out vec4 frag_color;

in vec3 normal;
in vec3 frag_location;

uniform vec3 u_light_color;
uniform vec3 u_object_color;
uniform vec3 u_light_location;
uniform vec3 u_cam_location;

// TODO: extract ambient_strength/specular_strength/shininess variables to material fields.
void main() {
   // ambient lighting
   float ambient_strength = 0.1;
   vec3 ambient = ambient_strength * u_light_color;

   // compute light direction vector
   vec3 norm = normalize(normal);
   vec3 light_direction = normalize(u_light_location - frag_location);

   // diffuse lighting
   // compute diffuse impact on the fragment
   float diff = max(dot(norm, light_direction), 0.0);
   vec3 diffuse = diff * u_light_color;

   // specular lighting
   float specular_strength = 0.5;
   vec3 view_direction = normalize(u_cam_location - frag_location);
   
   // negate the light_direction vector as it points from the fragment to the light
   vec3 reflection_direction = reflect(-light_direction, norm);

   // compute specular component
   float shininess = 64.0; // valid from 2 -> 256
   float spec = pow(max(dot(view_direction, reflection_direction), 0.0), shininess);
   vec3 specular = specular_strength * spec * u_light_color;

   vec3 result = (ambient + diffuse + specular) * u_object_color;
   frag_color = vec4(result, 1.0);
}
