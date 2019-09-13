class Object {
public:
	Object(glm::vec3 aDimensions, glm::vec3 aLocationFromJoint, glm::vec3 aRotation, Object* aParent, glm::vec3 aJointWithParent);
	glm::vec3 getDimensions();
	glm::vec3 getLocation();
	glm::vec3 getJointWithParent();
	glm::mat4 getRotation();
	glm::mat4 getTranformation();
	std::vector<Object *> getChildren();
	void pushChildren(Object* aChild);
	void rotate(float angle);
	Object* getParent();

private:
	glm::vec3 dimensions;
	glm::vec3 locationFromJoint;
	glm::vec3 jointWithParent;
	glm::vec3 rotation;
	std::vector<Object *> children;
	Object* parent;
};