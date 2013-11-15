#include "renderer.h"
#include "loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// Hei Håkon, dette er awesome kode, er du ikke enig? :D
float invsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalves = 1.5f;
 
    x2 = number*0.5f;
    y = number;
    i = *(long*)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float*)&i;
    y = y *(threehalves-(x2*y*y));
    return y;
}

void normalize(glm::vec3& point)
{
	point *= invsqrt(point.x*point.x + point.y*point.y + point.z*point.z);
}

Renderer::Renderer(Loader* loader, Camera* camera, Text* text)
{
	this->camera = camera;
	this->text = text;

	this->currentid = -1;
	this->objectcurrentid = -1;
	this->textTextures = text->getTextures();
	this->programID = loader->LoadShaders("vertexshader.shader", "fragmentshader.shader");
	
	this->MVPHandle = glGetUniformLocation(this->programID, "MVP");
	this->modelHandle = glGetUniformLocation(this->programID, "Model");
	this->viewHandle = glGetUniformLocation(this->programID, "View");
	
	this->posHandle = glGetAttribLocation(this->programID, "vertexPosition_modelspace");
	this->colorHandle = glGetAttribLocation(this->programID, "vertexColor");
	this->normalHandle = glGetAttribLocation(this->programID, "vertexNormal_modelspace");

	this->textureProgramID = loader->LoadShaders("texturevertexshader.shader", "texturefragmentshader.shader");

	this->textureMVPHandle = glGetUniformLocation(this->textureProgramID, "MVP");
	this->textureModelHandle = glGetUniformLocation(this->textureProgramID, "Model");
	this->textureViewHandle = glGetUniformLocation(this->textureProgramID, "View");
	this->textureSamplerHandle = glGetUniformLocation(this->textureProgramID, "myTexture");

	this->texturePosHandle = glGetAttribLocation(this->textureProgramID, "vertexPosition_modelspace");
	this->textureColorHandle = glGetAttribLocation(this->textureProgramID, "vertexColor");
	this->textureNormalHandle = glGetAttribLocation(this->textureProgramID, "vertexNormal_modelspace");
	this->textureUVHandle = glGetAttribLocation(this->textureProgramID, "vertexUV");
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

	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();

	for(it = activeMeshes.begin(); it != activeMeshes.end(); ++it)
	{
		if(it->useTexture)
		{
			glUseProgram(this->textureProgramID);
			glm::mat4 rotMat, scaleMat, transMat;

			rotMat = glm::rotate(glm::mat4(1.0f), it->rotation.x, glm::vec3(1,0,0));
			rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.y, glm::vec3(0,1,0));
			rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.z, glm::vec3(0,0,1));

			scaleMat = glm::scale(glm::mat4(1.0f), it->scale);
			transMat = glm::translate(glm::mat4(1.0f), it->translation);

			glm::mat4 modelMatrix = transMat * scaleMat * rotMat;
			glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	
			glUniformMatrix4fv(textureMVPHandle, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(textureModelHandle, 1, GL_FALSE, &modelMatrix[0][0]);
			glUniformMatrix4fv(textureViewHandle, 1, GL_FALSE, &viewMatrix[0][0]);	

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, it->texture);
			glUniform1i(textureSamplerHandle, 0);

			glEnableVertexAttribArray(texturePosHandle);
			glBindBuffer(GL_ARRAY_BUFFER, it->vertexBuffer);
			glVertexAttribPointer(texturePosHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	

			glEnableVertexAttribArray(textureColorHandle);
			glBindBuffer(GL_ARRAY_BUFFER, it->colorBuffer);
			glVertexAttribPointer(textureColorHandle, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(textureNormalHandle);
			glBindBuffer(GL_ARRAY_BUFFER, it->normalBuffer);
			glVertexAttribPointer(textureNormalHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glEnableVertexAttribArray(textureUVHandle);
			glBindBuffer(GL_ARRAY_BUFFER, it->uvBuffer);
			glVertexAttribPointer(textureUVHandle, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			if(it->useIndices)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->indexBuffer);
				glDrawElements(GL_TRIANGLES, it->indexSize, GL_UNSIGNED_INT, (void*)0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, it->indexSize);
			}

			glDisableVertexAttribArray(texturePosHandle);
			glDisableVertexAttribArray(textureColorHandle);
			glDisableVertexAttribArray(textureNormalHandle);
		}
		else
		{
			glUseProgram(this->programID);
			
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

			if(it->useIndices)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->indexBuffer);

				glDrawElements(GL_TRIANGLES, it->indexSize, GL_UNSIGNED_INT, (void*)0);
			}
			else
			{
				glDrawArrays(GL_TRIANGLES, 0, it->indexSize);
			}

			glDisableVertexAttribArray(posHandle);
			glDisableVertexAttribArray(colorHandle);
			glDisableVertexAttribArray(normalHandle);
		}
	}	

	std::vector<MeshObject>::iterator oit;
	
	for(oit = activeObjects.begin(); oit != activeObjects.end(); ++oit)
	{
		if(oit->buffers.size() == 0)
			continue;
		glm::mat4 oRotMat, oScaleMat, oTransMat, oModelMatrix;

		oRotMat = glm::rotate(glm::mat4(1.0f), oit->rotation.x, glm::vec3(1,0,0));
		oRotMat *= glm::rotate(glm::mat4(1.0f), oit->rotation.y, glm::vec3(0,1,0));
		oRotMat *= glm::rotate(glm::mat4(1.0f), oit->rotation.z, glm::vec3(0,0,1));

		oScaleMat = glm::scale(glm::mat4(1.0f), oit->scale);
		oTransMat = glm::translate(glm::mat4(1.0f), oit->translation);

		oModelMatrix = oTransMat * oScaleMat * oRotMat;

		for(it = oit->buffers.begin(); it != oit->buffers.end(); ++it)
		{
			if(it->useTexture)
			{
				glUseProgram(this->textureProgramID);
				glm::mat4 rotMat, scaleMat, transMat;

				rotMat = glm::rotate(glm::mat4(1.0f), it->rotation.x, glm::vec3(1,0,0));
				rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.y, glm::vec3(0,1,0));
				rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.z, glm::vec3(0,0,1));

				scaleMat = glm::scale(glm::mat4(1.0f), it->scale);
				transMat = glm::translate(glm::mat4(1.0f), it->translation);

				glm::mat4 modelMatrix = transMat * scaleMat * rotMat;
				glm::mat4 MVP = projectionMatrix * viewMatrix * oModelMatrix * modelMatrix;
	
				glUniformMatrix4fv(textureMVPHandle, 1, GL_FALSE, &MVP[0][0]);
				glUniformMatrix4fv(textureModelHandle, 1, GL_FALSE, &modelMatrix[0][0]);
				glUniformMatrix4fv(textureViewHandle, 1, GL_FALSE, &viewMatrix[0][0]);	

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, it->texture);
				glUniform1i(textureSamplerHandle, 0);

				glEnableVertexAttribArray(texturePosHandle);
				glBindBuffer(GL_ARRAY_BUFFER, it->vertexBuffer);
				glVertexAttribPointer(texturePosHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	

				glEnableVertexAttribArray(textureColorHandle);
				glBindBuffer(GL_ARRAY_BUFFER, it->colorBuffer);
				glVertexAttribPointer(textureColorHandle, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glEnableVertexAttribArray(textureNormalHandle);
				glBindBuffer(GL_ARRAY_BUFFER, it->normalBuffer);
				glVertexAttribPointer(textureNormalHandle, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glEnableVertexAttribArray(textureUVHandle);
				glBindBuffer(GL_ARRAY_BUFFER, it->uvBuffer);
				glVertexAttribPointer(textureUVHandle, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

				if(it->useIndices)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->indexBuffer);
					glDrawElements(GL_TRIANGLES, it->indexSize, GL_UNSIGNED_INT, (void*)0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLES, 0, it->indexSize);
				}

				glDisableVertexAttribArray(texturePosHandle);
				glDisableVertexAttribArray(textureColorHandle);
				glDisableVertexAttribArray(textureNormalHandle);
			}
			else
			{
				glUseProgram(this->programID);
			
				glm::mat4 rotMat, scaleMat, transMat;

				rotMat = glm::rotate(glm::mat4(1.0f), it->rotation.x, glm::vec3(1,0,0));
				rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.y, glm::vec3(0,1,0));
				rotMat *= glm::rotate(glm::mat4(1.0f), it->rotation.z, glm::vec3(0,0,1));

				scaleMat = glm::scale(glm::mat4(1.0f), it->scale);
				transMat = glm::translate(glm::mat4(1.0f), it->translation);

				glm::mat4 modelMatrix = transMat * scaleMat * rotMat;
				glm::mat4 MVP = projectionMatrix * viewMatrix * oModelMatrix * modelMatrix;
	
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

				if(it->useIndices)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, it->indexBuffer);

					glDrawElements(GL_TRIANGLES, it->indexSize, GL_UNSIGNED_INT, (void*)0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLES, 0, it->indexSize);
				}

				glDisableVertexAttribArray(posHandle);
				glDisableVertexAttribArray(colorHandle);
				glDisableVertexAttribArray(normalHandle);
			}
		}	
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
	this->meshUsesIndexes(true);
	
	radius = radius/40;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;
	std::vector<unsigned int> indices;

	int a, b;
	
	a = radius;
	b = 2;
	int res = 10;

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

			glm::vec3 p = glm::vec3(2*radius*sgn(radius)*sqrt(1-in*in)*cos(theta), 2*radius*sgn(radius)*sqrt(1-in*in)*sin(theta), i);
			vertices.push_back(p);
			normalize(p);
			normals.push_back(p);
			colors.push_back(color);
			//colors.push_back(glm::vec4(p,1));
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

	this->addTranslationToMesh(glm::vec3(x, y, 0)*(0.06f));
	this->commitMesh();

	this->text->addNumber(15, glm::vec3(x,y+15,0)*0.06f, this);
}

void Renderer::startMesh(int objectId)
{
	meshstarted = true;
	meshusingobject = true;
	uncommitedMesh.useIndices = false;
	uncommitedMesh.useTexture = false;
	uncommitedMesh.translation = glm::vec3(1.0f);
	uncommitedMesh.rotation = glm::vec3(1.0f);
	uncommitedMesh.scale = glm::vec3(1.0f);
}

void Renderer::startMesh()
{
	++currentid;
	meshstarted = true;
	meshusingobject = false;
	uncommitedMesh.useIndices = false;
	uncommitedMesh.useTexture = false;
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

void Renderer::setUVsToMesh(GLfloat uvs[], int length)
{
	for(int i = 0; i < length; i+=2)
	{
		this->uncommitedMesh.uvs.push_back(glm::vec2(uvs[i], uvs[i+1]));
	}
}

void Renderer::setUVsTomesh(std::vector<glm::vec2> uvs)
{
	std::vector<glm::vec2>::iterator it;
	for(it = uvs.begin(); it != uvs.end(); ++it)
	{
		this->uncommitedMesh.uvs.push_back(*it);
	}
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

void Renderer::addTextureToMesh(GLuint textureID)
{
	this->uncommitedMesh.texture = textureID;
	this->uncommitedMesh.useTexture = true;
}

void Renderer::removeTextureFromMesh()
{
	this->uncommitedMesh.texture = -1;
	this->uncommitedMesh.useTexture = false;
}

void Renderer::meshUsesIndexes(bool value)
{
	this->uncommitedMesh.useIndices = value;
}

int Renderer::commitMesh(int objectId)
{
	if(!this->meshusingobject)
	{
		printf("You are trying to commit a mesh not submitted to an object to an object, please use \"commitMesh(void)\"");
		return -1;
	}

	if(objectId > objectcurrentid || objectId < 0)
	{
		printf("Object %i doesn't exist Im afraid, wanna try using createObject()?", objectId);
	}

	if(this->uncommitedMesh.colors.size() == 0 || this->uncommitedMesh.vertices.size() == 0 || this->uncommitedMesh.normals.size() == 0)
	{
		printf("You are trying to commit a mesh with some data missing, please make sure that color, normal and vertex data are submitted");
		return -1;
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
	
	if(uncommitedMesh.useIndices)
	{
		if(this->uncommitedMesh.indices.size() == 0)
		{
			printf("Okey, so you are trying to use indices.. Might be a good idea to add some!");
			return -1;
		}

		glGenBuffers(1, &buffer.indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int)*uncommitedMesh.indices.size(), &uncommitedMesh.indices[0], GL_STATIC_DRAW);
		buffer.indexSize = uncommitedMesh.indices.size();
		buffer.useIndices = true;
	}
	else
	{
		buffer.useIndices = false;
		buffer.indexSize = uncommitedMesh.vertices.size()*3;
	}

	if(uncommitedMesh.useTexture)
	{
		glGenBuffers(1, &buffer.uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uncommitedMesh.uvs.size(), &uncommitedMesh.uvs[0], GL_STATIC_DRAW);
		buffer.texture = uncommitedMesh.texture;
		buffer.useTexture = true;
	}
	else
	{
		buffer.useTexture = false;
	}
	
	buffer.translation = uncommitedMesh.translation;
	buffer.rotation = uncommitedMesh.rotation;
	buffer.scale = uncommitedMesh.scale;

	//printf("\nMesh commited with %d vertices, %d normals, %d colorpoints, %d indices", this->uncommitedMesh.vertices.size(), this->uncommitedMesh.colors.size(), this->uncommitedMesh.normals.size(), this->uncommitedMesh.indices.size());
	activeObjects[objectId].buffers.push_back(buffer);

	uncommitedMesh.vertices.clear();
	uncommitedMesh.colors.clear();
	uncommitedMesh.normals.clear();
	uncommitedMesh.indices.clear();

	this->meshstarted = false;
	this->meshusingobject = false;
	return activeObjects[objectId].buffers.size()-1;
}

int Renderer::commitMesh()
{
	if(this->meshusingobject)
	{
		printf("You are trying to commit a mesh submitted to an object without specifying the objectid, please use \"commitMesh(int objectId)\"");
		return -1;
	}
	if(this->uncommitedMesh.colors.size() == 0 || this->uncommitedMesh.vertices.size() == 0 || this->uncommitedMesh.normals.size() == 0)
	{
		printf("You are trying to commit a mesh with some data missing, please make sure that color, normal and vertex data are submitted");
		currentid--;
		return -1;
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
	
	if(uncommitedMesh.useIndices)
	{
		if(this->uncommitedMesh.indices.size() == 0)
		{
			printf("Okey, so you are trying to use indices.. Might be a good idea to add some!");
		}

		glGenBuffers(1, &buffer.indexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int)*uncommitedMesh.indices.size(), &uncommitedMesh.indices[0], GL_STATIC_DRAW);
		buffer.indexSize = uncommitedMesh.indices.size();
		buffer.useIndices = true;
	}
	else
	{
		buffer.useIndices = false;
		buffer.indexSize = uncommitedMesh.vertices.size()*3;
	}

	if(uncommitedMesh.useTexture)
	{
		glGenBuffers(1, &buffer.uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*uncommitedMesh.uvs.size(), &uncommitedMesh.uvs[0], GL_STATIC_DRAW);
		buffer.texture = uncommitedMesh.texture;
		buffer.useTexture = true;
	}
	else
	{
		buffer.useTexture = false;
	}
	
	buffer.translation = uncommitedMesh.translation;
	buffer.rotation = uncommitedMesh.rotation;
	buffer.scale = uncommitedMesh.scale;

	//printf("\nMesh commited with %d vertices, %d normals, %d colorpoints, %d indices", this->uncommitedMesh.vertices.size(), this->uncommitedMesh.colors.size(), this->uncommitedMesh.normals.size(), this->uncommitedMesh.indices.size());
	activeMeshes.push_back(buffer);

	uncommitedMesh.vertices.clear();
	uncommitedMesh.colors.clear();
	uncommitedMesh.normals.clear();
	uncommitedMesh.indices.clear();

	this->keyLookUp.push_back(currentid);
	this->meshstarted = false;
	return currentid;	
}

void Renderer::setBufferTranslation(int id, glm::vec3 translation)
{
	int iid = keyLookUp[id];
	if(iid > activeMeshes.size() || iid < 0)
		return;
	activeMeshes[iid].translation = translation;
}

void Renderer::setBufferRotation(int id, glm::vec3 rotation)
{
	int iid = keyLookUp[id];
	if(iid > activeMeshes.size() || iid < 0)
		return;
	activeMeshes[iid].rotation = rotation;
}

void Renderer::setBufferScale(int id, glm::vec3 scale)
{
	int iid = keyLookUp[id];
	if(iid > activeMeshes.size() || iid < 0)
		return;
	activeMeshes[iid].scale = scale;

}

int Renderer::createObject()
{
	MeshObject obj;
	obj.rotation = glm::vec3(1.0f);
	obj.translation = glm::vec3(1.0f);
	obj.scale = glm::vec3(1.0f);
	this->activeObjects.push_back(obj);
	return ++objectcurrentid;

}

void Renderer::addMeshToObject(int objectId, int meshId)
{
	if(objectId >= activeObjects.size())
		return;
	this->activeObjects[objectId].buffers.push_back(activeMeshes[meshId]);
	this->activeMeshes.erase(activeMeshes.begin() + meshId);
}

void Renderer::addMeshToObject(int objectId, MeshBuffer buf)
{

}

void Renderer::setObjectTranslation(int id, glm::vec3 translation)
{
	if(id > activeObjects.size())
		return;
	this->activeObjects[id].translation = translation;
}

void Renderer::setObjectRotation(int id, glm::vec3 rotation)
{

	if(id > activeObjects.size())
		return;
	this->activeObjects[id].rotation = rotation;
}

void Renderer::setObjectScale(int id, glm::vec3 scale)
{
	
	if(id > activeObjects.size())
		return;
	this->activeObjects[id].scale = scale;
}

void Renderer::cascadeIndexChange(int start, int end, bool shiftLeft)
{
	for(int i = start; i < end; ++i)
	{
		if(shiftLeft)
		{
			keyLookUp[i]--;	
		}
		else
		{
			keyLookUp[i]++;
		}
	}
}


