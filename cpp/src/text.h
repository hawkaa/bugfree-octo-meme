#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "math.h"

class Renderer;

class Text
{
public:
	Text(void);
	~Text(void);

	void update(Renderer* renderer, float dt);
	void addNumberTexture(GLuint texture, int number);
	void addNumber(int number, glm::vec3 position, Renderer* renderer);

	std::vector<GLuint> getTextures();

	static float textSize;

private:
	std::vector<unsigned int> objectIds;

	float theta;

	bool hasInitialized;
	void checkIfInitialized();
	bool textsAdded[10];

	GLuint textures[10];
};

