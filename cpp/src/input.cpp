#include "input.h"

Input::Input(GLFWwindow* window)
{
	this->window = window;
}


Input::~Input(void)
{

}

void Input::update()
{
	glfwGetCursorPos(window, &mousex, &mousey); 
}

void Input::mouseCallBack(GLFWwindow* window, int button, int pressed, int modifier)
{
	if(button > 0xFF)
		return;

	if(pressed)
	{
		mouse[button] = true;
	}
	else
	{
		mouse[button] = false;
	}

}

void Input::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int modifier)
{
	if(key > 350)
		return;
	
	if(action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else
	{
		keys[key] = false;
	}
}

float Input::getMouseX()
{
	return mousex;
}

float Input::getMouseY()
{
	return mousey;
}

bool Input::isKeyPressed(char c)
{
	if(c > 350)
		return false;
	return keys[c];
}


