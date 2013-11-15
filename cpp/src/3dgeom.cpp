#include "3dgeom.h"
#include <glm\glm.hpp>
#include <vector>

/*
Creates a non indexed cube, returning a cube with 36 points.
*/
void createNonIndexedCube(GLfloat* array, GLfloat* normalArray, float sizex, float sizey, float sizez, bool createNormalVectors)
{

	glm::vec3 p1, p2, p3, p4, p5, p6, p7, p8;

	p1 = glm::vec3(sizex, sizey, sizez);
	p2 = glm::vec3(-sizex, sizey, sizez);
	p3 = glm::vec3(sizex, -sizey, sizez);
	p4 = glm::vec3(-sizex, -sizey, sizez);
	p5 = glm::vec3(sizex, sizey, -sizez);
	p6 = glm::vec3(-sizex, sizey, -sizez);
	p7 = glm::vec3(sizex, -sizey, -sizez);
	p8 = glm::vec3(-sizex, -sizey, -sizez);

	std::vector<glm::vec3> data;
	
	// Allocate 36 places
	data.reserve(36);

	//Front face
	addTripletToVector(&data, p2, p4, p1);
	addTripletToVector(&data, p4, p3, p1);

	//Back face
	addTripletToVector(&data, p5, p7, p6);
	addTripletToVector(&data, p7, p8, p6);

	//Right face
	addTripletToVector(&data, p1, p3, p5);
	addTripletToVector(&data, p3, p7, p5);

	//Left face
	addTripletToVector(&data, p6, p8, p2);
	addTripletToVector(&data, p8, p4, p2);

	//Top face
	addTripletToVector(&data, p6, p2, p5);
	addTripletToVector(&data, p2, p1, p5);

	//Bottom face
	addTripletToVector(&data, p3, p4, p7);
	addTripletToVector(&data, p4, p8, p7);

	memcpy(array, (GLfloat*)&data[0], sizeof(data[0])*data.size());

	if(createNormalVectors)
	{
		glm::vec3 n1, n2, n3, n4, n5, n6;
		n1 = glm::vec3( 1,  0,  0);
		n2 = glm::vec3( 0,  1,  0);
		n3 = glm::vec3( 0,  0,  1);
		n4 = glm::vec3(-1,  0,  0);
		n5 = glm::vec3( 0, -1,  0);
		n6 = glm::vec3( 0,  0, -1);

		std::vector<glm::vec3> normalData;
		data.reserve(36);

		//Front face
		addTripletToVector(&normalData, n3, n3, n3);
		addTripletToVector(&normalData, n3, n3, n3);

		//Back face
		addTripletToVector(&normalData, n6, n6, n6);
		addTripletToVector(&normalData, n6, n6, n6);
	
		//Right face
		addTripletToVector(&normalData, n1, n1, n1);
		addTripletToVector(&normalData, n1, n1, n1);

		//Left face
		addTripletToVector(&normalData, n4, n4, n4);
		addTripletToVector(&normalData, n4, n4, n4);
	
		//Top face
		addTripletToVector(&normalData, n2, n2, n2);
		addTripletToVector(&normalData, n2, n2, n2);
		
		//Bottom face
		addTripletToVector(&normalData, n5, n5, n5);
		addTripletToVector(&normalData, n5, n5, n5);

		memcpy(normalArray, (GLfloat*)&data[0], sizeof(normalData[0])*normalData.size());
	}
}

/*
Generate colors for all faces of cube
*/
void createFaceColorsForNonIndexedCube(	
	GLfloat* array,
	glm::vec4 colorFront,
	glm::vec4 colorBack,
	glm::vec4 colorRight,
	glm::vec4 colorLeft,
	glm::vec4 colorTop,
	glm::vec4 colorBottom
)
{
	std::vector<glm::vec4> data;
	data.reserve(36);

	addTripletToVector(&data, colorFront, colorFront, colorFront);
	addTripletToVector(&data, colorFront, colorFront, colorFront);

	addTripletToVector(&data, colorBack, colorBack, colorBack);
	addTripletToVector(&data, colorBack, colorBack, colorBack);

	addTripletToVector(&data, colorRight, colorRight, colorRight);
	addTripletToVector(&data, colorRight, colorRight, colorRight);

	addTripletToVector(&data, colorLeft, colorLeft, colorLeft);
	addTripletToVector(&data, colorLeft, colorLeft, colorLeft);

	addTripletToVector(&data, colorTop, colorTop, colorTop);
	addTripletToVector(&data, colorTop, colorTop, colorTop);

	addTripletToVector(&data, colorBottom, colorBottom, colorBottom);
	addTripletToVector(&data, colorBottom, colorBottom, colorBottom);

	memcpy(array, (GLfloat*)&data[0], sizeof(data[0])*data.size());
}

void createUVsForNonIndexedCube(GLfloat* uvArray, int facecount)
{
	float data[36*2] =
    {
        // Front face
		0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        // Right face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Back face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Left face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Top face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        // Bottom face
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
	};

	memcpy(uvArray, (GLfloat*)&data[0], sizeof(data[0])*6*facecount*2);
}

void create8IndexedCube(GLfloat* vertexArray, GLfloat* normalArray, GLint* indexArray, float sizex, float sizey, float sizez, bool createNormalVectors)
{


}

/*
Creates an indexed cube. The final parameter is set to true if the cube is to be 8-indexed instead of 24-indexed.
*/
void create24IndexedCube(GLfloat* vertexArray, GLfloat* normalArray, GLint* indexArray, float sizex, float sizey, float sizez, bool createNormalVectors)
{
	glm::vec3 p1, p2, p3, p4, p5, p6, p7, p8;

	p1 = glm::vec3(sizex, sizey, sizez);
	p2 = glm::vec3(-sizex, sizey, sizez);
	p3 = glm::vec3(sizex, -sizey, sizez);
	p4 = glm::vec3(-sizex, -sizey, sizez);
	p5 = glm::vec3(sizex, sizey, -sizez);
	p6 = glm::vec3(-sizex, sizey, -sizez);
	p7 = glm::vec3(sizex, -sizey, -sizez);
	p8 = glm::vec3(-sizex, -sizey, -sizez);

	std::vector<glm::vec3> vertexData;
	std::vector<unsigned int> indices;
	vertexData.reserve(24);
	indices.reserve(36);
	
	// Front face
	addTetradToVector(&vertexData, p1, p2, p3, p4);
	addTripletToVector<unsigned int>(&indices, 2, 4, 1);
	addTripletToVector<unsigned int>(&indices, 4, 3, 1);

	//Back face
	addTetradToVector(&vertexData, p5, p6, p7, p8);
	addTripletToVector<unsigned int>(&indices, 5, 7, 6);
	addTripletToVector<unsigned int>(&indices, 7, 8, 6);

	//Right face
	addTetradToVector(&vertexData, p1, p3, p5, p7);
	addTripletToVector<unsigned int>(&indices, 9, 10, 11);
	addTripletToVector<unsigned int>(&indices, 10, 12, 11);

	//Left face
	addTetradToVector(&vertexData, p5, p6, p7, p8);
	addTripletToVector<unsigned int>(&indices, 5, 7, 6);
	addTripletToVector<unsigned int>(&indices, 7, 8, 6);

	//Top face
	addTetradToVector(&vertexData, p5, p6, p7, p8);
	addTripletToVector<unsigned int>(&indices, 5, 7, 6);
	addTripletToVector<unsigned int>(&indices, 7, 8, 6);

	//Bottom face
	addTetradToVector(&vertexData, p5, p6, p7, p8);
	addTripletToVector<unsigned int>(&indices, 5, 7, 6);
	addTripletToVector<unsigned int>(&indices, 7, 8, 6);

	if(createNormalVectors)
	{
		glm::vec3 n1, n2, n3, n4, n5, n6;
		n1 = glm::vec3( 1,  0,  0);
		n2 = glm::vec3( 0,  1,  0);
		n3 = glm::vec3( 0,  0,  1);
		n4 = glm::vec3(-1,  0,  0);
		n5 = glm::vec3( 0, -1,  0);
		n6 = glm::vec3( 0,  0, -1);
	}	
}

void createColorsForIndexedCube
	(
	GLfloat* colorArray,
	GLfloat &indexArray,
	glm::vec4 colorFront,
	glm::vec4 colorBack,
	glm::vec4 colorRight,
	glm::vec4 colorLeft,
	glm::vec4 colorTop,
	glm::vec4 colorBottom
)
{

}

/*
Helper method for adding a tetrad to a std::vector
*/
template<typename T>
void addTetradToVector(std::vector<T>* vec, T p1, T p2 , T p3, T p4)
{
	vec->push_back(p1);
	vec->push_back(p2);
	vec->push_back(p3);	
	vec->push_back(p4);	
}

/*
Helper method for adding a triplet to a std::vector
*/
template<typename T>
void addTripletToVector(std::vector<T>* vec, T p1, T p2, T p3)
{
	vec->push_back(p1);
	vec->push_back(p2);
	vec->push_back(p3);
}
