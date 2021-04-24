#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;

out vec3 fragPos;
out vec3 fragNormal;

void main() {
  fragPos = vec3(model * vec4(vertPos, 1.0));
  fragNormal = vertNormal;

  gl_Position = projection * view * model * vec4(vertPos, 1.0);
}

