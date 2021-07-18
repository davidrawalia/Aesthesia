enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	LOOKUP,
	LOOKDOWN,
	LOOKLEFT,
	LOOKRIGHT,
	YAWLEFT,
	YAWRIGHT
};

const GLfloat YAW = -90.f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;


class Camera {

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLint mouseButtonPressed);
	void ProcessMouseScroll(GLfloat yOffset);
	GLfloat GetZoom();
	glm::vec3 getPosition();
	void resetCamera();

private:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Euler Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	void updateCameraVectors();
};