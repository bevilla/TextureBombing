#include <windows.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <SOIL\SOIL.h>

#include "TextureBombingShader.h"

TextureBombingShader::TextureBombingShader() :
	Shader(),
	kd(0, 0.5, 0.5),
	kaRatio(0.1),
	ks(1, 1, 1),
	shininess(10),
	numberOfTexture(1),
	scale(4.f),
	density(0.5f),
	textureType(PNG)
{
	_shaderProgram = new ShaderProgram();
	_shaderProgram->initFromFiles("shaders/TextureBombing.vert", "shaders/TextureBombing.frag");

	_shaderProgram->addAttribute("v_coord3d");
	_shaderProgram->addAttribute("v_normal");
	_shaderProgram->addAttribute("v_tex");

	_shaderProgram->addUniform("modelViewMatrix");
	_shaderProgram->addUniform("normalMatrix");
	_shaderProgram->addUniform("mvp");

	_shaderProgram->addUniform("lightPosition");
	_shaderProgram->addUniform("lightIntensity");

	_shaderProgram->addUniform("kd");
	_shaderProgram->addUniform("kaRatio");
	_shaderProgram->addUniform("ks");
	_shaderProgram->addUniform("shininess");
	_shaderProgram->addUniform("numberOfTexture");
	_shaderProgram->addUniform("scale");
	_shaderProgram->addUniform("density");
	_shaderProgram->addUniform("textureType");

	_shaderProgram->addUniform("image");
	_shaderProgram->addUniform("randomTex");
}

TextureBombingShader::~TextureBombingShader()
{
	delete _shaderProgram;
}

void TextureBombingShader::initVAO(const std::vector<Mesh::Vertex> &vertices, const std::vector<GLuint> &indices, const std::vector<Mesh::Texture> &textures)
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
}

void TextureBombingShader::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light, GLsizei count)
{
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;

	glm::vec4 lightPos(10, 10, 10, 1);
	glm::vec3 lightIntensity(1, 1, 1);

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));

	_shaderProgram->use();

	glUniformMatrix4fv(_shaderProgram->uniform("modelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
	glUniformMatrix3fv(_shaderProgram->uniform("normalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(_shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform4fv(_shaderProgram->uniform("lightPosition"), 1, glm::value_ptr(view * glm::vec4(light.position, 1.0)));
	glUniform3fv(_shaderProgram->uniform("lightIntensity"), 1, glm::value_ptr(light.intensity));

	glUniform1f(_shaderProgram->uniform("kaRatio"), kaRatio);
	glUniform3fv(_shaderProgram->uniform("kd"), 1, glm::value_ptr(kd));
	glUniform3fv(_shaderProgram->uniform("ks"), 1, glm::value_ptr(ks));
	glUniform1fv(_shaderProgram->uniform("shininess"), 1, &shininess);
	glUniform1i(_shaderProgram->uniform("numberOfTexture"), numberOfTexture);
	glUniform1f(_shaderProgram->uniform("scale"), scale);
	glUniform1f(_shaderProgram->uniform("density"), density);
	glUniform1i(_shaderProgram->uniform("textureType"), textureType);

	glUniform1f(_shaderProgram->uniform("image"), 0);
	glUniform1f(_shaderProgram->uniform("randomTex"), 1);


	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex[0]);
	glUniform1i(_shaderProgram->uniform("image"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _tex[1]);
	glUniform1i(_shaderProgram->uniform("randomTex"), 1);


	drawElements(count);

	glBindTexture(GL_TEXTURE_2D, 0);

	_shaderProgram->disable();
}

Shader *TextureBombingShader::clone()
{
	TextureBombingShader *shader = new TextureBombingShader();

	shader->density = density;
	shader->kaRatio = kaRatio;
	shader->kd = kd;
	shader->ks = ks;
	shader->numberOfTexture = numberOfTexture;
	shader->scale = scale;
	shader->shininess = shininess;
	shader->_tex[0] = _tex[0];
	shader->_tex[1] = _tex[1];
	return shader;
}

void TextureBombingShader::loadTexture(const std::string &path)
{
	glGenTextures(2, _tex);  //tex_2d is a member variable



	glBindTexture(GL_TEXTURE_2D, _tex[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	int width, height;
	unsigned char * image = NULL;

	image = SOIL_load_image(path.c_str(), &width, &height, 0,
		SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	randomGenerate();
}

void TextureBombingShader::randomGenerate()
{
	glBindTexture(GL_TEXTURE_2D, _tex[1]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	srand(GetTickCount());
	unsigned int width = 64;
	unsigned int height = 64;
	unsigned char *image = new unsigned char[width * height * 4];
	for (unsigned int i = 0; i < width * height; i++)
	{
		image[i * 4 + 0] = rand();
		image[i * 4 + 1] = rand();
		image[i * 4 + 2] = rand();
		image[i * 4 + 3] = rand();
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, image);

	glGenerateMipmap(GL_TEXTURE_2D);

	delete image;

	glBindTexture(GL_TEXTURE_2D, 0);
}
