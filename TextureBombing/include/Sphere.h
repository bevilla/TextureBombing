#pragma once

#include "Object3D.h"

class Sphere : public Object3D
{
private:
	int nVerts;
	int elements;
	float radius;
	int slices, stacks;

public:
	Sphere();
	~Sphere();

private:
	void generateVerts(float * verts, float * norms, float * tex, unsigned int * el);
};

