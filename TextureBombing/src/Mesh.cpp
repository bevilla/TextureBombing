#include <glm/matrix.hpp>

#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Texture> &textures, Shader *shader) :
	_vertices(vertices),
	_indices(indices),
	_textures(textures),
	_shader(shader)
{
	if (_shader)
		setupMesh();
}


Mesh::~Mesh()
{
	if (_shader)
		delete _shader;
}

Shader *Mesh::getShader() const
{
	return _shader;
}

void Mesh::setShader(Shader *shader)
{
	if (_shader)
		delete _shader;
	_shader = shader;
	setupMesh();
}

const std::vector<Mesh::Vertex>	&Mesh::getVertices() const
{
	return _vertices;
}

const std::vector<GLuint>	&Mesh::getIndices() const
{
	return _indices;
}

const std::vector<Mesh::Texture>	&Mesh::getTextures() const
{
	return _textures;
}

void Mesh::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light)
{
	if (_shader)
		_shader->draw(model, view, projection, light, _indices.size());
}

void Mesh::setupMesh()
{
	if (_shader)
		_shader->initVAO(_vertices, _indices, _textures);
}