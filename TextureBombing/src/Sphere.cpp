#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Sphere.h"

#include "BlinnPhongShader.h"

Sphere::Sphere()
{
	setName("Sphere");

	radius = 0.5f;
	slices = 60;
	stacks = 60;
	nVerts = (slices + 1) * (stacks + 1);  //the number of vertices
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Vertices
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];

	// Tex coords
	float * tex = new float[2 * nVerts];

	// Elements
	unsigned int * el = new unsigned int[elements];

	// Generate the vertex data : this function fill all data into the arrays.
	generateVerts(v, n, tex, el);

	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint> indices;

	for (int i = 0; i < nVerts; i++)
		vertices.push_back({
		glm::vec3(v[i * 3], v[i * 3 + 1], v[i * 3 + 2]),
		glm::vec3(n[i * 3], n[i * 3 + 1], n[i * 3 + 2]),
		glm::vec2(tex[i * 2], tex[i * 2 + 1]),
	});
	for (int i = 0; i < elements; i++)
		indices.push_back(el[i]);

	setMesh(vertices, indices, std::vector<Mesh::Texture>(), new BlinnPhongShader());
}


Sphere::~Sphere()
{
}

void Sphere::generateVerts(float * verts, float * norms, float * tex, unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}
