#include "renderer.h"
#include "loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Loader* loader, Camera* camera)
{
	this->camera = camera;
	this->currentid = 0;
	this->programID = loader->LoadShaders("vertexshader.shader", "fragmentshader.shader");
	
	this->MVPHandle = glGetUniformLocation(this->programID, "MVP");
	this->modelHandle = glGetUniformLocation(this->programID, "Model");
	this->viewHandle = glGetUniformLocation(this->programID, "View");
	
	this->posHandle = glGetAttribLocation(this->programID, "vertexPosition_modelspace");
	this->colorHandle = glGetAttribLocation(this->programID, "vertexColor");
	this->normalHandle = glGetAttribLocation(this->programID, "vertexNormal_modelspace");

	currentid = -1;
}

Renderer::~Renderer(void)
{

}

//Invalidate a specific mesh -> renders it as soon as possible
void Renderer::invalidate(int id)
{
	
}

//Invalidates all meshes -> renders everything as soon as possible
void Renderer::invalidate()
{
	std::vector<MeshBuffer>::iterator it;

	for(it = activeMeshes.begin(); it != activeMeshes.end(); ++it)
	{
		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), it->translation);
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	
		glUniformMatrix4fv(MVPHandle, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelHandle, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(viewHandle, 1, GL_FALSE, &viewMatrix[0][0]);	

		glEnableVertexAttribArray(posHandle);
		glBindBuffer(GL_ARRAY_BUFFER, it->vertexBuffer);
		glVertexAttribPointer(posHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	

		glEnableVertexAttribArray(colorHandle);
		glBindBuffer(GL_ARRAY_BUFFER, it->colorBuffer);
		glVertexAttribPointer(colorHandle, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(normalHandle);
		glBindBuffer(GL_ARRAY_BUFFER, it->normalBuffer);
		glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->indexBuffer);

		glDrawElements(GL_TRIANGLES, it->indexSize, GL_UNSIGNED_INT, (void*)0);

		glDisableVertexAttribArray(posHandle);
		glDisableVertexAttribArray(colorHandle);
		glDisableVertexAttribArray(normalHandle);
	}	
}

void Renderer::startMesh()
{
	++currentid;
	meshstarted = true;
}

void Renderer::addVertexToMesh(glm::vec3 p)
{
	this->uncommitedMesh.vertices.push_back(p);
}

void Renderer::addColorToMesh(glm::vec4 p)
{
	this->uncommitedMesh.colors.push_back(p);

}

void Renderer::addNormalToMesh(glm::vec3 p)
{
	this->uncommitedMesh.normals.push_back(p);
}

void Renderer::addTriangleToMesh(unsigned int p1, unsigned int p2, unsigned int p3)
{
	this->uncommitedMesh.indices.push_back(p1);
	this->uncommitedMesh.indices.push_back(p2);
	this->uncommitedMesh.indices.push_back(p3);
}


int Renderer::commitMesh()
{
	if(this->uncommitedMesh.indices.size() == 0)
	{
		printf("You are trying to commit a mesh without any indices, you suck");
		return -1;
	}

	if(this->uncommitedMesh.colors.size() == 0 || this->uncommitedMesh.vertices.size() == 0 || this->uncommitedMesh.normals.size() == 0)
	{
		printf("You are trying to commit a mesh with some data missing, please make sure that color, normal and vertex data are submitted");
	}

	MeshBuffer buffer;

	glGenBuffers(1, &buffer.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*uncommitedMesh.vertices.size(), &uncommitedMesh.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &buffer.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*uncommitedMesh.normals.size(), &uncommitedMesh.normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &buffer.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*uncommitedMesh.colors.size(), &uncommitedMesh.colors[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &buffer.indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int)*uncommitedMesh.indices.size(), &uncommitedMesh.indices[0], GL_STATIC_DRAW);

	buffer.indexSize = uncommitedMesh.indices.size();
	buffer.translation = glm::vec3(1.0f);
	buffer.rotation = glm::vec3(1.0f);
	buffer.scale = glm::vec3(1.0f);

	uncommitedMesh.vertices.clear();
	uncommitedMesh.colors.clear();
	uncommitedMesh.normals.clear();
	uncommitedMesh.indices.clear();

	this->meshstarted = false;
	return currentid;	
}
