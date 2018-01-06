#pragma once

#include "Shader.h"

#include "Loader.h"

class CheckerboardShader : public Shader
{
private:
	ShaderProgram *_shaderProgram;

public:
	float scale;
	glm::vec3 color1;
	glm::vec3 color2;
	float kaRatio;
	glm::vec3 ks;
	float shininess;

public:
	CheckerboardShader();
	~CheckerboardShader();

	void initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures);
	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count);

	Shader *clone();
};

