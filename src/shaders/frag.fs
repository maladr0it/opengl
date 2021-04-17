#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 texCoord;

out vec4 fragColor;

void main() {
  vec4 mixed = mix(
    texture(texture1, texCoord),
    texture(texture2, texCoord),
    0.2
  );

  fragColor = mixed;
}