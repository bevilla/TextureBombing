#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include "Model3DShader.h"

Model3DShader::Model3DShader() :
	Shader(),
	ka(0.01, 0.01, 0.01),
	shininess(10),
	enableTexture(true),
	enableSpecular(true)
{
	_shaderProgram = new ShaderProgram();
	_shaderProgram->initFromFiles("shaders/Model3D.vert", "shaders/Model3D.frag");

	_shaderProgram->addAttribute("v_coord3d");
	_shaderProgram->addAttribute("v_normal");
	_shaderProgram->addAttribute("v_tex");

	_shaderProgram->addUniform("modelViewMatrix");
	_shaderProgram->addUniform("normalMatrix");
	_shaderProgram->addUniform("mvp");

	_shaderProgram->addUniform("lightPosition");
	_shaderProgram->addUniform("lightIntensity");

	_shaderProgram->addUniform("ka");
	_shaderProgram->addUniform("shininess");

	_shaderProgram->addUniform("is_texture_diffuse1");
	_shaderProgram->addUniform("texture_diffuse1");
	_shaderProgram->addUniform("is_texture_specular1");
	_shaderProgram->addUniform("texture_specular1");
}

Model3DShader::~Model3DShader()
{
	delete _shaderProgram;
}

void Model3DShader::initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures)
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
	glVertexAttribPointer(_shaderProgram->attribute("v_tex"), 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (GLvoid *)offsetof(Mesh::Vertex, texCoords));
	glEnableVertexAttribArray(_shaderProgram->attribute("v_tex"));

	glBindVertexArray(0);

	_textures = &textures;
}

void Model3DShader::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count)
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
	glUniform1fv(_shaderProgram->uniform("shininess"), 1, &shininess);

	GLuint diffuseN = 1;
	GLuint specularN = 1;
	for (GLuint i = 0; i < _textures->size(); i++)
	{
		const std::string &name = _textures->at(i).type;
		GLuint number;

		glActiveTexture(GL_TEXTURE0 + i);
		if (name == "texture_diffuse")
			number = diffuseN++;
		else if (name == "texture_specular")
			number = specularN++;

		const std::string uniformName = name + std::to_string(number);

		glUniform1f(_shaderProgram->uniform(uniformName), (float)i);
		glBindTexture(GL_TEXTURE_2D, _textures->at(i).id);
	}
	glUniform1ui(_shaderProgram->uniform("is_texture_diffuse1"), enableTexture ? diffuseN - 1 : 0);
	glUniform1ui(_shaderProgram->uniform("is_texture_specular1"), enableSpecular ? specularN - 1 : 0);

	if (_textures->size())
		glActiveTexture(GL_TEXTURE0);
	drawElements(count);

	_shaderProgram->disable();
}

Shader *Model3DShader::clone()
{
	Model3DShader *shader = new Model3DShader();

	shader->enableSpecular = enableSpecular;
	shader->enableTexture = enableTexture;
	shader->ka = ka;
	shader->shininess = shininess;
	return shader;
}
