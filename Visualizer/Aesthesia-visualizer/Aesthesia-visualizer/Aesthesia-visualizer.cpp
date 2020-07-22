#include "pch.h"
#include "Aesthesia-visualizer.h"

int main()
{
	if (init() < 0) {
		return -1;
	}

	// Shader setup
	// TODO: abstract out to the shader class
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
	glUseProgram(shader.getProgramID());

	// Importing scene
	// TODO: Abstract out to a mesh class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	// TODO: prompt user with an open dialog box to get obj file path
	std::string assetDir = "C:\\Users\\David\\Documents\\Perso\\stockpile\\2020-07-29\\";
	std::string objFile = "ramen.obj";
	const aiScene* scene = importer.ReadFile(assetDir + objFile,
		aiProcessPreset_TargetRealtime_MaxQuality
	);
	// If the import failed, report it
	if (!scene) {
		std::cout << "file not found";
	}

	// Extract vertex coordinates
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<glm::vec3> meshVetices;
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshVetices.push_back(glm::vec3(
				scene->mMeshes[i]->mVertices[j].x,
				scene->mMeshes[i]->mVertices[j].y,
				scene->mMeshes[i]->mVertices[j].z));
		}
		meshesVertices.push_back(meshVetices);
	}

	// Extract indices
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<GLuint> meshIndices;
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
			meshIndices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
			meshIndices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
			meshIndices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
		}
		meshesIndices.push_back(meshIndices);
	}

	// Extract normals
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<glm::vec3> meshNormals;
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshNormals.push_back(glm::vec3(
				scene->mMeshes[i]->mNormals[j].x,
				scene->mMeshes[i]->mNormals[j].y,
				scene->mMeshes[i]->mNormals[j].z));
		}
		meshesNormals.push_back(meshNormals);
	}

	// Extract texture coordinates
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<glm::vec2> meshTexCoords;
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshTexCoords.push_back(glm::vec2(
				scene->mMeshes[i]->mTextureCoords[0][j].x,
				scene->mMeshes[i]->mTextureCoords[0][j].y)
			);
		}
		meshesTexCoords.push_back(meshTexCoords);
	}

	// Extract material indices per mesh
	for (int i = 0; i < scene->mNumMeshes; i++) {
		materialIndices.push_back(scene->mMeshes[i]->mMaterialIndex);
		scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_COLOR_DIFFUSE, materialColor);
		modelColor.push_back(glm::vec3(materialColor.r, materialColor.g, materialColor.b));
	}

	// Mesh VBO and VAO binding
	std::vector<GLuint> VAO, VBO, normVBO, texVBO, EBO;

	for(int i = 0; i < scene->mNumMeshes; i++){
		VBO.push_back(*new GLuint);
		VAO.push_back(*new GLuint);
		normVBO.push_back(*new GLuint);
		texVBO.push_back(*new GLuint);
		EBO.push_back(*new GLuint);

		glGenVertexArrays(1, &VAO[i]);
		glBindVertexArray(VAO[i]);

		glGenBuffers(1, &VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[i]->mNumVertices * sizeof(glm::vec3),
			&meshesVertices[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &normVBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, normVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[i]->mNumVertices * sizeof(glm::vec3),
			&meshesNormals[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &texVBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, texVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[i]->mNumVertices * sizeof(glm::vec2),
			&meshesTexCoords[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &EBO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene->mMeshes[i]->mNumFaces * 3 * sizeof(GLuint),
			&meshesIndices[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// Extract texture file paths
	// TODO: Abstract out to a material class
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiString* textureFilePath = new aiString;
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, textureFilePath);

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
		scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, textureFileName);
		std::string textureFileNameString = textureFileName->data;
		std::string texturePath = assetDir + textureFileNameString;

		unsigned char *image = SOIL_load_image(texturePath.c_str(), &texWidths[i], &texHeights[i], 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidths[i], texHeights[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

/*	GLuint texture;

	int texWidth, texHeight;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	aiString* textureFileName = new aiString;
	scene->mMaterials[1]->GetTexture(aiTextureType_DIFFUSE, 0, textureFileName);
	std::string textureFileNameString = textureFileName->data;
	std::string texturePath = assetDir + textureFileNameString;

	unsigned char *image = SOIL_load_image(texturePath.c_str(), &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
*/
	//	******** MAIN LOOP ********
	while (!glfwWindowShouldClose(window)){
		
		// Read incoming data from shared memory
		// TODO: abstract this to it's own object
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

		// FPS counter
		// TODO: abstract this to it's own object
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

		// Event polling
		glfwPollEvents();
		doMovement();

		// Update transformation matrices 
		projection = glm::perspective(camera.GetZoom(), 
												(GLfloat)width/(GLfloat)height, 
											    0.1f, 1000.0f);
		view = camera.GetViewMatrix();
		cameraPosition = camera.getPosition();
		worldTransform = glm::mat4(1.0f);
		worldTransform = glm::rotate(worldTransform, -worldXRotation, right);
		worldTransform = glm::rotate(worldTransform, worldYRotation, up);
		lightWorldPos = worldTransform * glm::vec4(lightModelPos, 1.0f);

		// Clear buffers 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Apply world transformation

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(cameraPositionLoc, 1, glm::value_ptr(cameraPosition));
		glUniform1f(textureBoolLoc, textureBool);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(worldTransform));
		glUniform3fv(lightLoc, 1, glm::value_ptr(lightWorldPos));
		glUniform1f(ambLightLoc, ambLight);

		// Bind vertex array and render scene
		for (int i = 0; i < scene->mNumMeshes; i++) {

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glUniform1i(glGetUniformLocation(shader.getProgramID(), "meshTexture"), 0);
			glBindVertexArray(VAO[i]);

			glUniform3fv(colorLoc, 1, glm::value_ptr(modelColor[i]));

			// Apply transformations to the mesh
			meshTransform = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
			meshTransform = glm::scale(meshTransform, glm::vec3(
				5.0f + inData[i]  *0.075, 
				5.0f + inData[i] * 0.075, 
				5.0f + inData[i] * 0.075));
			meshTransform = worldTransform * meshTransform;
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(meshTransform));

			glDrawElements(GL_TRIANGLES, scene->mMeshes[i]->mNumFaces * 3, GL_UNSIGNED_INT, nullptr);
			glBindVertexArray(0);
		}
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	for (int i = 0; i < scene->mNumMeshes; i++) {
		glDeleteVertexArrays(1, &VAO[i]);
		glDeleteBuffers(1, &VBO[i]);
		glDeleteBuffers(1, &normVBO[i]);
		glDeleteBuffers(1, &texVBO[i]);
		glDeleteBuffers(1, &EBO[i]);
	}

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
	glEnable(GL_CULL_FACE);
	// Define polygon rendering mode
	glPolygonMode(GL_FRONT_AND_BACK, renderMode);
	// Set background color to greenscreen green
	glClearColor(*redbk, *greenbk, *bluebk, 1.0f);
	// enable texture alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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