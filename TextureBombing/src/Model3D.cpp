#include <iostream>

#include <SOIL/SOIL.h>

#include "Model3D.h"

#include "Model3DShader.h"

Model3D::Model3D(const std::string &path) :
	Object3D()
{
	std::string name = path.substr(path.find_last_of('/') + 1, path.length());
	setName(name);
	std::cout << "Loading " << name << "..." << std::endl;
	loadModel(path);
}

Model3D::~Model3D()
{
	for (auto &mesh : _meshes)
		delete mesh;
}

bool Model3D::isLoaded() const
{
	return _meshes.size() != 0;
}

bool Model3D::isModel3D() const
{
	return true;
}

bool Model3D::isModel3DShader() const
{
	return dynamic_cast<Model3DShader *>(_meshes.at(0)->getShader()) != NULL;
}

void Model3D::enableTexture(bool value)
{
	for (auto &mesh : _meshes)
		dynamic_cast<Model3DShader *>(mesh->getShader())->enableTexture = value;
}

void Model3D::enableSpecular(bool value)
{
	for (auto &mesh : _meshes)
		dynamic_cast<Model3DShader *>(mesh->getShader())->enableSpecular = value;
}

const std::vector<Mesh *> &Model3D::getMeshes() const
{
	return _meshes;
}

void Model3D::setShader(Shader *shader)
{
	for (unsigned int i = 0; i < _meshes.size(); i++)
		_meshes[i]->setShader(i == 0 ? shader : shader->clone());
}

Shader *Model3D::getShader() const
{
	return _meshes[0]->getShader();
}

void Model3D::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Light &light)
{
	for (auto &mesh : _meshes)
		mesh->draw(model, view, projection, light);
}

void Model3D::loadModel(const std::string &path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	_directory = path.substr(0, path.find_last_of('.'));
	std::cout << path.substr(path.find_last_of('/') + 1, path.length()) << " loaded" << std::endl;
	processNode(scene->mRootNode, scene);
}

void Model3D::processNode(aiNode *node, const aiScene *scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *aimesh = scene->mMeshes[node->mMeshes[i]];
		Mesh *mesh = processMesh(aimesh, scene);

		if (mesh)
			_meshes.push_back(mesh);
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh *Model3D::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Mesh::Texture> textures;

	// process vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Mesh::Vertex vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->mNormals)
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords && mesh->mTextureCoords[0])
		{
			vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
			vertex.texCoords = glm::vec2(0.f, 0.f);

		vertices.push_back(vertex);
	}

	// process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Mesh::Texture> *diffuseMap = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMap->begin(), diffuseMap->end());
		delete diffuseMap;

		std::vector<Mesh::Texture> *specularMap = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMap->begin(), specularMap->end());
		delete specularMap;
	}

	return new Mesh(vertices, indices, textures, new Model3DShader());
}

std::vector<Mesh::Texture> *Model3D::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName)
{
	std::vector<Mesh::Texture> *textures = new std::vector<Mesh::Texture>();

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		Mesh::Texture texture;

		mat->GetTexture(type, i, &str);
		texture.id = getTextureFromFile(str.C_Str(), _directory);
		texture.type = typeName;
		texture.path = str;
		textures->push_back(texture);
	}
	return textures;
}

GLuint Model3D::getTextureFromFile(const std::string &filename, const std::string &directory)
{
	GLuint tex_2d;

	glGenTextures(1, &tex_2d);  //tex_2d is a member variable
	glBindTexture(GL_TEXTURE_2D, tex_2d);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	int width, height;
	unsigned char * image = SOIL_load_image((directory + '/' + filename).c_str(), &width, &height, 0,
		SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex_2d;
}
