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
	std::string getFileDir();
	float getReflectionCoefficient(int i);
	float getReflectionExponent(int i);
	float getSpecular(int i);
private:
	const aiScene* scene;
	std::wstring wFilePath;
	std::string filePath;
	std::string fileDir;
	Assimp::Importer importer;
	std::vector<std::vector<glm::vec3>> meshesVertices;
	std::vector<std::vector<GLuint>> meshesIndices;
	std::vector<std::vector<glm::vec3>> meshesNormals;
	std::vector<std::vector<glm::vec2>> meshesTexCoords;
	std::vector<GLuint> materialIndices;
	aiColor3D materialColor = aiColor3D(0.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> modelColor;
	std::vector<GLuint> VAO, VBO, normVBO, texVBO, EBO;
	aiColor3D materialReflectionCoefficient = aiColor3D(0.0f, 0.0f, 0.0f);
	float materialReflectionExponent = 0.0f;
	aiColor3D materialSpecular = aiColor3D(0.0f, 0.0f, 0.0f);
	std::vector<float> reflectionCoefficients;
	std::vector<float> reflectionExponents;
	std::vector<float> speculars;
};