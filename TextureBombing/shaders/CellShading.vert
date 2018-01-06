#version 400

in vec3 v_coord3d;
in vec3 v_normal;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvp;

out vec3 position;
out vec3 normal;

void main(void)
{
  vec4 vertexPosition = vec4(v_coord3d, 1.0);

  position = vec3(modelViewMatrix * vertexPosition);
  normal = normalize(normalMatrix * v_normal);

  gl_Position = mvp * vertexPosition;
}
