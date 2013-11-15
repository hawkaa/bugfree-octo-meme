#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#ifndef CUBE_NOINDEX_SIZE
#define CUBE_NOINDEX_SIZE 108
#endif

#ifndef CUBE_NOINDEX_COLOR_SIZE
#define CUBE_NOINDEX_COLOR_SIZE 144
#endif

#ifndef CUBE_INDEX_SIZE
#define CUBE_INDEX_SIZE 72
#endif

#ifndef CUBE_INDEX_COLOR_SIZE
#define CUBE_INDEX_COLOR_SIZE 96
#endif

#ifndef CUBE_NOINDEX_UV_SIZE
#define CUBE_NOINDEX_UV_SIZE 72
#endif


void createNonIndexedCube(GLfloat*, GLfloat* normalArray, float sizex, float sizey, float sizez, bool createNormalVectors);
void createFaceColorsForNonIndexedCube(GLfloat* array, glm::vec4 colorFront, glm::vec4 colorBack,	glm::vec4 colorRight, glm::vec4 colorLeft, glm::vec4 colorTop, glm::vec4 colorBottom);
void createUVsForNonIndexedCube(GLfloat* uvArray, int facecount);
void create8IndexedCube(GLfloat* vertexArray, GLfloat* normalArray, GLint* indexArray, float sizex, float sizey, float sizez, bool createNormalVectors);
void create24IndexedCube(GLfloat* vertexArray, GLfloat* normalArray, GLint* indexArray, float sizex, float sizey, float sizez, bool createNormalVectors);
void createColorsForIndexedCube(GLfloat* colorArray, GLint &indexArray,	glm::vec4 colorFront, glm::vec4 colorBack, glm::vec4 colorRight, glm::vec4 colorLeft, glm::vec4 colorTop, glm::vec4 colorBottom);

//Helper methods
template<typename T>
void addTripletToVector(std::vector<T>* vec, T p1, T p2, T p3);
template<typename T>
void addTetradToVector(std::vector<T>* vec, T p1, T p2 , T p3, T p4);