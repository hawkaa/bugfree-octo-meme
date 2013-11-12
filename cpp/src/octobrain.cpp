#include "octobrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>

#include <GL/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLFW_DLL

const char* Octobrain::objectsFileName = "objects.txt";
const char* Octobrain::textureFileNames[10] = {"textures/0.bmp", "textures/1.bmp", "textures/2.bmp", "textures/3.bmp", "textures/4.bmp", "textures/5.bmp", "textures/6.bmp", "textures/7.bmp", "textures/8.bmp", "textures/9.bmp"};

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
	this->loader = new Loader();
	this->camera = new Camera(this->window, this->input);
	this->renderer = new Renderer(this->loader, this->camera);	
	this->loader->setRenderer(this->renderer);
	this->loader->loadObjectsFromFile(Octobrain::objectsFileName);
	this->text = new Text();

	for(int i = 0; i < 10; ++i)
	{
		text->addNumberTexture(this->loader->loadBMP(textureFileNames[i]), i);
	}

	text->addNumber(250, glm::vec3(54,3,5));
}

void Octobrain::run()
{
	glClearColor(0.13f, 0.13f, 0.13f, 1.0f);

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
		glfwSetCursorPos(window, Octobrain::screenWidth/2, Octobrain::screenHeight/2);
		renderer->invalidate();
		
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

