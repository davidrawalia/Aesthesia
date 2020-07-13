#include "pch.h"
#include "Aesthesia-visualizer.h"

int main()
{
	if (init() < 0) {
		return -1;
	}

	//	Shader setup
	Shader shader = Shader("vertex.shader", "fragment.shader");
	GLint modelLoc = glGetUniformLocation(shader.getProgramID(), "model");
	GLint viewLoc = glGetUniformLocation(shader.getProgramID(), "view");
	GLint projLoc = glGetUniformLocation(shader.getProgramID(),
		"projection");
	GLint lightLoc = glGetUniformLocation(shader.getProgramID(), "light");
	GLint colorLoc = glGetUniformLocation(shader.getProgramID(), "color");
	GLint ambLightLoc = glGetUniformLocation(shader.getProgramID(),
		"ambLight");
	GLint cameraPositionLoc = glGetUniformLocation(shader.getProgramID(),
		"cameraPosition");
	GLint textureBoolLoc = glGetUniformLocation(shader.getProgramID(),
		"textureBool");

	// Importing scene
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile("C:\\Users\\David\\Documents\\Perso\\Wally\\logo 3d\\wally logo_001.obj",
		aiProcess_Triangulate |
		aiProcess_ValidateDataStructure |
		aiProcess_GenNormals
	);
	// If the import failed, report it
	if (!scene) {
		std::cout << "file not found";
	}

	// Extract vertex coordinates
	std::vector<glm::vec3> meshesVertices;
	int numVertices = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshesVertices.push_back(glm::vec3(
				scene->mMeshes[i]->mVertices[j].x,
				scene->mMeshes[i]->mVertices[j].y,
				scene->mMeshes[i]->mVertices[j].z));
			numVertices++;
			std:: cout << "v" << j << ": " << 
				scene->mMeshes[i]->mVertices[j].x << ", " <<
				scene->mMeshes[i]->mVertices[j].y << ", " <<
				scene->mMeshes[i]->mVertices[j].z << "\n ";
		}
	}

	std::cout << "numVertices: "  << numVertices << "\n";
	std::cout << "calculated numVertices: " << scene->mMeshes[0]->mNumVertices + scene->mMeshes[1]->mNumVertices << "\n";

	// Extract indices (assimp doesn't support vertex joins so indices are in order)
	std::vector<GLuint> meshesIndices;
	int numIndices = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
			meshesIndices.push_back(numIndices);
			numIndices++;
			meshesIndices.push_back(numIndices);
			numIndices++;
			meshesIndices.push_back(numIndices);
			numIndices++;
		}
	}

	std::cout << "numIndices: " << numIndices << "\n";
	std::cout << "calculated numIndices: " << scene->mMeshes[0]->mNumFaces * 3 + scene->mMeshes[1]->mNumFaces * 3 << "\n";


	// Extract normals
	std::vector<glm::vec3> meshesNormals;
	int numNormals = 0;

	for (int i = 0; i < scene->mNumMeshes; i++) {
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshesNormals.push_back(glm::vec3(
				scene->mMeshes[i]->mNormals[j].x,
				scene->mMeshes[i]->mNormals[j].y,
				scene->mMeshes[i]->mNormals[j].z));
			numNormals++;
		}
	}

	std::cout << "numNormals: " << numNormals << "\n";
	std::cout << "canumNormals: " << scene->mMeshes[0]->mNumVertices + scene->mMeshes[1]->mNumVertices << "\n";

	// Extract texture coordinates

	std::vector<glm::vec2> meshesTexCoords;

	/*
	if (scene->HasTextures){
		for (int i = 0; i < scene->mNumMeshes; i++) {
			for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
				meshesTexCoords.push_back(glm::vec2(
					scene->mMeshes[i]->mTextureCoords[0]->x,
					scene->mMeshes[i]->mTextureCoords[0]->y)
				);
			}
			meshesTexCoords.push_back(meshTexCoords);
		}
	}
	*/

	//	Mesh VBO and VAO binding
	GLuint VBO, VAO, normVBO, texVBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3),
		&meshesVertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(glm::vec3),
		&meshesNormals[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint),
		&meshesIndices.front(), GL_DYNAMIC_DRAW);
	
	/*
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(glm::vec2),
		&texCoord[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//	******** MAIN LOOP ********
	while (!glfwWindowShouldClose(window)){
		
		// Read incoming data from shared memory
		using namespace boost::interprocess;
		try {
			managed_shared_memory segment(open_only, "shared_memory");
			inDataLenght = segment.find<float>("data").second;
			for (int i = 0; i < 127; i++) {
				*(inData + i) = *(segment.find<float>("data").first + i);
			}
		}
		catch (interprocess_exception e) {
			std::cout << e.what() << '\n';
		}

		//	FPS counter
		GLdouble currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		nbFrames++;
		if (currentTime - lastTime >= 1.0) 
		{
			if (fps_count) std::cout << nbFrames << " - ";
			nbFrames = 0;
			lastTime += 1.0;
		}

		//	Event polling
		glfwPollEvents();
		doMovement();

		glClearColor(*redbk, *greenbk, *bluebk, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 light = glm::vec3(5.0f, 20.0f, 5.0f);	// *** LIGHT COORDS ***

		glm::mat4 projection = glm::perspective(camera.GetZoom(), 
												(GLfloat)width/(GLfloat)height, 
											    0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 color;
		glm::vec3 cameraPosition = camera.getPosition();
		glm::mat4 worldTransform;

		worldTransform = glm::rotate(worldTransform, -worldXRotation, right);
		worldTransform = glm::rotate(worldTransform, worldYRotation, up);
		light = worldTransform * glm::vec4(light, 1.0f);

		// Render scene
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.getProgramID());

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(cameraPositionLoc, 1, glm::value_ptr(cameraPosition));
		glUniform1f(textureBoolLoc, textureBool);

		glUniform3fv(lightLoc, 1, glm::value_ptr(light));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, 
						   glm::value_ptr(worldTransform));

		// Drawing cube
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));
		ambLight = 0.1f;
		glUniform1f(ambLightLoc, ambLight);
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, renderMode);

		glm::mat4 cubeTransform = glm::translate(glm::mat4(), 
												 glm::vec3(0.0f, 0.0f, 0.0f));
		cubeTransform = glm::scale(cubeTransform, glm::vec3(5.0f, 5.0f, 5.0f));		
//		cubeTransform = glm::scale(cubeTransform, glm::vec3(inData[0]*0.1, 
//								   5, 5.0f));
		cubeTransform = worldTransform * cubeTransform;

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, 
						   glm::value_ptr(cubeTransform));
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
			
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}

int init() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Aesthesia", nullptr, 
							  nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// projection matrix defines the properties of the camera
	glMatrixMode(GL_PROJECTION);
	// replace the current matrix with the identity matrix to place at (0,0,0)
	glLoadIdentity(); 
	// essentially set coordinate system
	glOrtho(0, width, 0, height, 0.1, 1000);
	// modelview matrix (default) defines how your objects are transformed
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// size of points when in point view mode
	glPointSize(5.0);
	// disable rendering of faces pointing away from camera
//	glEnable(GL_CULL_FACE);
}

void doMovement() {
	const GLfloat velocity = 1;

	if (keys[GLFW_KEY_UP])
	{
		worldXRotation += velocity * deltaTime;
	}
	if (keys[GLFW_KEY_DOWN])
	{
		worldXRotation -= velocity * deltaTime;

	}
	if (keys[GLFW_KEY_LEFT])
	{
		worldYRotation += velocity * deltaTime;
	}
	if (keys[GLFW_KEY_RIGHT])
	{
		worldYRotation -= velocity * deltaTime;
	}
};

void framebuffer_size_callback(GLFWwindow* window, 
							   int newwidth, int newheight) {
	glViewport(0, 0, newwidth, newheight);
	height = newheight;
	width = newwidth;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if (mousePressed[0])
		camera.ProcessMouseMovement(xOffset, yOffset, 0);

	if (mousePressed[1])
		camera.ProcessMouseMovement(xOffset, yOffset, 1);

	if (mousePressed[2])
		camera.ProcessMouseMovement(xOffset, yOffset, 2);

}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, 
			      int mode)
{
	const GLfloat scaleSpeed = 0.1;
	const GLfloat positionSpeed = 1;
	const GLfloat rotationSpeed = 0.0872665;

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_T:
			renderMode = GL_FILL;
			break;
		case GLFW_KEY_L:
			renderMode = GL_LINE;
			break;
		case GLFW_KEY_P:
			renderMode = GL_POINT;
			break;
		case GLFW_KEY_HOME:
			camera.resetCamera();
			worldXRotation = 0;
			worldYRotation = 0;
			break;
		case GLFW_KEY_X:
			if (textureBool == 1.0)
				textureBool = 0.0;
			else
				textureBool = 1.0;
			break;
		case GLFW_KEY_TAB:
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, 
						   int mods)
{
	GLdouble xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	lastX = xpos;
	lastY = ypos;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		mousePressed[0] = true;
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		mousePressed[0] = false;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		mousePressed[1] = true;
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		mousePressed[1] = false;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		mousePressed[2] = true;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		mousePressed[2] = false;
}