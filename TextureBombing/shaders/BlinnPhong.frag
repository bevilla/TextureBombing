#version 400

in vec3 position;
in vec3 normal;

uniform vec4 lightPosition;
uniform vec3 lightIntensity;

uniform vec3 kd;
uniform vec3 ka;
uniform vec3 ks;
uniform float shininess;

out vec4 fragColors;

void main()
{
  vec3 finalColor = vec3(0, 0, 0);
  vec3 ambient = lightIntensity * ka;
  vec3 l = vec3(0, 0, 0);
  vec3 v = vec3(0, 0, 0);
  vec3 h = vec3(0, 0, 0);
  float ldotn = 0.f;
  vec3 diffuse = vec3(0, 0, 0);
  vec3 spec = vec3(0, 0, 0);

  l = normalize(vec3(lightPosition - vec4(position, 1.0)));
  v = normalize(-position);
  h = normalize(v + l);

  ldotn = dot(l, normal);

  diffuse = lightIntensity * kd * max(ldotn, 0);
  spec = vec3(0, 0, 0);

  if (ldotn > 0)
    spec = lightIntensity * ks * pow(max(dot(h, normal), 0.0), shininess);

  finalColor += diffuse + spec + ambient;

  fragColors = vec4(finalColor, 1.0);
}
