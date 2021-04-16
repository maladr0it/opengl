#version 330 core

uniform mat4 transform;

layout (location = 0) in vec3 vertPos;
layout (location = 2) in vec3 vertTexCoord;

out vec2 texCoord;

void main() {
  gl_Position = transform * vec4(vertPos, 1.0f);
  texCoord = vec2(vertTexCoord.x, vertTexCoord.y);
}

