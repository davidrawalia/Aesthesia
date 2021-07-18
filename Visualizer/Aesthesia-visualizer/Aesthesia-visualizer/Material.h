#pragma once
#include "Mesh.h"


class Material {
public:
	Material(Mesh* mesh);
	GLuint getTexture(int i);
	bool hasTexture(int i);

private:
	aiString* textureFilePath;
	std::vector<GLuint> textures;
	std::vector<bool> haveTextures;
	std::vector<int> texWidths;
	std::vector<int> texHeights;
};