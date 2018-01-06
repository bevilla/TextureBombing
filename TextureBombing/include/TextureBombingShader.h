#pragma once

#include "Shader.h"

#include "Loader.h"

class TextureBombingShader : public Shader
{
public:
	enum TextureType
	{
		PNG,
		CIRCLE,
		VERONOI
	};

private:
	ShaderProgram *_shaderProgram;
	GLuint	_tex[2];

public:
	glm::vec3 kd;
	float kaRatio;
	glm::vec3 ks;
	float shininess;
	unsigned int numberOfTexture;
	float scale;
	float density;
	TextureType textureType;

public:
	TextureBombingShader();
	~TextureBombingShader();

	void initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures);
	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count);

	Shader *clone();

	void loadTexture(const std::string &path);
	void randomGenerate();
};
