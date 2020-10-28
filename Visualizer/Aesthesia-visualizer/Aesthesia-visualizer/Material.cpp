#include "pch.h"
#include "Material.h"

Material::Material(Mesh* mesh) {
	for (int i = 0; i < mesh->getScene()->mNumMaterials; i++) {
		aiString* textureFilePath = &aiString();
		mesh->getScene()->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, textureFilePath);
		textures.push_back(*new GLuint);
		texWidths.push_back(*new int);
		texHeights.push_back(*new int);


		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		aiString* textureFileName = new aiString;
		mesh->getScene()->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, textureFileName);
		std::string textureFileNameString = textureFileName->data;
		std::string texturePath = "C:\\Users\\David\\Documents\\Perso\\stockpile\\2020-07-29\\" + textureFileNameString;

		unsigned char *image = SOIL_load_image(texturePath.c_str(), &texWidths[i], &texHeights[i], 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidths[i], texHeights[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

GLuint Material::getTexture(int i) {
	return textures[i];
}