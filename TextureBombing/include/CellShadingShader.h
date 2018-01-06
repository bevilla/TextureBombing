#pragma once

#include "Shader.h"

#include "Loader.h"

class CellShadingShader : public Shader
{
private:
	ShaderProgram *_shaderProgram;
	ShaderProgram *_shaderProgramSilouhette;

public:
	glm::vec3 kd;
	glm::vec3 ka;
	int level;

	glm::vec3 silouhetteColor;
	float silouhetteSize;

public:
	CellShadingShader();
	~CellShadingShader();

	void initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures);
	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count);

	Shader *clone();
};

