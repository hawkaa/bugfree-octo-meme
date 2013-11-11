#pragma once
#include <GL\glfw3.h>

class Input
{
public:
	Input(GLFWwindow* window);
	~Input(void);

	void update();

	void mouseCallBack(GLFWwindow* window, int button, int pressed, int modifier);
	void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int modifier);

	float getMouseX();
	float getMouseY();

	bool isKeyPressed(char);

private:
	GLFWwindow* window;

	double mousex;
	double mousey;

	bool keys[360];
	bool mouse[0xF];
	
};

