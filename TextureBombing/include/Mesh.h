#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <assimp/IOStream.hpp>

#include "Light.h"

class Shader;

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	struct Texture
	{
		GLuint id;
		std::string type;
		aiString path;
	};

private:
	std::vector<Vertex> _vertices;
	std::vector<GLuint> _indices;
	std::vector<Texture> _textures;
	Shader *_shader;

public:
	Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures, Shader *shader = NULL);
	~Mesh();

	Shader *getShader() const;
	void setShader(Shader *shader);

	const std::vector<Vertex>	&getVertices() const;
	const std::vector<GLuint>	&getIndices() const;
	const std::vector<Texture>	&getTextures() const;

	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light);

private:
	void setupMesh();
};

