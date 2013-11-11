#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

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
	Renderer(void);
	~Renderer(void);

	void invalidate();
	void invalidate(int);

	void startMesh();
	void addPointToMesh(glm::vec3);
	void addPointsToMesh(glm::vec3 *points, ...);	
	void addColorPointToMesh(glm::vec4 p);
	void addColorPointsToMesh(glm::vec4 *points, ...);
	void addTriangleToMesh(unsigned int, unsigned int, unsigned int);
	int commitMesh();

private:
	int currentid;
	bool meshstarted;
	std::vector<MeshBuffer> activeMeshes;
	Mesh uncommitedMesh;
};

