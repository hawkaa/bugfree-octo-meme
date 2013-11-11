#include "renderer.h"


Renderer::Renderer(void)
{
	this->currentid = 0;
}


Renderer::~Renderer(void)
{

}

//Invalidate a specific mesh
void Renderer::invalidate(int id)
{

}

//Invalidates all meshes
void Renderer::invalidate()
{

}

void Renderer::startMesh()
{
	++currentid;
	meshstarted = true;
}

void Renderer::addPointToMesh(glm::vec3 p)
{
	this->uncommitedMesh.vertices.push_back(p);
}


void Renderer::addPointsToMesh(glm::vec3 *points, ...)
{
	glm::vec3 *p;
	
	for(p = points; p != nullptr; ++p)
	{
		this->addPointToMesh(*p);			
	}
}

void Renderer::addColorPointToMesh(glm::vec4 p)
{
	this->uncommitedMesh.colors.push_back(p);

}

void Renderer::addColorPointsToMesh(glm::vec4 *points, ...)
{
	glm::vec4 *p;
	
	for(p = points; p != nullptr; ++p)
	{
		this->addColorPointToMesh(*p);			
	}	
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

	return currentid;	
}
