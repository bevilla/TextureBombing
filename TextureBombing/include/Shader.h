#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "Mesh.h"

class Shader
{
protected:
	GLuint _vao;

public:
	virtual ~Shader();

	virtual void initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures) = 0;
	virtual void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count) = 0;

	virtual Shader *clone() = 0;

protected:
	void drawElements(GLsizei count);
};

