#version 400

in vec3 v_coord3d;
in vec3 v_normal;
in vec2 v_tex;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvp;

out vec3 position;
out vec3 normal;
out vec2 tex;
out float theZ;

void main(void)
{
  vec4 vertexPosition = vec4(v_coord3d, 1.0);

  position = vec3(modelViewMatrix * vertexPosition);
  normal = normalize(normalMatrix * v_normal);
  tex = v_tex;
  theZ = v_coord3d.z;

  gl_Position = mvp * vertexPosition;
}
