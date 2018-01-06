#version 400

in vec3 position;
in vec3 normal;
in vec2 tex;
in float theZ;

uniform sampler2D image;
uniform sampler2D randomTex;

uniform vec4 lightPosition;
uniform vec3 lightIntensity;

uniform vec3 kd;
uniform float kaRatio;
uniform vec3 ks;
uniform float shininess;
uniform int numberOfTexture;
uniform float scale;
uniform float density;
uniform int textureType;

out vec4 fragColors;

vec3 png(vec3 color)
{
  vec2 scaledUV = tex * scale;
  vec2 cell = floor(scaledUV);
  vec2 offset = scaledUV - cell;
  float priority = 1.0 - clamp(density, 0.0, 1.0);
  for (int i = -1; i <= 0; i++)
  {
	for (int j = -1; j <= 0; j++)
	{
	  vec2 cell_t = cell + vec2(i, j);
	  vec2 offset_t = offset - vec2(i, j);
	  vec2 randomUV = cell_t * vec2(0.037, 0.119);
	  vec4 random = texture(randomTex, randomUV);
	  float index = floor(random.z * numberOfTexture);
	  vec2 uv = offset_t.xy - random.xy;
	  uv.x = (clamp(uv.x, 0.0, 1.0) + index) / numberOfTexture;
	  vec4 texColor = texture(image, uv);
	  if (random.w > priority && texColor.w > 0.0001)
	  {
		  color = mix(color, texColor.xyz, texColor.w);
		  priority = random.w;
	  }
	}
  }
  return color;
}

vec3 circle(vec3 color)
{
  vec2 scaledUV = tex * scale;
  vec2 cell = floor(scaledUV);
  vec2 offset = scaledUV - cell;
  float priority = 1.0 - clamp(density, 0.0, 1.0);
  for (int i = -1; i <= 0; i++)
  {
	for (int j = -1; j <= 0; j++)
	{
	  vec2 cell_t = cell + vec2(i, j);
	  vec2 offset_t = offset - vec2(i, j);
	  vec2 randomUV = cell_t * vec2(0.037, 0.119);
	  vec4 random = texture(randomTex, randomUV);

	  offset_t = offset_t - (vec2(0.5, 0.5) + random.xy);
	  float radius = dot(offset_t, offset_t);

	  if (random.w > priority && radius < 0.25)
	  {
		  color = vec3(texture(randomTex, randomUV + vec2(0.13, 0.4)));
		  priority = random.w;
	  }
	}
  }
  return color;
}

vec3 veronoi(vec3 color)
{
  vec2 scaledUV = tex * scale;
  vec2 cell = floor(scaledUV);
  vec2 offset = scaledUV - cell;
  float priority = 999;
  for (int i = -1; i <= 0; i++)
  {
	for (int j = -1; j <= 0; j++)
	{
	  vec2 cell_t = cell + vec2(i, j);
	  vec2 offset_t = offset - vec2(i, j);
	  vec2 randomUV = cell_t * vec2(0.037, 0.119);
	  vec4 random = texture(randomTex, randomUV);

	  offset_t = offset_t - (vec2(0.5, 0.5) + random.xy);
	  float radius = dot(offset_t, offset_t);

	  if (radius < priority)
	  {
		  color = vec3(texture(randomTex, randomUV + vec2(0.13, 0.4)));
		  priority = radius;
	  }
	}
  }
  return color;
}

void main()
{
  vec3 finalColor = vec3(0, 0, 0);
  vec3 l = vec3(0, 0, 0);
  vec3 v = vec3(0, 0, 0);
  vec3 h = vec3(0, 0, 0);
  float ldotn = 0.f;
  vec3 diffuse = vec3(0, 0, 0);
  vec3 spec = vec3(0, 0, 0);
  vec3 color = kd;

  switch (textureType)
  {
	case 0:
		color = png(color);
		break ;
	case 1:
		color = circle(color);
		break ;
	case 2:
		color = veronoi(color);
		break ;
	default:
		break ;
  }

  l = normalize(vec3(lightPosition - vec4(position, 1.0)));
  v = normalize(-position);
  h = normalize(v + l);

  ldotn = dot(l, normal);

  vec3 ambient = lightIntensity * color * kaRatio;

  diffuse = lightIntensity * color * max(ldotn, 0);

  spec = vec3(0, 0, 0);

  if (ldotn > 0)
    spec = lightIntensity * ks * pow(max(dot(h, normal), 0.0), shininess);

  finalColor += diffuse + spec + ambient;

  fragColors = vec4(finalColor, 1.0);
}
