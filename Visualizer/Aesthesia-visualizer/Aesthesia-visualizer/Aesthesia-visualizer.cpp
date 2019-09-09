// Aesthesia-visualizer.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include "Aesthesia-visualizer.h"

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
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Lab", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
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
	//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glOrtho(0, width, 0, height, 0.1, 1000); // essentially set coordinate system
	glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glLoadIdentity(); // same as above comment

	glPointSize(5.0);
	glEnable(GL_CULL_FACE);
}

int main()
{

	*bluebk = 0.5f;
	*redbk = 0.5f;
	*greenbk = 0.5f;

	if (init() < 0) {
		return -1;
	}

	Shader shader = Shader("vertex.shader", "fragment.shader");

//	Shader shadowShader = Shader("svertex.shader", "sfragment.shader");

	//	Define cube vertices
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
	for (int i = 0; i < indices.size(); i += 3) {
		normals.push_back(normalize(glm::cross(vertices[indices[i + 1]] - vertices[indices[i]], vertices[indices[i + 2]] - vertices[indices[i]])));
		normals.push_back(normalize(glm::cross(vertices[indices[i + 2]] - vertices[indices[i + 1]], vertices[indices[i]] - vertices[indices[i + 1]])));
		normals.push_back(normalize(glm::cross(vertices[indices[i]] - vertices[indices[i + 2]], vertices[indices[i + 1]] - vertices[indices[i + 2]])));
	}



	//	Define grid vertices
	std::vector<glm::vec3> grid{
		glm::vec3(-50.0, 0.0, -50.0), glm::vec3(-50.0, 0.0, 50.0), glm::vec3(50.0, 0.0, -50.0),
		glm::vec3(50.0, 0.0, 50.0), glm::vec3(50.0, 0.0, -50.0), glm::vec3(-50.0, 0.0, 50.0)
	};

	std::vector<glm::vec3> gridNormals{
		glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0)
	};
	std::vector<glm::vec2> texCoordg{
		glm::vec2(0.0, 0.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, 1.0),
		glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0), glm::vec2(0.0, 1.0)
	};


	//	Define axes vertices
	std::vector<glm::vec3> axes{
		glm::vec3(0.01, 0.01, 0.01), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.01, 0.01, 5.0), glm::vec3(0.0, 0.0, 1.0),
		glm::vec3(0.01, 0.01, 0.01), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.01, 5.0, 0.01), glm::vec3(0.0, 1.0, 0.0),
		glm::vec3(0.01, 0.01, 0.01), glm::vec3(1.0, 0.0, 0.0), glm::vec3(5.0, 0.01, 0.01), glm::vec3(1.0, 0.0, 0.0),
	};


	//	Light source VBO and VAO binding
	GLuint VBOl, VAOl, normVBOl, EBOl;
	glGenVertexArrays(1, &VAOl);
	glBindVertexArray(VAOl);
	glGenBuffers(1, &normVBOl);
	glGenBuffers(1, &EBOl);
	glGenBuffers(1, &VBOl);

	glBindBuffer(GL_ARRAY_BUFFER, VBOl);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, normVBOl);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOl);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_DYNAMIC_DRAW);


	glBindVertexArray(0);


	//	Axes VBO and VAO binding
	GLuint VBOa, VAOa;
	glGenVertexArrays(1, &VAOa);
	glBindVertexArray(VAOa);
	glGenBuffers(1, &VBOa);
	glBindBuffer(GL_ARRAY_BUFFER, VBOa);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*axes.size(), &axes[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);


	//	Grid VBO and VAO binding
	GLuint VBOg, VAOg, normVBOg, texVBOg;
	glGenVertexArrays(1, &VAOg);
	glBindVertexArray(VAOg);
	glGenBuffers(1, &normVBOg);
	glGenBuffers(1, &VBOg);
	glBindBuffer(GL_ARRAY_BUFFER, VBOg);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*grid.size(), &grid[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normVBOg);
	glBufferData(GL_ARRAY_BUFFER, gridNormals.size() * sizeof(glm::vec3), &gridNormals[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &texVBOg);
	glBindBuffer(GL_ARRAY_BUFFER, texVBOg);
	glBufferData(GL_ARRAY_BUFFER, texCoordg.size() * sizeof(glm::vec2), &texCoordg[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);


	//	Cube VBO and VAO binding
	GLuint VBO, VAO, normVBO, texVBO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, texCoord.size() * sizeof(glm::vec2), &texCoord[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//texture setup
	glActiveTexture(GL_TEXTURE0);
	GLuint cube_texture;
	glGenTextures(1, &cube_texture);
	glBindTexture(GL_TEXTURE_2D, cube_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int cube_texture_width, cube_texture_height;
	unsigned char* horseTex = SOIL_load_image("assets/horse.jpg", &cube_texture_width, &cube_texture_height, 0, SOIL_LOAD_RGB);
	unsigned char* grassTex = SOIL_load_image("assets/grass.jpg", &cube_texture_width, &cube_texture_height, 0, SOIL_LOAD_RGB);


	//Shadowmap setup

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	//	Defining horse
	std::vector<Object*> horse;
	Object torso(glm::vec3(7.0f, 3.0f, 2.0f), glm::vec3(0.0f, 6.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), nullptr, glm::vec3(0.0f, -6.0f, 0.0f));
	Object neck(glm::vec3(3.0f, 2.0f, 1.5f), glm::vec3(-4.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -0.5f), &torso, glm::vec3(1.5f, 0.0f, 0.0f));
	Object head(glm::vec3(2.75f, 1.5f, 1.25f), glm::vec3(-2.25, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), &neck, glm::vec3(1.375f, 0.0f, 0.0f));
	Object leg1(glm::vec3(3.0f, 0.75f, 0.75f), glm::vec3(-4.5f, -1.5f, 0.75f), glm::vec3(0.0f, 0.0f, 1.57f), &torso, glm::vec3(1.5f, 0.0f, 0.0f));
	Object leg2(glm::vec3(3.0f, 0.75f, 0.75f), glm::vec3(-4.5f, -1.5f, -0.75f), glm::vec3(0.0f, 0.0f, 1.57f), &torso, glm::vec3(1.5f, 0.0f, 0.0f));
	Object leg3(glm::vec3(3.0f, 0.75f, 0.75f), glm::vec3(1.5f, -1.5f, 0.75f), glm::vec3(0.0f, 0.0f, 1.57f), &torso, glm::vec3(1.5f, 0.0f, 0.0f));
	Object leg4(glm::vec3(3.0f, 0.75f, 0.75f), glm::vec3(1.5f, -1.5f, -0.75f), glm::vec3(0.0f, 0.0f, 1.57f), &torso, glm::vec3(1.5f, 0.0f, 0.0f));
	Object calf1(glm::vec3(2.0f, 0.55f, 0.55f), glm::vec3(-2.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), &leg1, glm::vec3(1.0f, 0.0f, 0.0f));
	Object calf2(glm::vec3(2.0f, 0.55f, 0.55f), glm::vec3(-2.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), &leg2, glm::vec3(1.0f, 0.0f, 0.0f));
	Object calf3(glm::vec3(2.0f, 0.55f, 0.55f), glm::vec3(-2.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), &leg3, glm::vec3(1.0f, 0.0f, 0.0f));
	Object calf4(glm::vec3(2.0f, 0.55f, 0.55f), glm::vec3(-2.0f, -0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), &leg4, glm::vec3(1.0f, 0.0f, 0.0f));

	horse.push_back(&torso);
	horse.push_back(&neck);
	horse.push_back(&head);
	horse.push_back(&leg1);
	horse.push_back(&leg2);
	horse.push_back(&leg3);
	horse.push_back(&leg4);
	horse.push_back(&calf1);
	horse.push_back(&calf2);
	horse.push_back(&calf3);
	horse.push_back(&calf4);

	for (int i = 0; i < horse.size(); i++) {
		horseJointRotation.push_back(0.0);
	}



	glm::mat4 transformationStack;
	glm::mat4 currentTransform;
	Object* transformationPointer;
	std::vector<glm::mat4> transformationVec;


	//	******** MAIN LOOP ********

	while (!glfwWindowShouldClose(window))

		//	FPS counter
	{
		GLdouble currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		nbFrames++;
		if (currentTime - lastTime >= 1.0) {
			std::cout << nbFrames << " - ";
			nbFrames = 0;
			lastTime += 1.0;
		}

		//	Event polling
		glfwPollEvents();
		doMovement();

		glClearColor(*redbk, *greenbk, *bluebk, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//

		glm::vec3 light = glm::vec3(0.0f, 20.0f, 0.0f);			// *** LIGHT COORDS ***

		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 color;
		glm::vec3 cameraPosition = camera.getPosition();
		glm::mat4 worldTransform;
		glm::mat4 lightSpaceMatrix = glm::perspective(lightCamera.GetZoom(), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f) * lightCamera.GetViewMatrix();


		GLfloat ambLight;


		GLint modelLoc = glGetUniformLocation(shader.getProgramID(), "model");
		GLint viewLoc = glGetUniformLocation(shader.getProgramID(), "view");
		GLint projLoc = glGetUniformLocation(shader.getProgramID(), "projection");
		GLint lightLoc = glGetUniformLocation(shader.getProgramID(), "light");
		GLint colorLoc = glGetUniformLocation(shader.getProgramID(), "color");
		GLint ambLightLoc = glGetUniformLocation(shader.getProgramID(), "ambLight");
		GLint cameraPositionLoc = glGetUniformLocation(shader.getProgramID(), "cameraPosition");
		GLint textureBoolLoc = glGetUniformLocation(shader.getProgramID(), "textureBool");
		GLint lightSpaceMatrixLoc2 = glGetUniformLocation(shader.getProgramID(), "lightSpaceMatrix");
//		GLuint lightSpaceMatrixLoc = glGetUniformLocation(shadowShader.getProgramID(), "lightSpaceMatrix");
//		GLuint shadowModelLoc = glGetUniformLocation(shadowShader.getProgramID(), "model");


		worldTransform = glm::rotate(worldTransform, -worldXRotation, right);
		worldTransform = glm::rotate(worldTransform, worldYRotation, up);
		light = worldTransform * glm::vec4(light, 1.0f);

		//



				// 1. first render to depth map

		if (debug) {
			glViewport(0, 0, width, height);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else {
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		//	ConfigureShader and matrices
//		glUseProgram(shadowShader.getProgramID());

//		glUniformMatrix4fv(lightSpaceMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
//		glUniformMatrix4fv(shadowModelLoc, 1, GL_FALSE, glm::value_ptr(worldTransform));


		//	Define object transformations
		glm::mat4 horseTransform;

		horseTransform = glm::translate(horseTransform, horsePosition);
		horseTransform = glm::rotate(horseTransform, horseYRotation, glm::vec3(0.0f, 1.0f, 0.0f));
		horseTransform = glm::rotate(horseTransform, horseZRotation, glm::vec3(0.0f, 0.0f, 1.0f));
		horseTransform = glm::scale(horseTransform, horseScale);

		//	Drawing grid
		glBindVertexArray(VAOg);
		glPolygonMode(GL_FRONT_AND_BACK, renderMode);
		glDrawArrays(GL_TRIANGLES, 0, grid.size());

		glBindVertexArray(0);

		//	Drawing loop
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, renderMode);

		for (int i = 0; i < horse.size(); i++) {

			horse[i]->rotate(horseJointRotation[i]);
			horseJointRotation[i] = 0.0;

			transformationStack = glm::mat4();
			transformationPointer = horse[i];
			transformationVec.clear();

			currentTransform = glm::mat4();
			currentTransform = glm::translate(currentTransform, transformationPointer->getJointWithParent());
			currentTransform = currentTransform * transformationPointer->getRotation();
			currentTransform = glm::translate(currentTransform, -(transformationPointer->getJointWithParent()));
			transformationVec.push_back(currentTransform);

			currentTransform = glm::mat4();
			currentTransform = glm::translate(currentTransform, transformationPointer->getLocation());
			transformationVec.push_back(currentTransform);

			while (transformationPointer->getParent() != NULL) {
				transformationPointer = transformationPointer->getParent();

				currentTransform = glm::mat4();
				currentTransform = glm::translate(currentTransform, transformationPointer->getJointWithParent());
				currentTransform = currentTransform * transformationPointer->getRotation();
				currentTransform = glm::translate(currentTransform, -(transformationPointer->getJointWithParent()));
				transformationVec.push_back(currentTransform);

				currentTransform = glm::mat4();
				currentTransform = glm::translate(currentTransform, transformationPointer->getLocation());
				transformationVec.push_back(currentTransform);
			}

			currentTransform = glm::mat4();
			for (int i = transformationVec.size() - 1; i >= 0; i--) {
				currentTransform = currentTransform * transformationVec[i];
			}


			currentTransform = glm::scale(currentTransform, horse[i]->getDimensions());

//			glUniformMatrix4fv(shadowModelLoc, 1, GL_FALSE, glm::value_ptr(worldTransform * horseTransform * currentTransform));

			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (debug) {
			glBindVertexArray(0);
			glfwSwapBuffers(window);
		}


		if (!debug) {
			// 2. then render scene as normal with shadow mapping (using depth map)
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(shader.getProgramID());

			glBindTexture(GL_TEXTURE_2D, depthMap);

			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3fv(cameraPositionLoc, 1, glm::value_ptr(cameraPosition));
			glUniform1f(textureBoolLoc, textureBool);

			glUniform3fv(lightLoc, 1, glm::value_ptr(light));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(worldTransform));

			//	Drawing grid
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cube_texture_width, cube_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, grassTex);
			color = glm::vec3(0.0f, 0.7f, 0.0f);
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			ambLight = 0.2f;
			glUniform1f(ambLightLoc, ambLight);
			glBindVertexArray(VAOg);
			glPolygonMode(GL_FRONT_AND_BACK, renderMode);
			glDrawArrays(GL_TRIANGLES, 0, grid.size());

			glBindVertexArray(0);


			//	Drawing axes
			color = glm::vec3(0.0f, 0.0f, 1.0f);
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			ambLight = 1.0f;
			glUniform1f(ambLightLoc, ambLight);
			glBindVertexArray(VAOa);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawArrays(GL_LINES, 0, axes.size());

			glBindVertexArray(0);

			// Drawing lightSource
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			ambLight = 1.0f;
			glUniform1f(ambLightLoc, ambLight);
			glBindVertexArray(VAOl);
			glPolygonMode(GL_FRONT_AND_BACK, renderMode);

			glm::mat4 lightTransform = glm::translate(glm::mat4(), glm::vec3(0.0f, 20.0f, 0.0f));
			lightTransform = worldTransform * lightTransform;

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightTransform));
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

			glBindVertexArray(0);



			//	Drawing loop
			glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, renderMode);
			color = glm::vec3(0.7f, 0.2f, 0.1f);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cube_texture_width, cube_texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, horseTex);
			glUniform3fv(colorLoc, 1, glm::value_ptr(color));
			ambLight = 0.2f;
			glUniform1f(ambLightLoc, ambLight);

			for (int i = 0; i < horse.size(); i++) {

				horse[i]->rotate(horseJointRotation[i]);
				horseJointRotation[i] = 0.0;

				transformationStack = glm::mat4();
				transformationPointer = horse[i];
				transformationVec.clear();

				currentTransform = glm::mat4();
				currentTransform = glm::translate(currentTransform, transformationPointer->getJointWithParent());
				currentTransform = currentTransform * transformationPointer->getRotation();
				currentTransform = glm::translate(currentTransform, -(transformationPointer->getJointWithParent()));
				transformationVec.push_back(currentTransform);

				currentTransform = glm::mat4();
				currentTransform = glm::translate(currentTransform, transformationPointer->getLocation());
				transformationVec.push_back(currentTransform);

				while (transformationPointer->getParent() != NULL) {
					transformationPointer = transformationPointer->getParent();

					currentTransform = glm::mat4();
					currentTransform = glm::translate(currentTransform, transformationPointer->getJointWithParent());
					currentTransform = currentTransform * transformationPointer->getRotation();
					currentTransform = glm::translate(currentTransform, -(transformationPointer->getJointWithParent()));
					transformationVec.push_back(currentTransform);

					currentTransform = glm::mat4();
					currentTransform = glm::translate(currentTransform, transformationPointer->getLocation());
					transformationVec.push_back(currentTransform);
				}

				currentTransform = glm::mat4();
				for (int i = transformationVec.size() - 1; i >= 0; i--) {
					currentTransform = currentTransform * transformationVec[i];
				}


				currentTransform = glm::scale(currentTransform, horse[i]->getDimensions());

				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(worldTransform * horseTransform * currentTransform));

				glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
			}

			glBindVertexArray(0);
			glfwSwapBuffers(window);

		}

	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return EXIT_SUCCESS;
}

void drawHorse() {

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

void framebuffer_size_callback(GLFWwindow* window, int newwidth, int newheight) {
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
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
		case GLFW_KEY_SPACE:
			horsePosition = glm::vec3(rand() % 100 - 50.0f, 0.0f, rand() % 100 - 50.0f);
			break;
		case GLFW_KEY_U:
			horseScale = glm::vec3(horseScale[0] + scaleSpeed, horseScale[1] + scaleSpeed, horseScale[2] + scaleSpeed);
			break;
		case GLFW_KEY_J:
			horseScale = glm::vec3(horseScale[0] - scaleSpeed, horseScale[1] - scaleSpeed, horseScale[2] - scaleSpeed);
			break;
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
		case GLFW_KEY_A:
			if (mode == GLFW_MOD_SHIFT)
				horsePosition = glm::vec3(horsePosition[0] - positionSpeed, horsePosition[1], horsePosition[2]);
			else
				horseYRotation += rotationSpeed;
			break;
		case GLFW_KEY_D:
			if (mode == GLFW_MOD_SHIFT)
				horsePosition = glm::vec3(horsePosition[0] + positionSpeed, horsePosition[1], horsePosition[2]);
			else
				horseYRotation -= rotationSpeed;
			break;
		case GLFW_KEY_W:
			if (mode == GLFW_MOD_SHIFT)
				horsePosition = glm::vec3(horsePosition[0], horsePosition[1], horsePosition[2] + positionSpeed);
			else
				horseZRotation += rotationSpeed;
			break;
		case GLFW_KEY_S:
			if (mode == GLFW_MOD_SHIFT)
				horsePosition = glm::vec3(horsePosition[0], horsePosition[1], horsePosition[2] - positionSpeed);
			else
				horseZRotation -= rotationSpeed;
			break;
		case GLFW_KEY_HOME:
			camera.resetCamera();
			worldXRotation = 0;
			worldYRotation = 0;
			break;
		case GLFW_KEY_0:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[10] -= rotationSpeed;
			else
				horseJointRotation[10] += rotationSpeed;
			break;
		case GLFW_KEY_1:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[2] -= rotationSpeed;
			else
				horseJointRotation[2] += rotationSpeed;
			break;
		case GLFW_KEY_2:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[1] -= rotationSpeed;
			else
				horseJointRotation[1] += rotationSpeed;
			break;
		case GLFW_KEY_3:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[3] -= rotationSpeed;
			else
				horseJointRotation[3] += rotationSpeed;
			break;
		case GLFW_KEY_4:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[7] -= rotationSpeed;
			else
				horseJointRotation[7] += rotationSpeed;
			break;
		case GLFW_KEY_5:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[4] -= rotationSpeed;
			else
				horseJointRotation[4] += rotationSpeed;
			break;
		case GLFW_KEY_6:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[8] -= rotationSpeed;
			else
				horseJointRotation[8] += rotationSpeed;
			break;
		case GLFW_KEY_7:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[5] -= rotationSpeed;
			else
				horseJointRotation[5] += rotationSpeed;
			break;
		case GLFW_KEY_8:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[9] -= rotationSpeed;
			else
				horseJointRotation[9] += rotationSpeed;
			break;
		case GLFW_KEY_9:
			if (mode == GLFW_MOD_SHIFT)
				horseJointRotation[6] -= rotationSpeed;
			else
				horseJointRotation[6] += rotationSpeed;
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
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