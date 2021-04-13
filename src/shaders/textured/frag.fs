#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixVal;

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

void main() {
  fragColor = mix(
    texture(texture1, texCoord),
    texture(texture2, vec2((1.0 - texCoord.x) * 2, texCoord.y * 2)),
    mixVal
  );
}