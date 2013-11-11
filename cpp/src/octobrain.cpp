#include "octobrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_DLL


Octobrain::Octobrain(void)
{

}

Octobrain::~Octobrain(void)
{

}

void Octobrain::init()
{
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW");
	}

	this->window = glfwCreateWindow(screenWidth, screenHeight, "BugfreeOctoMeme", NULL, NULL);

	if(!this->window)
	{
		glfwTerminate();
		fprintf(stderr, "Unable to create window");
	}

	glfwSetWindowUserPointer(window, this);

	glfwMakeContextCurrent(this->window);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	
	if(GLenum err = glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Error while initializing glew: %s\n", glewGetErrorString(err));
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	this->input = new Input(this->window);
	this->renderer = new Renderer();
	this->camera = new Camera(this->window, this->renderer, this->input);
	
}

void Octobrain::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	double time1, time2;
	float delta;

	time1 = glfwGetTime();
	do
	{
		time2 = glfwGetTime();		
		delta = float(time2-time1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		input->update();
		camera->move(delta);
		renderer->invalidate();

		glfwSetCursorPos(window, Octobrain::screenWidth/2, Octobrain::screenHeight/2);
		glfwSwapBuffers(this->window);
		glfwPollEvents();
		time1 = time2;
	}while(glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(this->window));

	this->cleanup();
}

void Octobrain::cleanup()
{
	delete camera;
	delete input;
	delete renderer;
	glfwDestroyWindow(this->window);
}

