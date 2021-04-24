#version 330 core

uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 viewspaceLightPos;

void main() {
  fragPos = vec3(view * model * vec4(vertPos, 1.0));
  // inversing a matrix is expensive, and only needs to be calculated once per model
  // ideally do it on the cpu and pass it as a uniform
  fragNormal = mat3(transpose(inverse(view * model))) * vertNormal;
  viewspaceLightPos = vec3(view * vec4(lightPos, 1.0));

  gl_Position = projection * view * model * vec4(vertPos, 1.0);
}

