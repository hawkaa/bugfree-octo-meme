#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "math.h"

class Text
{
public:
	Text(void);
	~Text(void);

	void addNumberTexture(GLuint texture, int number);

	void addNumber(int number, glm::vec3 position);

private:
	bool hasInitialized;
	void checkIfInitialized();
	bool textsAdded[10];

	GLuint textures[10];
};

