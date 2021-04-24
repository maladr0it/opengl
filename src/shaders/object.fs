#version 330 core

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

in vec3 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

void main() {
  // ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(fragPos - lightPos);
  float diffuseStrength = max(dot(normal, -lightDir), 0);
  vec3 diffuse = diffuseStrength * lightColor;

  // specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(cameraPos - fragPos);
  vec3 reflectDir = reflect(lightDir, normal);
  float specularVal = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * specularVal * lightColor;


  vec3 result = (ambient + diffuse + specular) * objectColor;
  fragColor = vec4(result, 1.0);
}