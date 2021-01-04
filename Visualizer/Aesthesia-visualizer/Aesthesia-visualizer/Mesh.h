#pragma once

class Mesh {
public:
	Mesh();
	Mesh(const Mesh&) {};
	const aiScene* getScene();
	std::vector<GLuint> getVAO();
	std::vector<GLuint> getVBO();
	std::vector<GLuint> getNormVBO();
	std::vector<GLuint> getTexVBO();
	std::vector<GLuint> getEBO();
	std::vector<glm::vec3> getModelColor();
private:
	// TODO: prompt user with an open dialog box to get obj file path
	const aiScene* scene;
	std::string assetDir = "C:\\Users\\David\\Documents\\Perso\\stockpile\\2020-07-29\\";
	std::string objFile = "ramen.obj";
	Assimp::Importer importer;
	std::vector<std::vector<glm::vec3>> meshesVertices;
	std::vector<std::vector<GLuint>> meshesIndices;
	std::vector<std::vector<glm::vec3>> meshesNormals;
	std::vector<std::vector<glm::vec2>> meshesTexCoords;
	std::vector<GLuint> materialIndices;
	aiColor3D materialColor = aiColor3D(0.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> modelColor;
	std::vector<GLuint> VAO, VBO, normVBO, texVBO, EBO;
};