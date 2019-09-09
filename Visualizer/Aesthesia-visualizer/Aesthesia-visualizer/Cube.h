#pragma once

//	Define cube vertices
class Cube {

	public:
		Cube();
		std::vector<glm::vec3> getVertices();
		std::vector<glm::vec2> getTexCoord();
		std::vector<GLuint> getIndices();
		std::vector<glm::vec3> getNormals();

	private:
		std::vector<glm::vec3> vertices = {
			glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, -0.5),
			glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, -0.5, -0.5),

			glm::vec3(-0.5, 0.5, -0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5),
			glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, -0.5),

			glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5, -0.5),
			glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5),

			glm::vec3(0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, -0.5, 0.5),
			glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.5, 0.5, -0.5), glm::vec3(0.5, 0.5, 0.5),

			glm::vec3(0.5, -0.5, 0.5), glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, -0.5),
			glm::vec3(-0.5, -0.5, 0.5), glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, -0.5, 0.5),

			glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(-0.5, 0.5, 0.5),
			glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0.5, -0.5, 0.5), glm::vec3(0.5, 0.5, 0.5)
		};

		std::vector<glm::vec2> texCoord = {
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),

			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),

			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),

			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),

			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),

			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
			glm::vec2(0.0, 0.0), glm::vec2(1.0, 0.0), glm::vec2(1.0, 1.0),
		};

		std::vector<GLuint> indices = {
			0, 1, 2,
			3, 4, 5,

			6, 7, 8,
			9, 10, 11,

			12, 13, 14,
			15, 16, 17,

			18, 19, 20,
			21, 22, 23,

			24, 25, 26,
			27, 28, 29,

			30, 31, 32,
			33, 34, 35
		};

		std::vector<glm::vec3> normals;
};