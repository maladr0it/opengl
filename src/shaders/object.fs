#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Material material;
uniform Light light;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 viewspaceLightPos;

out vec4 fragColor;

void main() {
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragTexCoords));

  // diffuse
  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(fragPos - viewspaceLightPos);
  float diffuseStrength = max(dot(normal, -lightDir), 0);
  vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, fragTexCoords));

  // specular
  vec3 viewDir = normalize(fragPos);
  vec3 reflectDir = reflect(lightDir, normal);
  float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * specularStrength * vec3(texture(material.specular, fragTexCoords));

  // result
  vec3 result = (ambient + diffuse + specular);
  fragColor = vec4(result, 1.0);
}