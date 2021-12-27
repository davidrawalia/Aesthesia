#include "pch.h"
#include "Material.h"

Material::Material(Mesh* mesh) {
	for (int i = 0; i < mesh->getScene()->mNumMaterials; i++) {
		textureFilePath = &aiString();
		mesh->getScene()->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, 
													textureFilePath);
		if (textureFilePath->length) {
			haveTextures.push_back(TRUE);
			textures.push_back(*new GLuint);
			texWidths.push_back(*new int);
			texHeights.push_back(*new int);


			glGenTextures(1, &textures[i]);
			glBindTexture(GL_TEXTURE_2D, textures[i]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			std::string textureFileNameString = textureFilePath->data;
			std::string texturePath = mesh->getFileDir() + textureFileNameString;

			unsigned char *image = SOIL_load_image(texturePath.c_str(), &texWidths[i], 
												   &texHeights[i], 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidths[i], texHeights[i], 0, 
						 GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			haveTextures.push_back(FALSE);
		}
	}
}

GLuint Material::getTexture(int i) {
	return textures[i];
}

bool Material::hasTexture(int i) {
	return haveTextures[i];
}