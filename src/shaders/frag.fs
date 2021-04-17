#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixVal;

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

void main() {
  vec4 mixed = mix(
    texture(texture1, texCoord),
    texture(texture2, vec2((1.0 - texCoord.x), texCoord.y)),
    mixVal
  );

  fragColor = mixed * vec4(color, 1.0);
}