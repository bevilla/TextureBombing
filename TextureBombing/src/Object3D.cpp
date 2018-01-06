#include "Object3D.h"

#include "Model3DShader.h"

Object3D::Object3D() :
	_mesh(NULL),
	_name("Unnamed"),
	_position(0, 0, 0),
	_scale(1, 1, 1),
	_rotation(0, 0, 0)
{
}

Object3D::~Object3D()
{
	if (_mesh)
		delete _mesh;
}

const std::string &Object3D::getName() const
{
	return _name;
}

void Object3D::setName(const std::string &name)
{
	_name = name;
}

bool Object3D::isModel3D() const
{
	return false;
}

bool Object3D::isModel3DShader() const
{
	return dynamic_cast<Model3DShader *>(getMesh()->getShader()) != NULL;
}

Mesh *Object3D::getMesh() const
{
	return _mesh;
}

glm::vec3 &Object3D::getPosition()
{
	return _position;
}

glm::vec3 &Object3D::getScale()
{
	return _scale;
}

glm::vec3 &Object3D::getRotation()
{
	return _rotation;
}

const glm::vec3 &Object3D::getPosition() const
{
	return _position;
}

const glm::vec3 &Object3D::getScale() const
{
	return _scale;
}

const glm::vec3 &Object3D::getRotation() const
{
	return _rotation;
}

void Object3D::setPosition(const glm::vec3 &vec)
{
	_position = vec;
}

void Object3D::setScale(const glm::vec3 &vec)
{
	_scale = vec;
}

void Object3D::setRotation(const glm::vec3 &vec)
{
	_rotation = vec;
}

void Object3D::move(const glm::vec3 &vec)
{
	_position += vec;
}

void Object3D::scale(const glm::vec3 &vec)
{
	_scale += vec;
}

void Object3D::rotate(const glm::vec3 &vec)
{
	_rotation += vec;
}

void Object3D::setShader(Shader *shader)
{
	_mesh->setShader(shader);
	//	setMesh(_mesh->getVertices(), _mesh->getIndices(), _mesh->getTextures(), shader);
}

Shader *Object3D::getShader() const
{
	return _mesh->getShader();
}

void Object3D::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light)
{
	if (_mesh)
		_mesh->draw(model, view, projection, light);
}

void Object3D::setMesh(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures, Shader *shader)
{
	Mesh	*mesh = new Mesh(vertices, indices, textures, shader);

	if (_mesh)
		delete _mesh;
	_mesh = mesh;
}
