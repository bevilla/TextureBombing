#include "Cube.h"

#include "BlinnPhongShader.h"

Cube::Cube() :
	Object3D()
{
	setName("Cube");

	std::vector<Mesh::Vertex> vertices = {
		{ glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 0.5), glm::vec2(0, 0) },
		{ glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-0.5, 0.0, 0.0), glm::vec2(1, 0) },
		{ glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.0, -0.5, 0.0), glm::vec2(1, 1) },

		{ glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, 0.0, 0.5), glm::vec2(1, 0) },
		{ glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.5, 0.0, 0.0), glm::vec2(0, 0) },
		{ glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.0, -0.5, 0.0), glm::vec2(0, 1) },

		{ glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 0.5), glm::vec2(1, 1) },
		{ glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.0, 0.0), glm::vec2(0, 1) },
		{ glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.0, 0.5, 0.0), glm::vec2(1, 0) },

		{ glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 0.0, 0.5), glm::vec2(0, 1) },
		{ glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-0.5, 0.0, 0.0), glm::vec2(1, 1) },
		{ glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.0, 0.5, 0.0), glm::vec2(0, 0) },

		{ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, -0.5), glm::vec2(1, 0) },
		{ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, 0.0, 0.0), glm::vec2(0, 0) },
		{ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.0, -0.5, 0.0), glm::vec2(1, 0) },

		{ glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, 0.0, -0.5), glm::vec2(0, 0) },
		{ glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.5, 0.0, 0.0), glm::vec2(1, 0) },
		{ glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.0, -0.5, 0.0), glm::vec2(0, 0) },

		{ glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, -0.5), glm::vec2(0, 1) },
		{ glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, 0.0, 0.0), glm::vec2(1, 1) },
		{ glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.0, 0.5, 0.0), glm::vec2(1, 1) },

		{ glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 0.0, -0.5), glm::vec2(1, 1) },
		{ glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-0.5, 0.0, 0.0), glm::vec2(0, 1) },
		{ glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.0, 0.5, 0.0), glm::vec2(0, 1) }
	};

	std::vector<GLuint> indices = {
		// front
		0, 3, 6,
		6, 9, 0,
		// top
		4, 16, 19,
		19, 7, 4,
		// back
		21, 18, 15,
		15, 12, 21,
		// bottom
		13, 1, 10,
		10, 22, 13,
		// left
		14, 17, 5,
		5, 2, 14,
		// right
		11, 8, 20,
		20, 23, 11,
	};

	setMesh(vertices, indices, std::vector<Mesh::Texture>(), new BlinnPhongShader());
}

Cube::~Cube()
{
}
