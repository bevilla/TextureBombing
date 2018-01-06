#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include "BlinnPhongShader.h"

BlinnPhongShader::BlinnPhongShader() :
	Shader(),
	kd(1, 0, 0),
	ka(0.1, 0.1, 0.1),
	ks(0.8, 0.8, 0.8),
	shininess(100)
{
	_shaderProgram = new ShaderProgram();
	_shaderProgram->initFromFiles("shaders/BlinnPhong.vert", "shaders/BlinnPhong.frag");

	_shaderProgram->addAttribute("v_coord3d");
	_shaderProgram->addAttribute("v_normal");

	_shaderProgram->addUniform("modelViewMatrix");
	_shaderProgram->addUniform("normalMatrix");
	_shaderProgram->addUniform("mvp");

	_shaderProgram->addUniform("lightPosition");
	_shaderProgram->addUniform("lightIntensity");

	_shaderProgram->addUniform("kd");
	_shaderProgram->addUniform("ka");
	_shaderProgram->addUniform("ks");
	_shaderProgram->addUniform("shininess");
}

BlinnPhongShader::~BlinnPhongShader()
{
	delete _shaderProgram;
}

void BlinnPhongShader::initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures)
{
	GLuint vbo;
	GLuint ibo;

	glGenVertexArrays(1, &_vao);  //VAO
	glBindVertexArray(_vao);

	glGenBuffers(1, &vbo);  //VBO for position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Mesh::Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);  //VBO for index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(_shaderProgram->attribute("v_coord3d"), 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid *)0);
	glEnableVertexAttribArray(_shaderProgram->attribute("v_coord3d"));

	// Vertex Normals
	glVertexAttribPointer(_shaderProgram->attribute("v_normal"), 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid *)offsetof(Mesh::Vertex, normal));
	glEnableVertexAttribArray(_shaderProgram->attribute("v_normal"));

	// Vertex Texture Coords
	/*glVertexAttribPointer(texIdx, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(texIdx);*/

	glBindVertexArray(0);
}

void BlinnPhongShader::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count)
{
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	_shaderProgram->use();

	glUniformMatrix4fv(_shaderProgram->uniform("modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
	glUniformMatrix3fv(_shaderProgram->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform4fv(_shaderProgram->uniform("lightPosition"), 1, glm::value_ptr(view * glm::vec4(light.position, 1.0)));
	glUniform3fv(_shaderProgram->uniform("lightIntensity"), 1, glm::value_ptr(light.intensity));

	glUniform3fv(_shaderProgram->uniform("ka"), 1, glm::value_ptr(ka));
	glUniform3fv(_shaderProgram->uniform("kd"), 1, glm::value_ptr(kd));
	glUniform3fv(_shaderProgram->uniform("ks"), 1, glm::value_ptr(ks));
	glUniform1fv(_shaderProgram->uniform("shininess"), 1, &shininess);

	drawElements(count);

	_shaderProgram->disable();
}

Shader *BlinnPhongShader::clone()
{
	BlinnPhongShader *shader = new BlinnPhongShader();

	shader->kd = kd;
	shader->ka = ka;
	shader->ks = ks;
	shader->shininess = shininess;
	return shader;
}
