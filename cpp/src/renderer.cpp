#include "renderer.h"
#include "loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

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

	glUseProgram(this->programID);

	for(it = activeMeshes.begin(); it != activeMeshes.end(); ++it)
	{

		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::mat4 projectionMatrix = camera->getProjectionMatrix();

		glm::mat4 rotMat, scaleMat, transMat;

		rotMat = glm::rotate(glm::mat4(1.0f), it->rotation.x, glm::vec3(1,0,0));
		rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.y, glm::vec3(0,1,0));
		rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.z, glm::vec3(0,0,1));

		scaleMat = glm::scale(glm::mat4(1.0f), it->scale);
		transMat = glm::translate(glm::mat4(1.0f), it->translation);

		glm::mat4 modelMatrix = transMat * scaleMat * rotMat;
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

void Renderer::addImageObject(ImageObject& object)
{
	switch(object.getType())
	{
	case CIRCLE:
		createEllipsoid(object.getX(), object.getY(), object.getRadius(), object.getColor());
		break;
	case TRIANGLE:
	case SQUARE:
	case POLYGON:
		break;
	}
}

void Renderer::createEllipsoid(float x, float y, float radius, glm::vec4 color)
{
	this->startMesh();

	radius = radius/25;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;
	std::vector<unsigned int> indices;

	int a, b;
	
	a = radius;
	b = 2;
	int res = 32;

	for(float i = -radius; i < radius+2*radius/res; i += 2*radius/res)
	{
		for(float theta = 0; theta < 2*3.14f-2*3.14/res; theta += 2*3.14/res)
		{
			float in = i/radius;

			//Ninjafix
			if(in > 1)
			{
				in = 1;
			}

			glm::vec3 p = glm::vec3(sgn(radius)*sqrt(1-in*in)*cos(theta), i, sgn(radius)*sqrt(1-in*in)*sin(theta));
			vertices.push_back(p);
			normals.push_back(-p);
			colors.push_back(glm::vec4(p,1));
		}
	}

	for(int i = 0; i < vertices.size()-res; ++i)
	{
		indices.push_back(i);
		indices.push_back(i+res);
		indices.push_back(i+1);

		indices.push_back(i+res);
		indices.push_back(i+res+1);
		indices.push_back(i+1);
	}

	for(int i = 0; i < vertices.size(); ++i)
	{
		this->addPointToMesh(vertices[i], colors[i], normals[i]);
	}

	for(int j = 0; j < indices.size(); j+=3)
	{
		this->addTriangleToMesh(indices[j], indices[j+1], indices[j+2]);
	}

	//this->addRotationToMesh(glm::vec3(0,90,0));
	this->addTranslationToMesh(glm::vec3(x, y, 0)*(0.04f));
	this->commitMesh();
}

void Renderer::startMesh()
{
	++currentid;
	meshstarted = true;
	uncommitedMesh.translation = glm::vec3(1.0f);
	uncommitedMesh.rotation = glm::vec3(1.0f);
	uncommitedMesh.scale = glm::vec3(1.0f);
}

void Renderer::addPointToMesh(glm::vec3 &vertex, glm::vec4 &color, glm::vec3 &normal)
{	
	this->uncommitedMesh.vertices.push_back(vertex);
	this->uncommitedMesh.colors.push_back(color);
	this->uncommitedMesh.normals.push_back(normal);
}

void Renderer::addTriangleToMesh(unsigned int p1, unsigned int p2, unsigned int p3)
{
	this->uncommitedMesh.indices.push_back(p1);
	this->uncommitedMesh.indices.push_back(p2);
	this->uncommitedMesh.indices.push_back(p3);
}

void Renderer::addTranslationToMesh(glm::vec3 translation)
{
	this->uncommitedMesh.translation = translation;
}

void Renderer::addRotationToMesh(glm::vec3 rotation)
{
	this->uncommitedMesh.rotation = rotation;
}

void Renderer::addScaleToMesh(glm::vec3 scale)
{
	this->uncommitedMesh.scale = scale;
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

	glGenBuffers(1, &buffer.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*uncommitedMesh.colors.size(), &uncommitedMesh.colors[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &buffer.indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer.indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int)*uncommitedMesh.indices.size(), &uncommitedMesh.indices[0], GL_STATIC_DRAW);

	buffer.indexSize = uncommitedMesh.indices.size();
	buffer.translation = uncommitedMesh.translation;
	buffer.rotation = uncommitedMesh.rotation;
	buffer.scale = uncommitedMesh.scale;

	printf("Mesh commited with %d vertices, %d normals, %d colorpoints, %d indices", this->uncommitedMesh.vertices.size(), this->uncommitedMesh.colors.size(), this->uncommitedMesh.normals.size(), this->uncommitedMesh.indices.size());
	activeMeshes.push_back(buffer);

	uncommitedMesh.vertices.clear();
	uncommitedMesh.colors.clear();
	uncommitedMesh.normals.clear();
	uncommitedMesh.indices.clear();

	this->meshstarted = false;
	return currentid;	
}
