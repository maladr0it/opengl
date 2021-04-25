#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
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
in vec3 viewspaceLightPos;

out vec4 fragColor;

void main() {
  // ambient
  vec3 ambient = light.ambient * material.ambient;

  // diffuse
  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(fragPos - viewspaceLightPos);
  float diffuseStrength = max(dot(normal, -lightDir), 0);
  vec3 diffuse = light.diffuse * (diffuseStrength * material.diffuse);

  // specular
  vec3 viewDir = normalize(fragPos);
  vec3 reflectDir = reflect(lightDir, normal);
  float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (specularStrength * material.specular);

  // result
  vec3 result = (ambient + diffuse + specular);
  fragColor = vec4(result, 1.0);
}