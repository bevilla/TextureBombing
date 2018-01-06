#pragma once

#include "Shader.h"

#include "Loader.h"

class BlinnPhongShader : public Shader
{
private:
	ShaderProgram *_shaderProgram;

public:
	glm::vec3 kd;
	glm::vec3 ka;
	glm::vec3 ks;
	float shininess;

public:
	BlinnPhongShader();
	~BlinnPhongShader();

	void initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures);
	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count);

	Shader *clone();
};

