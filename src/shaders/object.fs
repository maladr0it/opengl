#version 330 core

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
  vec3 lightDir = normalize(lightPos - fragPos);
  float diffuseStrength = max(dot(normal, lightDir), 0);
  vec3 diffuse = diffuseStrength * lightColor;

  vec3 result = (ambient + diffuse) * objectColor;
  fragColor = vec4(result, 1.0);
}