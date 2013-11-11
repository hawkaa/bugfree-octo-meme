#include "camera.h"
#include "octobrain.h"
#include <algorithm>
#include <glm\gtc\matrix_transform.hpp>

float Camera::maxViewY = 3.14f/2;
float Camera::minViewY = -3.14f/2;

float Camera::closeView = 0.1f;
float Camera::farView = 100.0f;

int Camera::fov = 60;

Camera::Camera(GLFWwindow* window, Input* input)
{
	this->window = window;
	this->input = input;
	this->viewX = 3.14f;
	this->viewY = 0.0f;

	this->speed = 5.0f;
	this->mouseSpeed = 1.0f;

	this->pos = glm::vec3(0, 0, 5);
	this->dir = glm::vec3(cos(viewY)*sin(viewX), sin(viewY), cos(viewY)*cos(viewX));
	this->right = glm::vec3(cos(viewX), 0, -sin(viewX));

	up = glm::cross(dir, right);

	viewMatrix = glm::lookAt(pos, pos+dir, right);
	projectionMatrix = glm::perspective(float(fov), (float)Octobrain::screenWidth/(float)Octobrain::screenHeight, this->closeView, this->farView);
}

Camera::~Camera(void)
{

}


void Camera::move(float dt)
{
	double mouseX, mouseY;

	mouseX = input->getMouseX();
	mouseY = input->getMouseY();

	viewX += dt*mouseSpeed*float(Octobrain::screenWidth/2 - mouseX);
	viewY += dt*mouseSpeed*float(Octobrain::screenHeight/2 - mouseY);

	viewY = (viewY > Camera::maxViewY) ? Camera::maxViewY : (viewY < Camera::minViewY) ? Camera::minViewY : viewY;

	dir = glm::vec3(cos(viewY)*sin(viewX), sin(viewY), cos(viewY)*cos(viewX));
	right = glm::vec3(cos(viewX), 0, -sin(viewX));
	up = glm::cross(dir, right);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		pos += dir * dt * speed;
	}

	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		pos -= dir * dt * speed;
	}

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		pos += right * dt * speed;
	}

	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		pos -= right * dt * speed;
	}

	if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		pos += glm::vec3(0,1,0)*dt*speed;
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		pos -= glm::vec3(0,1,0)*dt*speed;
	}

	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		speed += 5;
	}

	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		if(speed > 0)
			speed -= 5;
	}

	viewMatrix = glm::lookAt(pos, pos + dir, up);
	printf("Pos: %f %f %f dt: %f\r", pos.x, pos.y, pos.z, dt);
}

glm::mat4& Camera::getViewMatrix()
{
	return this->viewMatrix;
}

glm::mat4& Camera::getProjectionMatrix()
{
	return this->projectionMatrix;
}

glm::vec3& Camera::getPos()
{
	return this->pos;
}

glm::vec3& Camera::getDir()
{
	return this->dir;
}





