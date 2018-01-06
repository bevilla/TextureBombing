#pragma once

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>

#include "Object3D.h"
#include "Light.h"

class Model3D : public Object3D
{
private:
	std::vector<Mesh *> _meshes;
	std::string _directory;

public:
	Model3D(const std::string &path);
	~Model3D();

	bool isLoaded() const;

	virtual bool isModel3D() const;
	virtual bool isModel3DShader() const;

	void enableTexture(bool value = true);
	void enableSpecular(bool value = true);

	const std::vector<Mesh *> &getMeshes() const;

	virtual void setShader(Shader *shader);
	virtual Shader *getShader() const;

	void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light);

private:
	void loadModel(const std::string &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh *processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Mesh::Texture> *loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
	GLuint getTextureFromFile(const std::string &filename, const std::string &directory);
};
