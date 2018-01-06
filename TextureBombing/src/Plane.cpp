#include "Plane.h"

#include "BlinnPhongShader.h"
#include "CheckerboardShader.h"

Plane::Plane() :
	Object3D()
{
	setName("Plane");

	std::vector<Mesh::Vertex> vertices = {
		{ glm::vec3(-0.5, 0.0, -0.5), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0, 0) },
		{ glm::vec3(0.5, 0.0, -0.5), glm::vec3(0.0, 1.0, 0.0), glm::vec2(1, 0) },
		{ glm::vec3(0.5, 0.0, 0.5), glm::vec3(0.0, 1.0, 0.0), glm::vec2(1, 1) },
		{ glm::vec3(-0.5, 0.0, 0.5), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0, 1) },
	};

	std::vector<GLuint> indices = {
		// front
		0, 3, 1,
		2, 1, 3,
	};

	setMesh(vertices, indices, std::vector<Mesh::Texture>(), new CheckerboardShader());
}

Plane::~Plane()
{
}
