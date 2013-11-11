#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <GL\glfw3.h>
#include "input.h"

class Camera
{
public:
	Camera(GLFWwindow* window, Input* input);
	~Camera(void);

	void move(float dt);

	glm::mat4 &getViewMatrix();
	glm::mat4 &getProjectionMatrix();
	glm::vec3 &getPos();
	glm::vec3 &getDir();

private:

	GLFWwindow* window;
	Input* input;

	static int fov;
	static float closeView;
	static float farView;

	static float maxViewY;
	static float minViewY;
	float viewX;
	float viewY;

	float speed;
	float mouseSpeed;

	glm::vec3 pos;
	glm::vec3 dir;
	glm::vec3 right;
	glm::vec3 up;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

};

