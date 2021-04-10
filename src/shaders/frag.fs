#version 330 core
uniform float greenValue;
in vec3 vertexColor;
out vec4 fragColor;

void main()
{
  fragColor = vec4(vertexColor.r, greenValue, vertexColor.b, 1.0);
}