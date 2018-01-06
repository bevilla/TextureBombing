#version 400

in vec3 position;
in vec3 normal;
in vec2 tex;

uniform vec4 lightPosition;
uniform vec3 lightIntensity;

uniform vec3 ka;
uniform float shininess;

uniform uint is_texture_diffuse1;
uniform sampler2D texture_diffuse1;
uniform uint is_texture_specular1;
uniform sampler2D texture_specular1;

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

  vec3 diffuseColor = vec3(0.4, 0.4, 0.4);
  vec3 specularColor = vec3(0.2, 0.2, 0.2);

  if (is_texture_diffuse1 != 0)
	  diffuseColor = vec3(texture(texture_diffuse1, tex));
  if (is_texture_specular1 != 0)
	  specularColor = vec3(texture(texture_specular1, tex));

  l = normalize(vec3(lightPosition - vec4(position, 1.0)));
  v = normalize(-position);
  h = normalize(v + l);

  ldotn = dot(l, normal);

  diffuse = lightIntensity * diffuseColor * max(ldotn, 0);
  spec = vec3(0, 0, 0);

  if (ldotn > 0)
    spec = lightIntensity * specularColor * pow(max(dot(h, normal), 0.0), shininess);

  finalColor += diffuse + spec + ambient;

  fragColors = vec4(finalColor, 1.0);
}
