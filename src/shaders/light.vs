#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vertPos;

void main() {
  gl_Position = projection * view * model * vec4(vertPos, 1.0);
}

