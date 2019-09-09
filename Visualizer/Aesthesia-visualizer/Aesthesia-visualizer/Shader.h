class Shader {
public:
	Shader(std::string vertex_shader_path, std::string fragment_shader_path);
	GLuint getProgramID();

private:
	GLuint ProgramID;
};