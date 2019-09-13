#include "pch.h"
#include "Cube.h"

Cube::Cube() {
	for (int i = 0; i < indices.size(); i += 3) {
		normals.push_back(normalize(glm::cross(vertices[indices[i + 1]] - vertices[indices[i]], vertices[indices[i + 2]] - vertices[indices[i]])));
		normals.push_back(normalize(glm::cross(vertices[indices[i + 2]] - vertices[indices[i + 1]], vertices[indices[i]] - vertices[indices[i + 1]])));
		normals.push_back(normalize(glm::cross(vertices[indices[i]] - vertices[indices[i + 2]], vertices[indices[i + 1]] - vertices[indices[i + 2]])));
	}
}

std::vector<glm::vec3> Cube::getVertices() {
	return vertices;
}

std::vector<glm::vec2> Cube::getTexCoord() {
	return texCoord;
}

std::vector<GLuint> Cube::getIndices() {
	return indices;
}

std::vector<glm::vec3> Cube::getNormals() {
	return normals;
}