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

	this->renderer->startMesh();
	glm::vec3 p1, p2, p3, p4;
	p1 = glm::vec3(1,0,0);
	p2 = glm::vec3(0,1,0);
	p3 = glm::vec3(0,0,1);
	p4 = glm::vec3(1,0,1);
	glm::vec3 n = glm::cross(p2-p1, p3-p1);
	glm::vec4 c = glm::vec4(1,0,0,1);

	this->renderer->startMesh();
	this->renderer->addPointToMesh(p1,c,n);
	this->renderer->addPointToMesh(p2,c,n);
	this->renderer->addPointToMesh(p3,c,n);
	this->renderer->addTriangleToMesh(1,2,3);
	this->renderer->commitMesh();

	this->renderer->startMesh();
	this->renderer->addPointToMesh(p1,c,n);
	this->renderer->addPointToMesh(p2,c,n);
	this->renderer->addPointToMesh(p3,c,n);
	this->renderer->addPointToMesh(p4,c,n);
	this->renderer->addTriangleToMesh(3,2,1);
	this->renderer->addTriangleToMesh(2,1,4);
	this->renderer->commitMesh();
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

