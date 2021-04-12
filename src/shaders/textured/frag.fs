#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec3 vertColor;
in vec2 vertTexCoord;

out vec4 fragColor;

void main() {
  fragColor = mix(texture(texture1, vertTexCoord), texture(texture2, vertTexCoord), 0.2);
}