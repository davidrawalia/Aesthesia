#include "pch.h"
#include "Aesthesia-visualizer.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
					_In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);


	//Initialize logger
	logger = spdlog::basic_logger_mt("file_log", "logs/visualizer-log.txt");
	spdlog::get("file_log")->info("Visualizer process start");

	//Setup unicode converter
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;

	//Log command-line arguments (converted from unicode to bytes)
	spdlog::get("file_log")->info(converter.to_bytes(lpCmdLine));

	//Define command-line arguments
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("vshader", po::value<std::string>(), "set path to vertex shader")
		("fshader", po::value<std::string>(), "set path to fragment shader")
		("model", po::value<std::string>(), "set path to 3d model");

	//Parse command-line arguments
	argsList = po::split_winmain(lpCmdLine);
	store(po::wcommand_line_parser(argsList).options(desc).run(), vm);
	po::notify(vm);

	//Use command-line arguments
	if (vm.count("help")) {
		std::cout << (desc);
		return 1;
	}
	if (vm.count("vshader")) {
		vertexShaderPath = vm["vshader"].as<std::string>();
	}
	if (vm.count("fshader")) {
		fragmentShaderPath = vm["fshader"].as<std::string>();
	}
	if (vm.count("model")) {
		modelPath = vm["model"].as<std::string>();
		spdlog::get("file_log")->info("Using model: {}", modelPath);
	}

	//Initialize GLFW
	if (init() < 0) {
		return -1;
	}

	// Define default shader files if no arguments given
	if (vertexShaderPath.empty()) {
		vertexShaderPath = "vertex.shader";
		spdlog::get("file_log")->info("Using default vertex shader");
	}
	if (fragmentShaderPath.empty()) {
		fragmentShaderPath = "fragmen.shader";
		spdlog::get("file_log")->info("Using default fragment shader");
	}

	// Shader setup
	shader = new Shader(vertexShaderPath, fragmentShaderPath);

	// Load mesh
	if (modelPath.empty()) {
		mesh = new Mesh();
	}
	else {
		mesh = new Mesh(modelPath);
	}
	
	// Extract texture file paths
	material = new Material(mesh);

	//	******** MAIN LOOP ********
	while (!glfwWindowShouldClose(window)) {
		
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
			spdlog::get("file_log")->error(e.what());
		}

		// FPS counter and limiter
		// TODO: abstract this to it's own object
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;

		// If time since last frame is more than 1/60th of a second, render
		if (deltaTime > (1.0 / 60.0)) {
			lastFrame = currentTime;

				nbFrames++;
				if (currentTime - lastTime >= 1.0) {
						nbFrames = 0;
						lastTime += 1.0;
				}

			// Event polling
			glfwPollEvents();
			doMovement();

			// Update transformation matrices 
			projection = glm::perspective(camera.GetZoom(),
										  (GLfloat)width / (GLfloat)height, 0.1f, 
										  1000.0f);
			view = camera.GetViewMatrix();
			cameraPosition = camera.getPosition();
			worldTransform = glm::mat4(1.0f);
			worldTransform = glm::rotate(worldTransform, -worldXRotation, right);
			worldTransform = glm::rotate(worldTransform, worldYRotation, up);
			lightWorldPos = worldTransform * glm::vec4(lightModelPos, 1.0f);

			// Clear buffers 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Apply world transformation
			glUniformMatrix4fv(shader->getViewLoc(), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(shader->getProjLoc(), 1, GL_FALSE, 
							   glm::value_ptr(projection));
			glUniform3fv(shader->getCameraPositionLoc(), 1, 
						 glm::value_ptr(cameraPosition));
			glUniformMatrix4fv(shader->getModelLoc(), 1, GL_FALSE, 
							   glm::value_ptr(worldTransform));
			glUniform3fv(shader->getLightLoc(), 1, glm::value_ptr(lightWorldPos));
			glUniform1f(shader->getAmbLightLoc(), ambLight);

			// Iterate through all meshes
			for (int i = 0; i < mesh->getScene()->mNumMeshes; i++) {

				// If material has a texture, bind it
				if (material->hasTexture(i)) {
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, material->getTexture(i));
					glUniform1f(shader->getTextureBoolLoc(), 1);
				}
				else {
					glUniform1f(shader->getTextureBoolLoc(), 0);
				}

				// Bind material color and reflection characteristics
				glUniform3fv(shader->getColorLoc(), 1, 
							 glm::value_ptr(mesh->getModelColor()[i]));
				glUniform1f(shader->getReflectionCoefficientLoc(), 
							mesh->getReflectionCoefficient(i));
				glUniform1f(shader->getReflectionExponentLoc(), 
						    mesh->getReflectionExponent(i));
				glUniform1f(shader->getSpecularLoc(), mesh->getSpecular(i));

				//Bind vertex array
				glBindVertexArray(mesh->getVAO()[i]);

				// Apply transformations to the mesh
				meshTransform = glm::translate(glm::mat4(), 
											   glm::vec3(0.0f, 0.0f, 0.0f));
				// Apply dynamic transforms
				// TODO: Make dynamic transformations assignable instead of hardcoded
				if (i == 0) {
					meshTransform = glm::scale(meshTransform, 
											   glm::vec3(1 + inData[i] * 0.01, 
														 1 + inData[i] * 0.01,	
														 1 + inData[i] * 0.01));
					meshTransform = glm::rotate(meshTransform, 
												(float)fmod(glfwGetTime() / 10, 360), 
											    glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else if (i == 1) {
					meshTransform = glm::translate(meshTransform, 
												   glm::vec3(0, inData[i] * 0.01, 0));
					meshTransform = glm::rotate(meshTransform, 
												(float)fmod(glfwGetTime() / 10, 360), 
												glm::vec3(0.0f, 1.0f, 0.0f));
				}
				else if (i == 2) {
					meshTransform = glm::rotate(meshTransform, 
												(float)fmod(glfwGetTime() / 10, 360), 
												glm::vec3(0.0f, -1.0f, 0.0f));
				}
				else if (i == 3) {
					meshTransform = glm::rotate(meshTransform, 
											    (float)fmod(glfwGetTime() / 10, 360), 
												glm::vec3(0.0f, -1.0f, 0.0f));
				};
				meshTransform = worldTransform * meshTransform;

				//Bind transformation matrix
				glUniformMatrix4fv(shader->getModelLoc(), 1, GL_FALSE, 
								   glm::value_ptr(meshTransform));

				// Render scene
				glDrawElements(GL_TRIANGLES, 
							   mesh->getScene()->mMeshes[i]->mNumFaces * 3, 
							   GL_UNSIGNED_INT, nullptr);
				glBindVertexArray(0);
			}
			glfwSwapBuffers(window);
		}
	}

	// Properly de-allocate all resources once they've outlived their purpose
	for (int i = 0; i < mesh->getScene()->mNumMeshes; i++) {
		glDeleteVertexArrays(1, &(mesh->getVAO()[i]));
		glDeleteBuffers(1, &(mesh->getVBO()[i]));
		glDeleteBuffers(1, &(mesh->getNormVBO()[i]));
		glDeleteBuffers(1, &(mesh->getTexVBO()[i]));
		glDeleteBuffers(1, &(mesh->getEBO()[i]));
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}

int init() {
	spdlog::get("file_log")->info("Starting GLFW context, OpenGL 3.3");
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
	if (window == nullptr) {
		spdlog::get("file_log")->error("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		spdlog::get("file_log")->error("Failed to initialize GLEW");
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
	camera.ProcessMouseMovement(sin(glfwGetTime() / 4) * 0.005, 
								cos(glfwGetTime() / 8)* 0.005, 0);
	worldXRotation -= sin(glfwGetTime() * 0.25)  * 0.0005;

	if (keys[GLFW_KEY_UP]) {
		worldXRotation += velocity * deltaTime;
	}
	if (keys[GLFW_KEY_DOWN]) {
		worldXRotation -= velocity * deltaTime;
	}
	if (keys[GLFW_KEY_LEFT]) {
		worldYRotation += velocity * deltaTime;
	}
	if (keys[GLFW_KEY_RIGHT]) {
		worldYRotation -= velocity * deltaTime;
	}
	if (keys[GLFW_KEY_W]) {
		camera.ProcessKeyboard(FORWARD, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_S]) {
		camera.ProcessKeyboard(BACKWARD, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_A]) {
		camera.ProcessKeyboard(LEFT, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_D]) {
		camera.ProcessKeyboard(RIGHT, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_9]) {
		camera.ProcessKeyboard(TOP, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_3]) {
		camera.ProcessKeyboard(BOTTOM, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_8]) {
		camera.ProcessKeyboard(LOOKUP, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_2]) {
		camera.ProcessKeyboard(LOOKDOWN, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_4]) {
		camera.ProcessKeyboard(LOOKLEFT, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_6]) {
		camera.ProcessKeyboard(LOOKRIGHT, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_0]) {
		camera.ProcessKeyboard(YAWLEFT, velocity * deltaTime);
	}
	if (keys[GLFW_KEY_KP_DECIMAL]) {
		camera.ProcessKeyboard(YAWRIGHT, velocity * deltaTime);
	}
};

void framebuffer_size_callback(GLFWwindow* window, int newwidth, int newheight) {
	glViewport(0, 0, newwidth, newheight);
	height = newheight;
	width = newwidth;
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	xOffset = xPos - lastX;
	yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	if (mousePressed[0]) {
		camera.ProcessMouseMovement(xOffset, yOffset, 0);
	}
	if (mousePressed[1]) {
		camera.ProcessMouseMovement(xOffset, yOffset, 1);
	}
	if (mousePressed[2]) {
		camera.ProcessMouseMovement(xOffset, yOffset, 2);
	}
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.ProcessMouseScroll(yOffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
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
		case GLFW_KEY_TAB:
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			break;
		}
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	glfwGetCursorPos(window, &xpos, &ypos);
	lastX = xpos;
	lastY = ypos;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mousePressed[0] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mousePressed[0] = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		mousePressed[1] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		mousePressed[1] = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		mousePressed[2] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		mousePressed[2] = false;
	}
}