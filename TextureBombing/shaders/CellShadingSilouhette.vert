#version 400

in vec3 v_coord3d;
in vec3 v_normal;

uniform mat4 mvp;

uniform float offset;

void main(void)
{
  gl_Position = mvp * vec4(v_coord3d + v_normal * offset, 1.0);
}
