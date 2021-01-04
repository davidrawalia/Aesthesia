#pragma once
#include "Mesh.h"


class Material {
public:
	Material(Mesh* mesh);
	GLuint getTexture(int i);
private:
	aiString* textureFilePath;
	std::vector<GLuint> textures;
	std::vector<int> texWidths;
	std::vector<int> texHeights;
};