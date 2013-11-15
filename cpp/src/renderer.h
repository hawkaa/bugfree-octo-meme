#pragma once

#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "loader.h"
#include "imageobject.h"
#include "camera.h"
#include "text.h"
#include "3dgeom.h"

class Loader;
class Camera;
class Text;

struct Mesh
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec2> uvs;
	std::vector<unsigned int> indices;
	
	GLuint texture;

	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	bool useIndices;
	bool useTexture;
};

struct MeshBuffer
{
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint uvBuffer;

	GLuint texture;
	
	int indexSize;
	
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
	
	bool useIndices;
	bool useTexture;
};

struct MeshObject
{
	std::vector<MeshBuffer> buffers;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};


class Renderer
{
public:
	Renderer(Loader* loader, Camera* camera, Text* text);
	~Renderer(void);

	void invalidate();
	void invalidate(int);

	void startMesh();
	void startMesh(int objectId);

	void addPointToMesh(glm::vec3& vertex, glm::vec4& color, glm::vec3& normal);
	void addTriangleToMesh(unsigned int, unsigned int, unsigned int);
	
	void addUVToMesh(glm::vec2 uv);
	void setUVsToMesh(GLfloat uvs[], int length);
	void setUVsTomesh(std::vector<glm::vec2> uvs);

	void addTranslationToMesh(glm::vec3 translation);
	void addRotationToMesh(glm::vec3 rotation);
	void addScaleToMesh(glm::vec3 scale);

	void addTextureToMesh(GLuint textureID);
	void removeTextureFromMesh();

	void meshUsesIndexes(bool value);

	int commitMesh();
	int commitMesh(int objectId);

	void addImageObject(ImageObject& object);
	void createEllipsoid(float x, float y, float radius, glm::vec4 color);	

	void setBufferTranslation(int id, glm::vec3 translation);
	void setBufferRotation(int id, glm::vec3 rotation);
	void setBufferScale(int id, glm::vec3 scale);

	void renderACube();

	int createObject();
	void addMeshToObject(int objectId, int meshId);
	void addMeshToObject(int objectId, MeshBuffer buf);

	void setObjectTranslation(int id, glm::vec3 translation);
	void setObjectRotation(int id, glm::vec3 rotation);
	void setObjectScale(int id, glm::vec3 scale);

	std::vector<GLuint> textTextures;

private:
	void cascadeIndexChange(int start, int end, bool shiftLeft); 

protected:

	Camera* camera;
	Text* text;

	GLuint programID;

	GLuint posHandle;
	GLuint colorHandle;
	GLuint normalHandle;

	GLuint MVPHandle;
	GLuint modelHandle;
	GLuint viewHandle;

	GLuint textureProgramID;
	GLuint texturePosHandle;
	GLuint textureColorHandle;
	GLuint textureNormalHandle;

	GLuint textureMVPHandle;
	GLuint textureModelHandle;
	GLuint textureViewHandle;
	GLuint textureUVHandle;
	GLuint textureSamplerHandle;

	int currentid;
	int objectcurrentid;

	std::vector<unsigned int> keyLookUp;

	bool meshstarted;
	bool meshusingobject;
	bool textstarted;

	std::vector<MeshBuffer> activeMeshes;
	std::vector<MeshObject> activeObjects;

	Mesh uncommitedMesh;
};

