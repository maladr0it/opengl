#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

struct Light {
  // vec3 pos;
  vec3 dir;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 fragColor;

void main() {
  // ambient
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragTexCoords));

  // diffuse
  vec3 normal = normalize(fragNormal);
  // vec3 lightDir = normalize(fragPos - light.pos);
  vec3 lightDir = normalize(light.dir);
  float diffuseStrength = max(dot(normal, -lightDir), 0);
  vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, fragTexCoords));

  // specular
  vec3 viewDir = normalize(fragPos - viewPos);
  vec3 reflectDir = reflect(lightDir, normal);
  float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * specularStrength * vec3(texture(material.specular, fragTexCoords));

  // emission
  vec3 emission = vec3(texture(material.emission, fragTexCoords)) * floor(vec3(1.0) - vec3(texture(material.specular, fragTexCoords)));

  // result
  vec3 result = (ambient + diffuse + specular);
  fragColor = vec4(result, 1.0);
}