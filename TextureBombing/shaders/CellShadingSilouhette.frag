#version 400

uniform vec3 color;

out vec4 fragColors;

void main()
{
  fragColors = vec4(color, 1.0);
}
