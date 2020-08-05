class Shader {
public:
	Shader(std::string vertex_shader_path, std::string fragment_shader_path);
	GLuint getProgramID();
	GLuint getModelLoc();
	GLuint getViewLoc();
	GLuint getProjLoc();
	GLuint getLightLoc();
	GLuint getColorLoc();
	GLuint getAmbLightLoc();
	GLuint getCameraPositionLoc();
	GLuint getTextureBoolLoc();

private:
	GLuint ProgramID;
	GLint modelLoc;
	GLint viewLoc;
	GLint projLoc;
	GLint lightLoc;
	GLint colorLoc;
	GLint ambLightLoc;
	GLint cameraPositionLoc;
	GLint textureBoolLoc;
};