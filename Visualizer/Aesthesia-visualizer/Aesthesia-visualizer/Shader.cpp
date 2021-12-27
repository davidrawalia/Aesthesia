#include "pch.h"
#include "Shader.h"

Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);

	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", fragment_shader_path.c_str());
		getchar();
		exit(-1);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_shader_path.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_shader_path.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glBindAttribLocation(ProgramID, 0, "in_Position");

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//free up memory
	glDeleteShader(VertexShaderID); 
	glDeleteShader(FragmentShaderID);

	modelLoc = glGetUniformLocation(ProgramID, "model");
	viewLoc = glGetUniformLocation(ProgramID, "view");
	projLoc = glGetUniformLocation(ProgramID, "projection");
	lightLoc = glGetUniformLocation(ProgramID, "light");
	colorLoc = glGetUniformLocation(ProgramID, "color");
	ambLightLoc = glGetUniformLocation(ProgramID, "ambLight");
	cameraPositionLoc = glGetUniformLocation(ProgramID, "cameraPosition");
	textureBoolLoc = glGetUniformLocation(ProgramID, "textureBool");
	reflectionCoefficientLoc = glGetUniformLocation(ProgramID, "reflectionCoefficient");
	reflectionExponentLoc = glGetUniformLocation(ProgramID, "reflectionExponent");
	specularLoc = glGetUniformLocation(ProgramID, "specular");
	glUseProgram(ProgramID);
}

GLuint Shader::getProgramID() {
	return ProgramID;
}

GLuint Shader::getModelLoc() {
	return modelLoc;
}

GLuint Shader::getViewLoc() {
	return viewLoc;
}

GLuint Shader::getProjLoc() {
	return projLoc;
}

GLuint Shader::getLightLoc() {
	return lightLoc;
}

GLuint Shader::getColorLoc() {
	return colorLoc;
}

GLuint Shader::getAmbLightLoc() {
	return ambLightLoc;
}

GLuint Shader::getCameraPositionLoc() {
	return cameraPositionLoc;
}

GLuint Shader::getTextureBoolLoc() {
	return textureBoolLoc;
}

GLuint Shader::getReflectionCoefficientLoc()
{
	return reflectionCoefficientLoc;
}

GLuint Shader::getReflectionExponentLoc()
{
	return reflectionExponentLoc;
}

GLuint Shader::getSpecularLoc()
{
	return specularLoc;
}