#include "text.h"
#include <vector>
#include "stdio.h"
#include "3dgeom.h"
#include "renderer.h"

float Text::textSize = 0.3;

Text::Text(void)
{
	this->hasInitialized = false;
	this->theta = 0;

}


Text::~Text(void)
{

}

void Text::update(Renderer* renderer, float dt)
{
	theta += 100*dt;
	if(theta > 360)
		theta -= 360;

	std::vector<unsigned int>::iterator it;

	for(it = objectIds.begin(); it != objectIds.end(); ++it)
	{
		renderer->setObjectRotation(*it, glm::vec3(0,theta,0));
	}	
}

void Text::addNumberTexture(GLuint texture, int number)
{
	if((number < 10 && number > 0) || this->textsAdded[number])
	{
		this->textsAdded[number] = true;
		this->textures[number] = texture;	
	}
	this->checkIfInitialized();
}

void Text::checkIfInitialized()
{
	for(int i = 0; i < 10; ++i)
	{
		if(!this->textsAdded[i])
		{
			return;
		}
	}
	this->hasInitialized = true;
}


void Text::addNumber(int number, glm::vec3 position, Renderer* renderer)
{
	//if(!this->hasInitialized)
	//	return;
	std::vector<int> numbers;
	while(number >= 1)
	{
		int residue = number%10;
		number /= 10;
		numbers.push_back(residue);
	}

	int objectId = renderer->createObject();

	objectIds.push_back(objectId);

	renderer->setObjectTranslation(objectId, position);

	for(int i = 0; i < numbers.size(); ++i)
	{
		renderer->startMesh(objectId);
		renderer->meshUsesIndexes(false);
		renderer->addTextureToMesh(renderer->textTextures[numbers[i]]);

		GLfloat textIndexes[CUBE_NOINDEX_SIZE];
		GLfloat textNormals[CUBE_NOINDEX_SIZE];
		GLfloat textColors[CUBE_NOINDEX_COLOR_SIZE];
		GLfloat textUVs[CUBE_NOINDEX_UV_SIZE];

		glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

		createNonIndexedCube(textIndexes, textNormals, Text::textSize, Text::textSize, Text::textSize/10, true);
		createFaceColorsForNonIndexedCube(textColors, color, color, color, color, color, color);
		createUVsForNonIndexedCube(textUVs, 1);
	
		for(int i = 0, j = 0; i < CUBE_NOINDEX_SIZE && j < CUBE_NOINDEX_COLOR_SIZE; i += 3, j += 4)
		{
			renderer->addPointToMesh(glm::vec3(textIndexes[i], textIndexes[i+1], textIndexes[i+2]), glm::vec4(textColors[j], textColors[j+1], textColors[j+2], textColors[j+3]), glm::vec3(textNormals[i], textNormals[i+1], textNormals[i+2])); 		
		}

		renderer->setUVsToMesh(textUVs, CUBE_NOINDEX_UV_SIZE);
		renderer->addTranslationToMesh(glm::vec3(numbers.size() == 1 ? 0 : numbers.size() == 2 ?  i == 0 ? Text::textSize : -Text::textSize : numbers.size()*Text::textSize - 2*i*Text::textSize, 0, 0));
		renderer->commitMesh(objectId);
	}
}

std::vector<GLuint> Text::getTextures()
{
	std::vector<GLuint> textures;
	for(int i = 0; i < 10; ++i)
	{
		textures.push_back(this->textures[i]);
	}
	return textures;
}