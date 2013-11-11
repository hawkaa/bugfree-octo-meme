#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "loader.h"
#include "imageobject.h"
#include "camera.h"

class Loader;
class Camera;

struct Mesh
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;
	std::vector<unsigned int> indices;

};

struct MeshBuffer
{
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	int indexSize;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class Renderer
{
public:
	Renderer(Loader* loader, Camera* camera);
	~Renderer(void);

	void invalidate();
	void invalidate(int);

	void startMesh();
	void addVertexToMesh(glm::vec3);
	void addColorToMesh(glm::vec4 p);
	void addNormalToMesh(glm::vec3 p);
	void addTriangleToMesh(unsigned int, unsigned int, unsigned int);

	void addImageObject(ImageObject& object);
	int commitMesh();

private:
	Camera* camera;

	GLuint programID;

	GLuint posHandle;
	GLuint colorHandle;
	GLuint normalHandle;

	GLuint MVPHandle;
	GLuint modelHandle;
	GLuint viewHandle;

	int currentid;
	bool meshstarted;
	std::vector<MeshBuffer> activeMeshes;
	Mesh uncommitedMesh;
};

