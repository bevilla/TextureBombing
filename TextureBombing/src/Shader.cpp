#include "Shader.h"

Shader::~Shader()
{
}

void Shader::drawElements(GLsizei count)
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
