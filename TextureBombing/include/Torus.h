#pragma once

#include "Object3D.h"

class Torus : public Object3D
{
private:
	int faces;
	int rings;
	int sides;
	int verts;
	int elements;

public:
	Torus();
	~Torus();

private:
	void generateVerts(GLfloat * verts, GLfloat * norms, unsigned int * el, float outerRadius, float innerRadius);
};

