#pragma once

#include "Shader.h"
#include "Camera.h"
#include "Object.h"
#include "Cube.h"
#include "Mesh.h"
#include "Material.h"

const bool debug = false;
const bool fps_count = false;

const GLuint WIN_WIDTH = 1920, WIN_HEIGHT = 1080;
GLFWwindow* window;
GLclampf* redbk = new GLclampf(0.0f);
GLclampf* greenbk = new GLclampf(1.0f);
GLclampf* bluebk = new GLclampf(0.0f);
GLfloat worldYRotation = 0.0f;
GLfloat worldXRotation = 0.0f;
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 right = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
GLdouble lastTime = glfwGetTime();
GLuint nbFrames = 0;
Camera camera = Camera(0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, -90.0f, -89.0f);
GLfloat ambLight = 0.1f;
glm::vec3 lightModelPos = glm::vec3(-10.0f, 20.0f, -20.0f);
glm::vec3 lightWorldPos;
glm::mat4 projection;
glm::mat4 view;
glm::vec3 cameraPosition;
glm::mat4 worldTransform;
glm::mat4 meshTransform;
Mesh* mesh;
Shader* shader;
Material* material;

GLfloat lastX = WIN_WIDTH / 2.0f;
GLfloat lastY = WIN_WIDTH / 2.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool keys[1024];
bool mousePressed[3];
int width = WIN_HEIGHT;
int height = WIN_HEIGHT;
GLenum renderMode = GL_FILL;

int inDataLenght;
float inData[128];
float lastFrameData[128];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int newwidth, int newheight);
void doMovement();

int init();
int main();