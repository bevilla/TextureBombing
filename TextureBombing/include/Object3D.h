#pragma once

#include <glm/mat4x4.hpp>

#include "Light.h"
#include "Mesh.h"

class Object3D
{
private:
	Mesh	*_mesh;
	std::string _name;
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::vec3 _rotation;

public:
	Object3D();
	virtual ~Object3D();

	const std::string &getName() const;
	void setName(const std::string &name);

	virtual bool isModel3D() const;
	virtual bool isModel3DShader() const;

	Mesh *getMesh() const;

	glm::vec3 &getPosition();
	glm::vec3 &getScale();
	glm::vec3 &getRotation();
	const glm::vec3 &getPosition() const;
	const glm::vec3 &getScale() const;
	const glm::vec3 &getRotation() const;

	void setPosition(const glm::vec3 &vec);
	void setScale(const glm::vec3 &vec);
	void setRotation(const glm::vec3 &vec);

	void move(const glm::vec3 &vec);
	void scale(const glm::vec3 &vec);
	void rotate(const glm::vec3 &vec);

	virtual void setShader(Shader *shader);
	virtual Shader *getShader() const;

	virtual void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light);

protected:
	void setMesh(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures, Shader *shader = NULL);
};
