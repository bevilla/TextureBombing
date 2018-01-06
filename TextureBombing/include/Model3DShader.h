#pragma once

#include "Shader.h"

#include "Loader.h"

class Model3DShader : public Shader
{
private:
	ShaderProgram *_shaderProgram;
	const std::vector<Mesh::Texture> *_textures;

public:
	glm::vec3 ka;
	float shininess;
	bool enableTexture;
	bool enableSpecular;

public:
	Model3DShader();
	~Model3DShader();

	void initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures);
	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count);

	Shader *clone();
};
